////////////////////////////////////////////////////////////////////////////////
/// @file    main.c
/// @author  AE TEAM
/// @brief   THIS FILE PROVIDES ALL THE SYSTEM FUNCTIONS.
////////////////////////////////////////////////////////////////////////////////
/// @attention
///
/// THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
/// CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
/// TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
/// CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
/// HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
/// CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.
///
/// <H2><CENTER>&COPY; COPYRIGHT MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////

// Define to prevent recursive inclusion
#define _MAIN_C_

// Files includes

#include "main.h"
#include "delay.h"
#include "led.h"
#include "exti.h"
#include "uart.h"
#include "module.h"
#include "isp.h"
#include "target_flash_code.h"

typedef struct MCU_inof
{
    bool Completion;
    uint16_t FlashSize;
    uint8_t Only96bitID[12]; /* UID */
    uint8_t Cmm_ID[11];      /* 支持的命令 */
    char Device_ID_Code[16];
    uint16_t PID;            /* PID */
    uint8_t BootloaderVer;   /* ISP版本 */
    uint8_t OptionBytes[16]; /* 选型字节 */
} MCU_inof_t;

MCU_inof_t mm32_inof;

uint8_t PROGRAM_CODE[256] = {0,};

FrameDcb_t tComFrame =
{
    .tDirection = PC_TO_MCU,
};

bool mRDPReadOut = false; /* 读保护,默认关闭 */
bool mRDPReadOutUn = false; /* 去除读保护使能,默认关闭 */

bool key_status = false; //默认状态

void mm32isp_process(void)
{
    uint32_t i = 0;

    /* 默认配置串口为9600 无奇偶校验 */
    UART1_NVIC_Init(9600, UART_Parity_No);
    /* 握手 */
    while (fsm_rt_cpl != Handshake(&tComFrame));
    /* 载入SRAM Loader程序 */
    while (fsm_rt_cpl != LoadRamCode(&tComFrame));
    /* 重新配置串口波特率为115200 偶校验*/
    UART1_NVIC_Init(115200, UART_Parity_Even);
    /* 发送0x7F 自适应波特率 */
    UART1_Send_Byte(ISP_START_FRAME);
    UART1_Send_Byte(ISP_START_FRAME);
    UART1_Send_Byte(ISP_START_FRAME);
    UART1_Send_Byte(ISP_START_FRAME);
    while (fsm_rt_cpl != SyncBaud(&tComFrame));
    /* 读取支持的命令 */
    while (fsm_rt_cpl != IspGet(&tComFrame));
    memcpy(mm32_inof.Cmm_ID, tComFrame.framebuf, sizeof(mm32_inof.Cmm_ID));

    if (mRDPReadOutUn == true) /* 如果读保护使能,先擦除读保护,然后重新下载SRAM Loader程序 */
    {
        /* 读保护解除 */
        while (fsm_rt_cpl != IspReadUNProtect(&tComFrame));

        /* 默认配置串口为9600 无奇偶校验 */
        UART1_NVIC_Init(9600, UART_Parity_No);
        /* 握手 */
        while (fsm_rt_cpl != Handshake(&tComFrame));
        /* 载入SRAM Loader程序 */
        while (fsm_rt_cpl != LoadRamCode(&tComFrame));
        /* 重新配置串口波特率为115200 偶校验*/
        UART1_NVIC_Init(115200, UART_Parity_Even);
        /* 发送0x7F 自适应波特率 */
        UART1_Send_Byte(ISP_START_FRAME);
        UART1_Send_Byte(ISP_START_FRAME);
        UART1_Send_Byte(ISP_START_FRAME);
        UART1_Send_Byte(ISP_START_FRAME);
        while (fsm_rt_cpl != SyncBaud(&tComFrame));
        /* 读取支持的命令 */
        while (fsm_rt_cpl != IspGet(&tComFrame));
        memcpy(mm32_inof.Cmm_ID, tComFrame.framebuf, sizeof(mm32_inof.Cmm_ID));
    }

    /* 读取版本 */
    while (fsm_rt_cpl != IspGetVersion(&tComFrame));
    memcpy(&mm32_inof.BootloaderVer, tComFrame.framebuf, 1);
    /* 读取ID */
    while (fsm_rt_cpl != IspGetID(&tComFrame));
    mm32_inof.PID = (tComFrame.framebuf[0] << 8) | tComFrame.framebuf[1];
    /* 读UID */
    tComFrame.wAddress = 0x1FFFF7E8;
    tComFrame.wDataLength = 0xFF;
    while (fsm_rt_cpl != IspReadMemory(&tComFrame));
    memcpy(mm32_inof.Only96bitID, tComFrame.framebuf, sizeof(mm32_inof.Only96bitID));
    /* 读选项字节 */
    tComFrame.wAddress = 0x1FFFF800;
    tComFrame.wDataLength = 0xFF;
    while (fsm_rt_cpl != IspReadMemory(&tComFrame));
    memcpy(mm32_inof.OptionBytes, tComFrame.framebuf, sizeof(mm32_inof.OptionBytes));
    /* 擦除flash */
    while (fsm_rt_cpl != IspEraseMempry(&tComFrame));

    /* 往flash地址FLASH_CODE_START_ADDR写入固件 */
    for (i = 0; i < FLASH_CODE_LENGTH / 256; i++)
    {
        memset(PROGRAM_CODE, 0xFF, sizeof(PROGRAM_CODE));
        memcpy(PROGRAM_CODE, FLASH_CODE + i * 256, 256);
        tComFrame.wAddress = FLASH_CODE_START_ADDR + i * 256;
        tComFrame.pData = PROGRAM_CODE;
        tComFrame.wDataLength = 0xFF;
        while (fsm_rt_cpl != IspWriteMemroy(&tComFrame));
    }
    memset(PROGRAM_CODE, 0xFF, sizeof(PROGRAM_CODE));
    memcpy(PROGRAM_CODE, FLASH_CODE + i * 256, FLASH_CODE_LENGTH % 256);
    tComFrame.wAddress = FLASH_CODE_START_ADDR + i * 256;
    tComFrame.pData = PROGRAM_CODE;
    tComFrame.wDataLength = 0xFF;
    while (fsm_rt_cpl != IspWriteMemroy(&tComFrame));

    /* 如果写保护使能进行写保护,否则跳转程序 */
    if (mRDPReadOutUn == true)
    {
        /* 写保护使能 */
        while (fsm_rt_cpl != IspWriteProtect(&tComFrame));
    }
    else
    {
        /* 跳转地址 */
        tComFrame.wAddress = FLASH_CODE_START_ADDR;
        while (fsm_rt_cpl != IspGo(&tComFrame));
    }
}

