#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "module.h"
#include "isp.h"
#include "uart.h"

//#define MM32F031xx_q
//#define MM32F103xx_n
//#define MM32L0xx_n
//#define MM32F103xx_o
#define MM32SPIN2xx_p

#if defined MM32F031xx_q
    #include "MM32F031xx_q_sram_code.h"
#elif defined MM32F103xx_n
    #include "MM32F103xx_n_sram_code.h"
#elif defined MM32L0xx_n
    #include "MM32L0xx_n_sram_code.h"
#elif defined MM32F103xx_o
    #include "MM32F103xx_o_sram_code.h"
#elif defined MM32SPIN2xx_p
    #include "MM32SPIN2xx_p_sram_code.h"
#endif

//接收缓冲,最大USART_REC_LEN个字节
uint8_t g_chUartRxBuf[UART_REC_MAX_LEN];
uint8_t g_chUartTxBuf[UART_SEND_MAX_LEN];

//接收到的Bytes
uint16_t g_wUartRxCnt = 0;

//SRAM中计算校验和
static uint8_t SRAM_check_sum(uint8_t *pchBuff, uint16_t hwLength)
{
    uint32_t wAccumulationSum = 0;

    while (hwLength > 0)
    {
        wAccumulationSum += *pchBuff++;
        hwLength --;
    }

    wAccumulationSum &= 0x000000ff;         //校验和为累加和的低八位
    return (uint8_t)wAccumulationSum;
}

//Flash中计算校验和
uint8_t flash_check_sum(uint8_t *data, uint16_t len)
{
    uint8_t sum = 0;

    while (len--)
    {
        sum ^= *data++;
    }

    return sum;
}

//发送一帧数据
bool Send_SRAM_Frame(FrameDcb_t *pFrameDcb)
{
    __IO uint16_t num = 0;

    if (NULL == pFrameDcb)
    {
        return false;
    }

    memset(g_chUartTxBuf, 0, sizeof(g_chUartTxBuf));    //清空g_chUartTxBuf
    memset(g_chUartRxBuf, 0, sizeof(g_chUartRxBuf));    //清空g_chUartRxBuf
    g_chUartTxBuf[num++] = pFrameDcb->tDirection;       //帧数据方向
    g_chUartTxBuf[num++] = (uint8_t)(pFrameDcb->hwFrameLengh >> 8) & 0xFF;
    g_chUartTxBuf[num++] = (uint8_t)(pFrameDcb->hwFrameLengh) & 0xFF;//16位帧长度，
    g_chUartTxBuf[num++] = pFrameDcb->tCommand;         //帧命令

    if (HANDSHAKE_PROTOCOL == pFrameDcb->tCommand)      //握手命令
    {
        g_chUartTxBuf[num++] = SRAM_check_sum(g_chUartTxBuf, pFrameDcb->hwFrameLengh - 1);//填入校验和
    }

    if (LOAD_CONFIG == pFrameDcb->tCommand)//加载SRAM程序
    {
        //加载SRAM程序的配置信息
        g_chUartTxBuf[num++] = (uint8_t)(pFrameDcb->wConfig >> 24) & 0xFF;
        g_chUartTxBuf[num++] = (uint8_t)(pFrameDcb->wConfig >> 16) & 0xFF;
        g_chUartTxBuf[num++] = (uint8_t)(pFrameDcb->wConfig >> 8) & 0xFF;
        g_chUartTxBuf[num++] = (uint8_t)(pFrameDcb->wConfig) & 0xFF;
        //加载SRAM程序的当前地址
        g_chUartTxBuf[num++] = (uint8_t)(pFrameDcb->wAddress >> 24) & 0xFF;
        g_chUartTxBuf[num++] = (uint8_t)(pFrameDcb->wAddress >> 16) & 0xFF;
        g_chUartTxBuf[num++] = (uint8_t)(pFrameDcb->wAddress >> 8) & 0xFF;
        g_chUartTxBuf[num++] = (uint8_t)(pFrameDcb->wAddress) & 0xFF;

        if (START_CONFIG == pFrameDcb->wConfig)
        {
            //开始发送时填入数据长度
            g_chUartTxBuf[num++] = (uint8_t)(pFrameDcb->wDataLength >> 24) & 0xFF;
            g_chUartTxBuf[num++] = (uint8_t)(pFrameDcb->wDataLength >> 16) & 0xFF;
            g_chUartTxBuf[num++] = (uint8_t)(pFrameDcb->wDataLength >> 8) & 0xFF;
            g_chUartTxBuf[num++] = (uint8_t)(pFrameDcb->wDataLength) & 0xFF;
            g_chUartTxBuf[num++] = SRAM_check_sum(g_chUartTxBuf, pFrameDcb->hwFrameLengh - 1);//填入校验和
        }
        else if (SEND_CONFIG == pFrameDcb->wConfig)
        {
            //将SRAM程序分段拷贝到帧数据区
            memcpy(&g_chUartTxBuf[num++], pFrameDcb->pData, LOAD_CONFIG_DATA_LENGTH);
            g_chUartTxBuf[UART_SEND_MAX_LEN - 1] = SRAM_check_sum(g_chUartTxBuf, pFrameDcb->hwFrameLengh - 1);//填入校验和
        }
        else if (CONFIG_COMPLETE == pFrameDcb->wConfig)
        {
            memcpy(&g_chUartTxBuf[num++], pFrameDcb->pData, RAM_CODE_LENGTH % LOAD_CONFIG_DATA_LENGTH + 12);
            g_chUartTxBuf[UART_SEND_MAX_LEN - 1] = SRAM_check_sum(g_chUartTxBuf, pFrameDcb->hwFrameLengh - 1);
        }
    }

    if (READ_REGISTER == pFrameDcb->tCommand)//读寄存器
    {
        g_chUartTxBuf[num++] = (uint8_t)(pFrameDcb->wAddress >> 24) & 0xFF;
        g_chUartTxBuf[num++] = (uint8_t)(pFrameDcb->wAddress >> 16) & 0xFF;
        g_chUartTxBuf[num++] = (uint8_t)(pFrameDcb->wAddress >> 8) & 0xFF;
        g_chUartTxBuf[num++] = (uint8_t)(pFrameDcb->wAddress) & 0xFF;
        g_chUartTxBuf[num++] = SRAM_check_sum(g_chUartTxBuf, pFrameDcb->hwFrameLengh - 1);//填入校验和
    }

    //todo
    UART1_Send_Group(g_chUartTxBuf, pFrameDcb->hwFrameLengh);//UART发送一帧数据
    return true;
}

