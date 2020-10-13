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
    uint8_t Cmm_ID[11];      /* ֧�ֵ����� */
    char Device_ID_Code[16];
    uint16_t PID;            /* PID */
    uint8_t BootloaderVer;   /* ISP�汾 */
    uint8_t OptionBytes[16]; /* ѡ���ֽ� */
} MCU_inof_t;

MCU_inof_t mm32_inof;

uint8_t PROGRAM_CODE[256] = {0,};

FrameDcb_t tComFrame =
{
    .tDirection = PC_TO_MCU,
};

bool mRDPReadOut = false; /* ������,Ĭ�Ϲر� */
bool mRDPReadOutUn = false; /* ȥ��������ʹ��,Ĭ�Ϲر� */

bool key_status = false; //Ĭ��״̬

void mm32isp_process(void)
{
    uint32_t i = 0;

    /* Ĭ�����ô���Ϊ9600 ����żУ�� */
    UART1_NVIC_Init(9600, UART_Parity_No);
    /* ���� */
    while (fsm_rt_cpl != Handshake(&tComFrame));
    /* ����SRAM Loader���� */
    while (fsm_rt_cpl != LoadRamCode(&tComFrame));
    /* �������ô��ڲ�����Ϊ115200 żУ��*/
    UART1_NVIC_Init(115200, UART_Parity_Even);
    /* ����0x7F ����Ӧ������ */
    UART1_Send_Byte(ISP_START_FRAME);
    UART1_Send_Byte(ISP_START_FRAME);
    UART1_Send_Byte(ISP_START_FRAME);
    UART1_Send_Byte(ISP_START_FRAME);
    while (fsm_rt_cpl != SyncBaud(&tComFrame));
    /* ��ȡ֧�ֵ����� */
    while (fsm_rt_cpl != IspGet(&tComFrame));
    memcpy(mm32_inof.Cmm_ID, tComFrame.framebuf, sizeof(mm32_inof.Cmm_ID));

    if (mRDPReadOutUn == true) /* ���������ʹ��,�Ȳ���������,Ȼ����������SRAM Loader���� */
    {
        /* ��������� */
        while (fsm_rt_cpl != IspReadUNProtect(&tComFrame));

        /* Ĭ�����ô���Ϊ9600 ����żУ�� */
        UART1_NVIC_Init(9600, UART_Parity_No);
        /* ���� */
        while (fsm_rt_cpl != Handshake(&tComFrame));
        /* ����SRAM Loader���� */
        while (fsm_rt_cpl != LoadRamCode(&tComFrame));
        /* �������ô��ڲ�����Ϊ115200 żУ��*/
        UART1_NVIC_Init(115200, UART_Parity_Even);
        /* ����0x7F ����Ӧ������ */
        UART1_Send_Byte(ISP_START_FRAME);
        UART1_Send_Byte(ISP_START_FRAME);
        UART1_Send_Byte(ISP_START_FRAME);
        UART1_Send_Byte(ISP_START_FRAME);
        while (fsm_rt_cpl != SyncBaud(&tComFrame));
        /* ��ȡ֧�ֵ����� */
        while (fsm_rt_cpl != IspGet(&tComFrame));
        memcpy(mm32_inof.Cmm_ID, tComFrame.framebuf, sizeof(mm32_inof.Cmm_ID));
    }

    /* ��ȡ�汾 */
    while (fsm_rt_cpl != IspGetVersion(&tComFrame));
    memcpy(&mm32_inof.BootloaderVer, tComFrame.framebuf, 1);
    /* ��ȡID */
    while (fsm_rt_cpl != IspGetID(&tComFrame));
    mm32_inof.PID = (tComFrame.framebuf[0] << 8) | tComFrame.framebuf[1];
    /* ��UID */
    tComFrame.wAddress = 0x1FFFF7E8;
    tComFrame.wDataLength = 0xFF;
    while (fsm_rt_cpl != IspReadMemory(&tComFrame));
    memcpy(mm32_inof.Only96bitID, tComFrame.framebuf, sizeof(mm32_inof.Only96bitID));
    /* ��ѡ���ֽ� */
    tComFrame.wAddress = 0x1FFFF800;
    tComFrame.wDataLength = 0xFF;
    while (fsm_rt_cpl != IspReadMemory(&tComFrame));
    memcpy(mm32_inof.OptionBytes, tComFrame.framebuf, sizeof(mm32_inof.OptionBytes));
    /* ����flash */
    while (fsm_rt_cpl != IspEraseMempry(&tComFrame));

    /* ��flash��ַFLASH_CODE_START_ADDRд��̼� */
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

    /* ���д����ʹ�ܽ���д����,������ת���� */
    if (mRDPReadOutUn == true)
    {
        /* д����ʹ�� */
        while (fsm_rt_cpl != IspWriteProtect(&tComFrame));
    }
    else
    {
        /* ��ת��ַ */
        tComFrame.wAddress = FLASH_CODE_START_ADDR;
        while (fsm_rt_cpl != IspGo(&tComFrame));
    }
}