//EXAMPLE
//int main(void)
//{
//    DELAY_Init();
//    LED_Init();
//    /* 默认配置串口为9600 无奇偶校验 */
//    UART1_NVIC_Init(9600, UART_Parity_No);
//    /* 握手 */
//    while (fsm_rt_cpl != Handshake(&tComFrame));
//    /* 载入SRAM Loader程序 */
//    while (fsm_rt_cpl != LoadRamCode(&tComFrame));
//    /* 重新配置串口波特率为115200 偶校验*/
//    UART1_NVIC_Init(115200, UART_Parity_Even);
//    /* 发送0x7F 自适应波特率 */
//    UART1_Send_Byte(ISP_START_FRAME);
//    UART1_Send_Byte(ISP_START_FRAME);
//    UART1_Send_Byte(ISP_START_FRAME);
//    UART1_Send_Byte(ISP_START_FRAME);
//    while (fsm_rt_cpl != SyncBaud(&tComFrame));
//    /* 读取支持的命令 */
//    while (fsm_rt_cpl != IspGet(&tComFrame));
//    memcpy(mm32_inof.Cmm_ID, tComFrame.framebuf, sizeof(mm32_inof.Cmm_ID));
//    /* 读取版本 */
//    while (fsm_rt_cpl != IspGetVersion(&tComFrame));
//    memcpy(&mm32_inof.BootloaderVer, tComFrame.framebuf, 1);
//    /* 读取ID */
//    while (fsm_rt_cpl != IspGetID(&tComFrame));
//    mm32_inof.PID = (tComFrame.framebuf[0] << 8) | tComFrame.framebuf[1];
//    /* 擦除flash */
//    while (fsm_rt_cpl != IspEraseMempry(&tComFrame));
//    /* 往flash地址0x08000000写入256字节 */
//    tComFrame.wAddress = 0x08000000;
//    tComFrame.pData = FLASH_CODE;
//    tComFrame.wDataLength = 0xFF;
//    while (fsm_rt_cpl != IspWriteMemroy(&tComFrame));
//    /* 从flash地址0x08000000读取256字节 */
//    tComFrame.wAddress = 0x08000000;
//    tComFrame.wDataLength = 0xFF;
//    while (fsm_rt_cpl != IspReadMemory(&tComFrame));
//    memcpy(PROGRAM_CODE, tComFrame.framebuf, tComFrame.wDataLength + 1);
//    /* 写保护使能 */
//    while (fsm_rt_cpl != IspWriteProtect(&tComFrame));
//    /* 写保护解除 */
//    while (fsm_rt_cpl != IspWriteUNProtect(&tComFrame));
//    /* 读保护使能 */
//    while (fsm_rt_cpl != IspReadProtect(&tComFrame));
//    /* 读保护解除 */
//    while (fsm_rt_cpl != IspReadUNProtect(&tComFrame));
//    /* 跳转地址 */
//    tComFrame.wAddress = FLASH_CODE_START_ADDR;
//    while (fsm_rt_cpl != IspGo(&tComFrame));

//    while (1)
//    {
//    }
//}

/**
 * @brief 根据需求选择配置的烧录型号，具体在ISP.c中打开对应的宏
 * @return int
 */
int main(void)
{
    DELAY_Init();
    LED_Init();
    KEY_Init();
    EXTI_Config();
    while (1)
    {
        if (key_status == true)
        {
            LED1_OFF();
            mm32isp_process();
            LED1_ON();
            key_status = false;
        }
    }
}