//发送一帧数据
bool Send_Flash_Frame(FrameDcb_t *pFrameDcb)
{
    if (NULL == pFrameDcb)
    {
        return false;
    }

    memset(g_chUartTxBuf, 0, sizeof(g_chUartTxBuf));
    memset(g_chUartRxBuf, 0, sizeof(g_chUartRxBuf));

    if (COMPATIBLE_SYNCBAUD_PROTOCOL == pFrameDcb->tCommand)
    {
        g_chUartTxBuf[0] = ISP_START_FRAME;
    }

    if (COMPATIBLE_ISP_GET == pFrameDcb->tCommand)
    {
        g_chUartTxBuf[0] = ISP_GET;
        g_chUartTxBuf[1] = ISP_GET ^ 0xFF;
    }

    if (COMPATIBLE_ISP_GET_VERSION == pFrameDcb->tCommand)
    {
        g_chUartTxBuf[0] = ISP_GET_VERSION;
        g_chUartTxBuf[1] = ISP_GET_VERSION ^ 0xFF;
    }

    if (COMPATIBLE_ISP_GET_ID == pFrameDcb->tCommand)
    {
        g_chUartTxBuf[0] = ISP_GET_ID;
        g_chUartTxBuf[1] = ISP_GET_ID ^ 0xFF;
    }

    if (COMPATIBLE_ISP_READ_MEMORY == pFrameDcb->tCommand)
    {
        if (pFrameDcb->wConfig == ONE_STEP)
        {
            g_chUartTxBuf[0] = ISP_READ_MEMORY;
            g_chUartTxBuf[1] = ISP_READ_MEMORY ^ 0xFF;
        }
        else if (pFrameDcb->wConfig == TWO_STEP)
        {
            g_chUartTxBuf[0] = (uint8_t)(pFrameDcb->wAddress >> 24) & 0xFF;
            g_chUartTxBuf[1] = (uint8_t)(pFrameDcb->wAddress >> 16) & 0xFF;
            g_chUartTxBuf[2] = (uint8_t)(pFrameDcb->wAddress >> 8) & 0xFF;
            g_chUartTxBuf[3] = (uint8_t)(pFrameDcb->wAddress) & 0xFF;
            g_chUartTxBuf[4] = flash_check_sum(g_chUartTxBuf, pFrameDcb->hwFrameLengh - 1);
        }
        else if (pFrameDcb->wConfig == THREE_STEP)
        {
            g_chUartTxBuf[0] = pFrameDcb->wDataLength;
            g_chUartTxBuf[1] = g_chUartTxBuf[0] ^ 0xFF;
        }
    }

    if (COMPATIBLE_ISP_GO == pFrameDcb->tCommand)
    {
        if (pFrameDcb->wConfig == ONE_STEP)
        {
            g_chUartTxBuf[0] = ISP_GO;
            g_chUartTxBuf[1] = ISP_GO ^ 0xFF;
        }
        else if (pFrameDcb->wConfig == TWO_STEP)
        {
            g_chUartTxBuf[0] = (uint8_t)(pFrameDcb->wAddress >> 24) & 0xFF;
            g_chUartTxBuf[1] = (uint8_t)(pFrameDcb->wAddress >> 16) & 0xFF;
            g_chUartTxBuf[2] = (uint8_t)(pFrameDcb->wAddress >> 8) & 0xFF;
            g_chUartTxBuf[3] = (uint8_t)(pFrameDcb->wAddress) & 0xFF;
            g_chUartTxBuf[4] = flash_check_sum(g_chUartTxBuf, pFrameDcb->hwFrameLengh - 1);
        }
    }

    if (COMPATIBLE_ISP_WRITE_MEMORY == pFrameDcb->tCommand)
    {
        if (pFrameDcb->wConfig == ONE_STEP)
        {
            g_chUartTxBuf[0] = ISP_WRITE_MEMORY;
            g_chUartTxBuf[1] = ISP_WRITE_MEMORY ^ 0xFF;
        }
        else if (pFrameDcb->wConfig == TWO_STEP)
        {
            g_chUartTxBuf[0] = (uint8_t)(pFrameDcb->wAddress >> 24) & 0xFF;
            g_chUartTxBuf[1] = (uint8_t)(pFrameDcb->wAddress >> 16) & 0xFF;
            g_chUartTxBuf[2] = (uint8_t)(pFrameDcb->wAddress >> 8) & 0xFF;
            g_chUartTxBuf[3] = (uint8_t)(pFrameDcb->wAddress) & 0xFF;
            g_chUartTxBuf[4] = flash_check_sum(g_chUartTxBuf, pFrameDcb->hwFrameLengh - 1);
        }
        else if (pFrameDcb->wConfig == THREE_STEP)
        {
            g_chUartTxBuf[0] = pFrameDcb->wDataLength;
            memcpy(&g_chUartTxBuf[1], pFrameDcb->pData, pFrameDcb->wDataLength + 1);
            g_chUartTxBuf[pFrameDcb->wDataLength + 2] = flash_check_sum(g_chUartTxBuf, pFrameDcb->wDataLength + 2);
        }
    }

    if (COMPATIBLE_ISP_ERASE_MEMORY == pFrameDcb->tCommand)
    {
        if (pFrameDcb->wConfig == ONE_STEP)
        {
            g_chUartTxBuf[0] = ISP_ERASE_MEMORY;
            g_chUartTxBuf[1] = ISP_ERASE_MEMORY ^ 0xFF;
        }
        else if (pFrameDcb->wConfig == TWO_STEP)
        {
            g_chUartTxBuf[0] = 0xFF;
            g_chUartTxBuf[1] = 0x00;
        }
    }

    if (COMPATIBLE_ISP_ERASE_MEMORY_EX == pFrameDcb->tCommand)
    {
        g_chUartTxBuf[0] = ISP_ERASE_MEMORY_EX;
        g_chUartTxBuf[1] = ISP_ERASE_MEMORY_EX ^ 0xFF;
    }

    if (COMPATIBLE_ISP_WRITE_PROTECT == pFrameDcb->tCommand)
    {
        if (pFrameDcb->wConfig == ONE_STEP)
        {
            g_chUartTxBuf[0] = ISP_WRITE_PROTECT;
            g_chUartTxBuf[1] = ISP_WRITE_PROTECT ^ 0xFF;
        }
        else if (pFrameDcb->wConfig == TWO_STEP)
        {
            g_chUartTxBuf[0] = 0x00;
            g_chUartTxBuf[1] = 0xFF;
            g_chUartTxBuf[4] = flash_check_sum(g_chUartTxBuf, pFrameDcb->hwFrameLengh - 1);
        }
    }

    if (COMPATIBLE_ISP_WRITE_UNPROTECT == pFrameDcb->tCommand)
    {
        if (pFrameDcb->wConfig == ONE_STEP)
        {
            g_chUartTxBuf[0] = ISP_WRITE_UNPROTECT;
            g_chUartTxBuf[1] = ISP_WRITE_UNPROTECT ^ 0xFF;
        }
    }

    if (COMPATIBLE_ISP_READOUT_PROTECT == pFrameDcb->tCommand)
    {
        if (pFrameDcb->wConfig == ONE_STEP)
        {
            g_chUartTxBuf[0] = ISP_READOUT_PROTECT;
            g_chUartTxBuf[1] = ISP_READOUT_PROTECT ^ 0xFF;
        }
    }

    if (COMPATIBLE_ISP_READOUT_UNPROTECT == pFrameDcb->tCommand)
    {
        if (pFrameDcb->wConfig == ONE_STEP)
        {
            g_chUartTxBuf[0] = ISP_READOUT_UNPROTECT;
            g_chUartTxBuf[1] = ISP_READOUT_UNPROTECT ^ 0xFF;
        }
    }

    //todo
    UART1_Send_Group(g_chUartTxBuf, pFrameDcb->hwFrameLengh); //UART发送一帧数据
    return true;
}