//EXAMPLE
//int main(void)
//{
//    DELAY_Init();
//    LED_Init();
//    /* Ĭ�����ô���Ϊ9600 ����żУ�� */
//    UART1_NVIC_Init(9600, UART_Parity_No);
//    /* ���� */
//    while (fsm_rt_cpl != Handshake(&tComFrame));
//    /* ����SRAM Loader���� */
//    while (fsm_rt_cpl != LoadRamCode(&tComFrame));
//    /* �������ô��ڲ�����Ϊ115200 żУ��*/
//    UART1_NVIC_Init(115200, UART_Parity_Even);
//    /* ����0x7F ����Ӧ������ */
//    UART1_Send_Byte(ISP_START_FRAME);
//    UART1_Send_Byte(ISP_START_FRAME);
//    UART1_Send_Byte(ISP_START_FRAME);
//    UART1_Send_Byte(ISP_START_FRAME);
//    while (fsm_rt_cpl != SyncBaud(&tComFrame));
//    /* ��ȡ֧�ֵ����� */
//    while (fsm_rt_cpl != IspGet(&tComFrame));
//    memcpy(mm32_inof.Cmm_ID, tComFrame.framebuf, sizeof(mm32_inof.Cmm_ID));
//    /* ��ȡ�汾 */
//    while (fsm_rt_cpl != IspGetVersion(&tComFrame));
//    memcpy(&mm32_inof.BootloaderVer, tComFrame.framebuf, 1);
//    /* ��ȡID */
//    while (fsm_rt_cpl != IspGetID(&tComFrame));
//    mm32_inof.PID = (tComFrame.framebuf[0] << 8) | tComFrame.framebuf[1];
//    /* ����flash */
//    while (fsm_rt_cpl != IspEraseMempry(&tComFrame));
//    /* ��flash��ַ0x08000000д��256�ֽ� */
//    tComFrame.wAddress = 0x08000000;
//    tComFrame.pData = FLASH_CODE;
//    tComFrame.wDataLength = 0xFF;
//    while (fsm_rt_cpl != IspWriteMemroy(&tComFrame));
//    /* ��flash��ַ0x08000000��ȡ256�ֽ� */
//    tComFrame.wAddress = 0x08000000;
//    tComFrame.wDataLength = 0xFF;
//    while (fsm_rt_cpl != IspReadMemory(&tComFrame));
//    memcpy(PROGRAM_CODE, tComFrame.framebuf, tComFrame.wDataLength + 1);
//    /* д����ʹ�� */
//    while (fsm_rt_cpl != IspWriteProtect(&tComFrame));
//    /* д������� */
//    while (fsm_rt_cpl != IspWriteUNProtect(&tComFrame));
//    /* ������ʹ�� */
//    while (fsm_rt_cpl != IspReadProtect(&tComFrame));
//    /* ��������� */
//    while (fsm_rt_cpl != IspReadUNProtect(&tComFrame));
//    /* ��ת��ַ */
//    tComFrame.wAddress = FLASH_CODE_START_ADDR;
//    while (fsm_rt_cpl != IspGo(&tComFrame));

//    while (1)
//    {
//    }
//}

/**
 * @brief ��������ѡ�����õ���¼�ͺţ�������ISP.c�д򿪶�Ӧ�ĺ�
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