#define COMMAND_RECEIVED_FSM_RESET() \
do {\
    s_tReceived.tState = START;\
} while(0)
#define RECEIVED_DELAY      (0X1FF)
fsm_rt_t Received_SRAM_Command(void)
{
    static struct
    {
        enum
        {
            START = 0,
            RECEIVED,
            DELAY,
            REC_COMPLETE
        } tState;
        uint16_t hwDelay;
        uint16_t hwLastCount;
    } s_tReceived = {START, 0, 0};

    switch (s_tReceived.tState)
    {
    case START:
        s_tReceived.tState = RECEIVED;
        s_tReceived.hwDelay = RECEIVED_DELAY;//初始化delay次数

    //break;
    case RECEIVED:
        if (g_wUartRxCnt)//UART开始接收数据
        {
            s_tReceived.hwLastCount = g_wUartRxCnt;
            s_tReceived.tState = DELAY;
        }

        break;

    case DELAY:
        if (-- s_tReceived.hwDelay < 1)//延时等待接收完成
        {
            s_tReceived.tState = REC_COMPLETE;
        }

        break;

    case REC_COMPLETE:
        if ((g_wUartRxCnt >= UART_REC_MIN_LEN) && (s_tReceived.hwLastCount == g_wUartRxCnt))//接收完成(延时结束后UART接收计数未变化)
        {
            COMMAND_RECEIVED_FSM_RESET();
            s_tReceived.hwLastCount = 0;
            g_wUartRxCnt = 0;
            return fsm_rt_cpl;//状态机已完成
        }
        else
        {
            s_tReceived.hwLastCount = g_wUartRxCnt;//接收未完成(延时结束后UART接收计数发生变化)
            s_tReceived.hwDelay = RECEIVED_DELAY;//重新进入延时等待
            s_tReceived.tState = DELAY;
        }

        break;
    }

    return fsm_rt_on_going;//状态机运行中
}

fsm_rt_t Received_Flash_Command(void)
{
    static struct
    {
        enum
        {
            START = 0,
            RECEIVED,
            DELAY,
            REC_COMPLETE
        } tState;
        uint16_t hwDelay;
        uint16_t hwLastCount;
    } s_tReceived = {START, 0, 0};

    switch (s_tReceived.tState)
    {
    case START:
        s_tReceived.tState = RECEIVED;
        s_tReceived.hwDelay = RECEIVED_DELAY;//初始化delay次数

    //break;
    case RECEIVED:
        if (g_wUartRxCnt)//UART开始接收数据
        {
            s_tReceived.hwLastCount = g_wUartRxCnt;
            s_tReceived.tState = DELAY;
        }

        break;

    case DELAY:
        if (-- s_tReceived.hwDelay < 1)//延时等待接收完成
        {
            s_tReceived.tState = REC_COMPLETE;
        }

        break;

    case REC_COMPLETE:
        if ((g_wUartRxCnt >= 1) && (s_tReceived.hwLastCount == g_wUartRxCnt))//接收完成(延时结束后UART接收计数未变化)
        {
            COMMAND_RECEIVED_FSM_RESET();
            s_tReceived.hwLastCount = 0;
            g_wUartRxCnt = 0;
            return fsm_rt_cpl;//状态机已完成
        }
        else
        {
            s_tReceived.hwLastCount = g_wUartRxCnt;//接收未完成(延时结束后UART接收计数发生变化)
            s_tReceived.hwDelay = RECEIVED_DELAY;//重新进入延时等待
            s_tReceived.tState = DELAY;
        }

        break;
    }

    return fsm_rt_on_going;//状态机运行中
}

//握手协议函数
#define HANDSHAKE_FSM_RESET() \
do {\
    s_tState = START;\
} while(0)
#define HANDSHAKE_PROTOCOL_LENGTH   5
fsm_rt_t Handshake(FrameDcb_t *pFrameDcb)
{
    static enum
    {
        START = 0,
        SEND,
        RECEIVED,
        CHRECK
    } s_tState;
    uint16_t hwFrameLength;

    switch (s_tState)
    {
    case START:
        pFrameDcb->hwFrameLengh = HANDSHAKE_PROTOCOL_LENGTH;
        pFrameDcb->tCommand = HANDSHAKE_PROTOCOL;
        s_tState = SEND;

    //break;
    case SEND:
        if (Send_SRAM_Frame(pFrameDcb))
        {
            s_tState = RECEIVED;
        }

        break;

    case RECEIVED:
        if (fsm_rt_cpl == Received_SRAM_Command())
        {
            s_tState = CHRECK;
        }

        break;

    case CHRECK:
        if (MCU_TO_PC == g_chUartRxBuf[0])
        {
            if (HANDSHAKE_PROTOCOL == g_chUartRxBuf[3])
            {
                hwFrameLength = (uint16_t)(g_chUartRxBuf[1] << 8) | g_chUartRxBuf[2];

                if (SRAM_check_sum(g_chUartRxBuf, hwFrameLength - 1) == g_chUartRxBuf[hwFrameLength - 1])
                {
                    HANDSHAKE_FSM_RESET();
                    return  fsm_rt_cpl;
                }
            }
        }

        HANDSHAKE_FSM_RESET();
        break;
    }

    return fsm_rt_on_going;
}

//载入配置,加载SRAM程序
#define LOADRAMCODE_FSM_RESET() \
do {\
    s_tState = START;\
} while(0)
#define RECEIVE_CONTINUE    (0x01)
#define RECEIVE_COMPLETE    (0x02)
fsm_rt_t LoadRamCode(FrameDcb_t *pFrameDcb)
{
    static enum
    {
        START = 0,
        SEND_START,
        RECEIVED,
        CHRECK,
        SEND_DATA
    } s_tState;
    uint16_t hwFrameLength;

    switch (s_tState)
    {
    case START:                 //配置
        pFrameDcb->hwFrameLengh = LOAD_CONFIG_LENGTH;
        pFrameDcb->tCommand = LOAD_CONFIG;
        pFrameDcb->wConfig = START_CONFIG;
        pFrameDcb->wAddress = RAM_CODE_START_ADDR;
        pFrameDcb->wDataLength = RAM_CODE_LENGTH;
        pFrameDcb->pData = RAM_CODE;
        s_tState = SEND_START;

    //break;
    case SEND_START:
        if (Send_SRAM_Frame(pFrameDcb))//PC发送配置信息
        {
            pFrameDcb->hwFrameLengh = UART_SEND_MAX_LEN;
            s_tState = RECEIVED;
        }

        break;

    case RECEIVED:
        if (fsm_rt_cpl == Received_SRAM_Command())//接收到一帧数据
        {
            s_tState = CHRECK;
        }

        break;

    case CHRECK:
        if (MCU_TO_PC == g_chUartRxBuf[0])//数据包为目标MCU发送
        {
            if (LOAD_CONFIG == g_chUartRxBuf[3])//数据帧为载入配置
            {
                hwFrameLength = (uint16_t)(g_chUartRxBuf[1] << 8) | g_chUartRxBuf[2];

                if (SRAM_check_sum(g_chUartRxBuf, hwFrameLength - 1) == g_chUartRxBuf[hwFrameLength - 1])
                {
                    if (RECEIVE_CONTINUE == g_chUartRxBuf[4])//SRAM程序发送未完成
                    {
                        if (SEND_CONFIG == pFrameDcb->wConfig)//发数据配置
                        {
                            pFrameDcb->pData += LOAD_CONFIG_DATA_LENGTH;
                            pFrameDcb->wAddress += LOAD_CONFIG_DATA_LENGTH;
                        }

                        s_tState = SEND_DATA;
                        break;
                    }
                    else if (RECEIVE_COMPLETE == g_chUartRxBuf[4])//SRAM程序发送完成
                    {
                        LOADRAMCODE_FSM_RESET();
                        return fsm_rt_cpl;
                    }
                    else
                    {
                        LOADRAMCODE_FSM_RESET();
                        return fsm_rt_err;
                    }

                    //todo
                }
            }
        }

        LOADRAMCODE_FSM_RESET();
        break;

    case SEND_DATA:
        if ((pFrameDcb->wAddress + LOAD_CONFIG_DATA_LENGTH) >= (RAM_CODE_START_ADDR + pFrameDcb->wDataLength))
        {
            pFrameDcb->wConfig = CONFIG_COMPLETE;
        }
        else
        {
            pFrameDcb->wConfig = SEND_CONFIG;
        }

        if (Send_SRAM_Frame(pFrameDcb))//PC发送配置信息
        {
            s_tState = RECEIVED;
        }

        //todo
        break;
    }

    return fsm_rt_on_going;
}

//同步波特率函数
#define SYNCBAUD_FSM_RESET() \
do {\
    s_tState = START;\
} while(0)
#define COMPATIBLE_SYNCBAUD_PROTOCOL_LENGTH   1
fsm_rt_t SyncBaud(FrameDcb_t *pFrameDcb)
{
    static enum
    {
        START = 0,
        SEND,
        RECEIVED,
        CHRECK
    } s_tState;

    switch (s_tState)
    {
    case START:
        pFrameDcb->hwFrameLengh = COMPATIBLE_SYNCBAUD_PROTOCOL_LENGTH;
        pFrameDcb->tCommand = COMPATIBLE_SYNCBAUD_PROTOCOL;
        s_tState = SEND;

    //break;
    case SEND:
        if (Send_Flash_Frame(pFrameDcb))
        {
            s_tState = RECEIVED;
        }

        break;

    case RECEIVED:
        if (fsm_rt_cpl == Received_Flash_Command())
        {
            s_tState = CHRECK;
        }

        break;

    case CHRECK:
        if ((g_chUartRxBuf[0] == ISP_ACK) || (g_chUartRxBuf[0] == ISP_NACK))
        {
            SYNCBAUD_FSM_RESET();
            return  fsm_rt_cpl;
        }

        SYNCBAUD_FSM_RESET();
        break;
    }

    return fsm_rt_on_going;
}

//GET函数
#define ISP_GET_FSM_RESET() \
do {\
    s_tState = START;\
} while(0)
#define COMPATIBLE_ISP_GET_LENGTH   2
fsm_rt_t IspGet(FrameDcb_t *pFrameDcb)
{
    static enum
    {
        START = 0,
        SEND,
        RECEIVED,
        CHRECK
    } s_tState;

    switch (s_tState)
    {
    case START:
        pFrameDcb->hwFrameLengh = COMPATIBLE_ISP_GET_LENGTH;
        pFrameDcb->tCommand = COMPATIBLE_ISP_GET;
        s_tState = SEND;

    //break;
    case SEND:
        if (Send_Flash_Frame(pFrameDcb))
        {
            s_tState = RECEIVED;
        }

        break;

    case RECEIVED:
        if (fsm_rt_cpl == Received_Flash_Command())
        {
            s_tState = CHRECK;
        }

        break;

    case CHRECK:
        if ((g_chUartRxBuf[0] == ISP_ACK) && (g_chUartRxBuf[14] == ISP_ACK) && (g_chUartRxBuf[1] == (15 - 3 - 1)))
        {
            memcpy(pFrameDcb->framebuf, g_chUartRxBuf + 3, g_chUartRxBuf[1]);
            ISP_GET_FSM_RESET();
            return  fsm_rt_cpl;
        }

        ISP_GET_FSM_RESET();
        break;
    }

    return fsm_rt_on_going;
}

//GET_VERSION函数
#define ISP_GET_VERSION_FSM_RESET() \
do {\
    s_tState = START;\
} while(0)
#define COMPATIBLE_ISP_GET_VERSION_LENGTH   2
fsm_rt_t IspGetVersion(FrameDcb_t *pFrameDcb)
{
    static enum
    {
        START = 0,
        SEND,
        RECEIVED,
        CHRECK
    } s_tState;

    switch (s_tState)
    {
    case START:
        pFrameDcb->hwFrameLengh = COMPATIBLE_ISP_GET_VERSION_LENGTH;
        pFrameDcb->tCommand = COMPATIBLE_ISP_GET_VERSION;
        s_tState = SEND;

    //break;
    case SEND:
        if (Send_Flash_Frame(pFrameDcb))
        {
            s_tState = RECEIVED;
        }

        break;

    case RECEIVED:
        if (fsm_rt_cpl == Received_Flash_Command())
        {
            s_tState = CHRECK;
        }

        break;

    case CHRECK:
        if ((ISP_ACK == g_chUartRxBuf[0]) && (ISP_ACK == g_chUartRxBuf[4]))
        {
            memcpy(pFrameDcb->framebuf, g_chUartRxBuf + 1, 3);
            ISP_GET_VERSION_FSM_RESET();
            return  fsm_rt_cpl;
        }

        ISP_GET_VERSION_FSM_RESET();
        break;
    }

    return fsm_rt_on_going;
}

//GET_ID函数
#define ISP_GET_ID_FSM_RESET() \
do {\
    s_tState = START;\
} while(0)
#define COMPATIBLE_ISP_GET_ID_LENGTH   2
fsm_rt_t IspGetID(FrameDcb_t *pFrameDcb)
{
    static enum
    {
        START = 0,
        SEND,
        RECEIVED,
        CHRECK
    } s_tState;

    switch (s_tState)
    {
    case START:
        pFrameDcb->hwFrameLengh = COMPATIBLE_ISP_GET_ID_LENGTH;
        pFrameDcb->tCommand = COMPATIBLE_ISP_GET_ID;
        s_tState = SEND;

    //break;
    case SEND:
        if (Send_Flash_Frame(pFrameDcb))
        {
            s_tState = RECEIVED;
        }

        break;

    case RECEIVED:
        if (fsm_rt_cpl == Received_Flash_Command())
        {
            s_tState = CHRECK;
        }

        break;

    case CHRECK:
        if ((g_chUartRxBuf[0] == ISP_ACK) && (g_chUartRxBuf[4] == ISP_ACK) && (g_chUartRxBuf[1] == (5 - 3 - 1)))
        {
            memcpy(pFrameDcb->framebuf, g_chUartRxBuf + 2, 2);
            ISP_GET_ID_FSM_RESET();
            return  fsm_rt_cpl;
        }

        ISP_GET_ID_FSM_RESET();
        break;
    }

    return fsm_rt_on_going;
}

//ISP_READ_MEMORY函数
#define ISP_READ_MEMORY_FSM_RESET() \
do {\
    s_tState = START;\
} while(0)
#define COMPATIBLE_ISP_READ_MEMORY_LENGTH   2
fsm_rt_t IspReadMemory(FrameDcb_t *pFrameDcb)
{
    static enum
    {
        START = 0,
        SEND,
        RECEIVED,
        CHRECK
    } s_tState;

    switch (s_tState)
    {
    case START:
        pFrameDcb->hwFrameLengh = COMPATIBLE_ISP_READ_MEMORY_LENGTH;
        pFrameDcb->wConfig = ONE_STEP;
        pFrameDcb->tCommand = COMPATIBLE_ISP_READ_MEMORY;
        s_tState = SEND;

    //break;
    case SEND:
        if (Send_Flash_Frame(pFrameDcb))
        {
            s_tState = RECEIVED;
        }

        break;

    case RECEIVED:
        if (fsm_rt_cpl == Received_Flash_Command())
        {
            s_tState = CHRECK;
        }

        break;

    case CHRECK:
        if (pFrameDcb->wConfig == ONE_STEP)
        {
            if (g_chUartRxBuf[0] == ISP_ACK)
            {
                pFrameDcb->hwFrameLengh = 5;
                pFrameDcb->wConfig = TWO_STEP;
                s_tState = SEND;
                break;
            }
            else
            {
                ISP_READ_MEMORY_FSM_RESET();
                return fsm_rt_err;
            }
        }
        else if (pFrameDcb->wConfig == TWO_STEP)
        {
            if (g_chUartRxBuf[0] == ISP_ACK)
            {
                pFrameDcb->hwFrameLengh = 2;
                pFrameDcb->wConfig = THREE_STEP;
                s_tState = SEND;
                break;
            }
            else
            {
                ISP_READ_MEMORY_FSM_RESET();
                return fsm_rt_err;
            }
        }
        else if (pFrameDcb->wConfig == THREE_STEP)
        {
            if (g_chUartRxBuf[0] == ISP_ACK)
            {
                pFrameDcb->wConfig = 0;
                memcpy(pFrameDcb->framebuf, g_chUartRxBuf + 1, pFrameDcb->wDataLength + 1);
                ISP_READ_MEMORY_FSM_RESET();
                return  fsm_rt_cpl;
            }
            else
            {
                ISP_READ_MEMORY_FSM_RESET();
                return fsm_rt_err;
            }
        }

        ISP_READ_MEMORY_FSM_RESET();
        break;
    }

    return fsm_rt_on_going;
}

//GO函数
#define ISP_GO_FSM_RESET() \
do {\
    s_tState = START;\
} while(0)
#define COMPATIBLE_ISP_GO_LENGTH   2
fsm_rt_t IspGo(FrameDcb_t *pFrameDcb)
{
    static enum
    {
        START = 0,
        SEND,
        RECEIVED,
        CHRECK
    } s_tState;

    switch (s_tState)
    {
    case START:
        pFrameDcb->hwFrameLengh = COMPATIBLE_ISP_GO_LENGTH;
        pFrameDcb->wConfig = ONE_STEP;
        pFrameDcb->tCommand = COMPATIBLE_ISP_GO;
        s_tState = SEND;

    //break;
    case SEND:
        if (Send_Flash_Frame(pFrameDcb))
        {
            s_tState = RECEIVED;
        }

        break;

    case RECEIVED:
        if (fsm_rt_cpl == Received_Flash_Command())
        {
            s_tState = CHRECK;
        }

        break;

    case CHRECK:
        if (pFrameDcb->wConfig == ONE_STEP)
        {
            if (g_chUartRxBuf[0] == ISP_ACK)
            {
                pFrameDcb->hwFrameLengh = 5;
                pFrameDcb->wConfig = TWO_STEP;
                s_tState = SEND;
                break;
            }
            else
            {
                ISP_GO_FSM_RESET();
                return fsm_rt_err;
            }
        }
        else if (pFrameDcb->wConfig == TWO_STEP)
        {
            if (g_chUartRxBuf[0] == ISP_ACK)
            {
                pFrameDcb->wConfig = 0;
                ISP_GO_FSM_RESET();
                return  fsm_rt_cpl;
            }
            else
            {
                ISP_GO_FSM_RESET();
                return fsm_rt_err;
            }
        }

        ISP_GO_FSM_RESET();
        break;
    }

    return fsm_rt_on_going;
}

//ISP_WRITE_MEMORY函数
#define ISP_WRITE_MEMORY_FSM_RESET() \
do {\
    s_tState = START;\
} while(0)
#define COMPATIBLE_ISP_WRITE_MEMORY_LENGTH   2
fsm_rt_t IspWriteMemroy(FrameDcb_t *pFrameDcb)
{
    static enum
    {
        START = 0,
        SEND,
        RECEIVED,
        CHRECK
    } s_tState;

    switch (s_tState)
    {
    case START:
        pFrameDcb->hwFrameLengh = COMPATIBLE_ISP_WRITE_MEMORY_LENGTH;
        pFrameDcb->wConfig = ONE_STEP;
        pFrameDcb->tCommand = COMPATIBLE_ISP_WRITE_MEMORY;
        s_tState = SEND;

    //break;
    case SEND:
        if (Send_Flash_Frame(pFrameDcb))
        {
            s_tState = RECEIVED;
        }

        break;

    case RECEIVED:
        if (fsm_rt_cpl == Received_Flash_Command())
        {
            s_tState = CHRECK;
        }

        break;

    case CHRECK:
        if (pFrameDcb->wConfig == ONE_STEP)
        {
            if (g_chUartRxBuf[0] == ISP_ACK)
            {
                pFrameDcb->hwFrameLengh = 5;
                pFrameDcb->wConfig = TWO_STEP;
                s_tState = SEND;
                break;
            }
            else
            {
                ISP_WRITE_MEMORY_FSM_RESET();
                return fsm_rt_err;
            }
        }
        else if (pFrameDcb->wConfig == TWO_STEP)
        {
            if (g_chUartRxBuf[0] == ISP_ACK)
            {
                pFrameDcb->hwFrameLengh = pFrameDcb->wDataLength + 3;
                pFrameDcb->wConfig = THREE_STEP;
                s_tState = SEND;
                break;
            }
            else
            {
                ISP_WRITE_MEMORY_FSM_RESET();
                return fsm_rt_err;
            }
        }
        else if (pFrameDcb->wConfig == THREE_STEP)
        {
            if (g_chUartRxBuf[0] == ISP_ACK)
            {
                pFrameDcb->wConfig = 0;
                ISP_WRITE_MEMORY_FSM_RESET();
                return  fsm_rt_cpl;
            }
            else
            {
                ISP_WRITE_MEMORY_FSM_RESET();
                return fsm_rt_err;
            }
        }

        ISP_WRITE_MEMORY_FSM_RESET();
        break;
    }

    return fsm_rt_on_going;
}

//ISP_ERASE_MEMORY函数
#define ISP_ERASE_MEMORY_FSM_RESET() \
do {\
    s_tState = START;\
} while(0)
#define COMPATIBLE_ISP_ERASE_MEMORY_LENGTH   2
fsm_rt_t IspEraseMempry(FrameDcb_t *pFrameDcb)
{
    static enum
    {
        START = 0,
        SEND,
        RECEIVED,
        CHRECK
    } s_tState;

    switch (s_tState)
    {
    case START:
        pFrameDcb->hwFrameLengh = COMPATIBLE_ISP_ERASE_MEMORY_LENGTH;
        pFrameDcb->wConfig = ONE_STEP;
        pFrameDcb->tCommand = COMPATIBLE_ISP_ERASE_MEMORY;
        s_tState = SEND;

    //break;
    case SEND:
        if (Send_Flash_Frame(pFrameDcb))
        {
            s_tState = RECEIVED;
        }

        break;

    case RECEIVED:
        if (fsm_rt_cpl == Received_Flash_Command())
        {
            s_tState = CHRECK;
        }

        break;

    case CHRECK:
        if (pFrameDcb->wConfig == ONE_STEP)
        {
            if (g_chUartRxBuf[0] == ISP_ACK)
            {
                pFrameDcb->wConfig = TWO_STEP;
                s_tState = SEND;
                break;
            }
            else
            {
                ISP_ERASE_MEMORY_FSM_RESET();
                return fsm_rt_err;
            }
        }
        else if (pFrameDcb->wConfig == TWO_STEP)
        {
            if (g_chUartRxBuf[0] == ISP_ACK)
            {
                pFrameDcb->wConfig = 0;
                ISP_ERASE_MEMORY_FSM_RESET();
                return  fsm_rt_cpl;
            }
            else
            {
                ISP_ERASE_MEMORY_FSM_RESET();
                return fsm_rt_err;
            }
        }

        ISP_ERASE_MEMORY_FSM_RESET();
        break;
    }

    return fsm_rt_on_going;
}

//ISP_ERASE_MEMORY_EX函数
#define ISP_ERASE_MEMORY_EX_FSM_RESET() \
do {\
    s_tState = START;\
} while(0)
#define COMPATIBLE_ISP_ERASE_MEMORY_EX   2
fsm_rt_t IspEraseMempryEX(FrameDcb_t *pFrameDcb)
{
    return fsm_rt_on_going;
}

//ISP_WRITE_PROTECT函数
#define ISP_WRITE_PROTECT_FSM_RESET() \
do {\
    s_tState = START;\
} while(0)
#define COMPATIBLE_ISP_WRITE_PROTECT_LENGTH   2
fsm_rt_t IspWriteProtect(FrameDcb_t *pFrameDcb)
{
    static enum
    {
        START = 0,
        SEND,
        RECEIVED,
        CHRECK
    } s_tState;

    switch (s_tState)
    {
    case START:
        pFrameDcb->hwFrameLengh = COMPATIBLE_ISP_WRITE_PROTECT_LENGTH;
        pFrameDcb->wConfig = ONE_STEP;
        pFrameDcb->tCommand = COMPATIBLE_ISP_WRITE_PROTECT;
        s_tState = SEND;

    //break;
    case SEND:
        if (Send_Flash_Frame(pFrameDcb))
        {
            s_tState = RECEIVED;
        }

        break;

    case RECEIVED:
        if (fsm_rt_cpl == Received_Flash_Command())
        {
            s_tState = CHRECK;
        }

        break;

    case CHRECK:
        if (pFrameDcb->wConfig == ONE_STEP)
        {
            if (g_chUartRxBuf[0] == ISP_ACK)
            {
                pFrameDcb->hwFrameLengh = 3;
                pFrameDcb->wConfig = TWO_STEP;
                s_tState = SEND;
                break;
            }
            else
            {
                ISP_WRITE_PROTECT_FSM_RESET();
                return fsm_rt_err;
            }
        }
        else if (pFrameDcb->wConfig == TWO_STEP)
        {
            if (g_chUartRxBuf[0] == ISP_ACK)
            {
                pFrameDcb->wConfig = 0;
                ISP_WRITE_PROTECT_FSM_RESET();
                return  fsm_rt_cpl;
            }
            else
            {
                ISP_WRITE_PROTECT_FSM_RESET();
                return fsm_rt_err;
            }
        }

        ISP_WRITE_PROTECT_FSM_RESET();
        break;
    }

    return fsm_rt_on_going;
}

//ISP_WRITE_UNPROTECT函数
#define ISP_WRITE_UNPROTECT_FSM_RESET() \
do {\
    s_tState = START;\
} while(0)
#define COMPATIBLE_ISP_WRITE_UNPROTECT_LENGTH   2
fsm_rt_t IspWriteUNProtect(FrameDcb_t *pFrameDcb)
{
    static enum
    {
        START = 0,
        SEND,
        RECEIVED,
        CHRECK
    } s_tState;

    switch (s_tState)
    {
    case START:
        pFrameDcb->hwFrameLengh = COMPATIBLE_ISP_WRITE_UNPROTECT_LENGTH;
        pFrameDcb->tCommand = COMPATIBLE_ISP_WRITE_UNPROTECT;
        s_tState = SEND;

    //break;
    case SEND:
        if (Send_Flash_Frame(pFrameDcb))
        {
            s_tState = RECEIVED;
        }

        break;

    case RECEIVED:
        if (fsm_rt_cpl == Received_Flash_Command())
        {
            s_tState = CHRECK;
        }

        break;

    case CHRECK:
        if (pFrameDcb->wConfig == ONE_STEP)
        {
            if (g_chUartRxBuf[0] == ISP_ACK)
            {
                pFrameDcb->hwFrameLengh = 3;
                pFrameDcb->wConfig = TWO_STEP;
                s_tState = SEND;
                break;
            }
            else
            {
                ISP_WRITE_UNPROTECT_FSM_RESET();
                return fsm_rt_err;
            }
        }
        else if (pFrameDcb->wConfig == TWO_STEP)
        {
            if (g_chUartRxBuf[0] == ISP_ACK)
            {
                pFrameDcb->wConfig = 0;
                ISP_WRITE_UNPROTECT_FSM_RESET();
                return  fsm_rt_cpl;
            }
            else
            {
                ISP_WRITE_UNPROTECT_FSM_RESET();
                return fsm_rt_err;
            }
        }

        ISP_WRITE_UNPROTECT_FSM_RESET();
        break;
    }

    return fsm_rt_on_going;
}

//ISP_READOUT_PROTECT函数
#define ISP_READOUT_PROTECT_FSM_RESET() \
do {\
    s_tState = START;\
} while(0)
#define COMPATIBLE_ISP_READOUT_PROTECT_LENGTH   2
fsm_rt_t IspReadProtect(FrameDcb_t *pFrameDcb)
{
    static enum
    {
        START = 0,
        SEND,
        RECEIVED,
        CHRECK
    } s_tState;

    switch (s_tState)
    {
    case START:
        pFrameDcb->hwFrameLengh = COMPATIBLE_ISP_READOUT_PROTECT_LENGTH;
        pFrameDcb->tCommand = COMPATIBLE_ISP_READOUT_PROTECT;
        s_tState = SEND;

    //break;
    case SEND:
        if (Send_Flash_Frame(pFrameDcb))
        {
            s_tState = RECEIVED;
        }

        break;

    case RECEIVED:
        if (fsm_rt_cpl == Received_Flash_Command())
        {
            s_tState = CHRECK;
        }

        break;

    case CHRECK:
        if (pFrameDcb->wConfig == ONE_STEP)
        {
            if (g_chUartRxBuf[0] == ISP_ACK)
            {
                pFrameDcb->hwFrameLengh = 3;
                pFrameDcb->wConfig = TWO_STEP;
                s_tState = SEND;
                break;
            }
            else
            {
                ISP_WRITE_UNPROTECT_FSM_RESET();
                return fsm_rt_err;
            }
        }
        else if (pFrameDcb->wConfig == TWO_STEP)
        {
            if (g_chUartRxBuf[0] == ISP_ACK)
            {
                pFrameDcb->wConfig = 0;
                ISP_WRITE_UNPROTECT_FSM_RESET();
                return  fsm_rt_cpl;
            }
            else
            {
                ISP_WRITE_UNPROTECT_FSM_RESET();
                return fsm_rt_err;
            }
        }

        ISP_WRITE_UNPROTECT_FSM_RESET();
        break;
    }

    return fsm_rt_on_going;
}

//ISP_READOUT_UNPROTECT函数
#define ISP_READOUT_UNPROTECT_FSM_RESET() \
do {\
    s_tState = START;\
} while(0)
#define COMPATIBLE_ISP_READOUT_UNPROTECT_LENGTH   2
fsm_rt_t IspReadUNProtect(FrameDcb_t *pFrameDcb)
{
    static enum
    {
        START = 0,
        SEND,
        RECEIVED,
        CHRECK
    } s_tState;

    //uint16_t hwFrameLength;
    switch (s_tState)
    {
    case START:
        pFrameDcb->hwFrameLengh = COMPATIBLE_ISP_READOUT_UNPROTECT_LENGTH;
        pFrameDcb->tCommand = COMPATIBLE_ISP_READOUT_UNPROTECT;
        s_tState = SEND;

    //break;
    case SEND:
        if (Send_Flash_Frame(pFrameDcb))
        {
            s_tState = RECEIVED;
        }

        break;

    case RECEIVED:
        if (fsm_rt_cpl == Received_Flash_Command())
        {
            s_tState = CHRECK;
        }

        break;

    case CHRECK:
        if (pFrameDcb->wConfig == ONE_STEP)
        {
            if (g_chUartRxBuf[0] == ISP_ACK)
            {
                pFrameDcb->hwFrameLengh = 3;
                pFrameDcb->wConfig = TWO_STEP;
                s_tState = SEND;
                break;
            }
            else
            {
                ISP_WRITE_UNPROTECT_FSM_RESET();
                return fsm_rt_err;
            }
        }
        else if (pFrameDcb->wConfig == TWO_STEP)
        {
            if (g_chUartRxBuf[0] == ISP_ACK)
            {
                pFrameDcb->wConfig = 0;
                ISP_WRITE_UNPROTECT_FSM_RESET();
                return  fsm_rt_cpl;
            }
            else
            {
                ISP_WRITE_UNPROTECT_FSM_RESET();
                return fsm_rt_err;
            }
        }

        ISP_WRITE_UNPROTECT_FSM_RESET();
        break;
    }

    return fsm_rt_on_going;
}







