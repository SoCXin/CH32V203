/******************************************************************************
 * @file    ascii.c
 * @author  MM32 AE
 * @version V1.01
 * @date    12-June-2020
 * @brief   ......
 ******************************************************************************
 *  @attention
 *
 *  THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
 *  CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
 *  TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
 *  CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
 *  HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
 *  CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.
 *  <H2><CENTER>&COPY; COPYRIGHT 2020 MINDMOTION </CENTER></H2>
******************************************************************************/


/* Define to prevent recursive inclusion ------------------------------------*/
#define __ASCII_C__


/* Includes -----------------------------------------------------------------*/
#include    "config.h"
#include    "flash.h"
#include    "stdbool.h"
#include    "uart.h"
#include    "string.h"
#include    <stdio.h>
#include    "main.h"
#include    "m0_interrupt_table_redefine.h"

#if defined (ASCII_EN)
/* Private typedef ----------------------------------------------------------*/

typedef struct
{
    uint32_t wAppAddr;
    uint16_t hwAppLenth;
    uint8_t chKeyVal;
    event_t tReceivedEvent;
    bool bProtectFlag;
} AppControlBlock ;

/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/

#define APP_RECEIVED_FSM_RESET() \
do { \
    s_tReceived.tState = START; \
} while(0)

/* Private variables --------------------------------------------------------*/

static uint32_t wAppSize = 27 * 1024 ;
static AppControlBlock s_tAppControlBlock =
{
    APPLICATION_ADDRESS,      /* APP starting address */
    0,                        /* APP size(in Bytes) */
    0,                        /* Key value */
    AUTO,                     /* Automatic reset */
    RESET,                    /* None event */
    false,                    /* Whether to turn on read protection */
};

static uint32_t    UartRxCnt = 0; /* Data counter received by UART interrupt */
static uint8_t     ProgramFlashDataTemp[BUFFER_SIZE]; /* Program hex data buf */
/*static uint32_t    UserProgramAddr = 0;*/ /* User program execution address */
static uint32_t    HaveGetAddr = 0; /* Get user program address flag */
static uint32_t    HexAddrBase = 0x800; /* hex data start address */

/* Reserve a buffer in RAM address to receive UART data */
#ifndef USE_IAR
static uint8_t UartRxBuf[UART_REC_LEN]   __attribute__((at(0X20000600))) ;
#else
static uint8_t UartRxBuf[UART_REC_LEN]                                        \
    __attribute__((section(".ARM.__at_0x20000600"))) ;
#endif

/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/

/******************************************************************************
 * @brief  The state machine of receiving app
 * @param  None
 * @retval None
 * @attention  None
******************************************************************************/
static fsm_rt_t Received_App(void)
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
        uint8_t chDelay;
        uint16_t hwLastCount;
    } s_tReceived = {START, 0, 0};
    switch (s_tReceived.tState)
    {
    case START:
        s_tReceived.tState = RECEIVED;
        s_tReceived.chDelay = 0XFF;  /* Initialization delay times */
    //break;
    case RECEIVED:
        if (UartRxCnt)         /* UART begins to receive data */
        {
            s_tReceived.hwLastCount = UartRxCnt;
            s_tReceived.tState = DELAY;
        }
        break;
    case DELAY:
        if (-- s_tReceived.chDelay < 1)
        {  /* Delay waiting for reception to complete */
            s_tReceived.tState = REC_COMPLETE;
        }
        break;
    case REC_COMPLETE:
        if (s_tReceived.hwLastCount == UartRxCnt)
        {  /* Receive data finished */
            APP_RECEIVED_FSM_RESET();
            s_tAppControlBlock.hwAppLenth = UartRxCnt;
            s_tReceived.hwLastCount = 0;
            UartRxCnt = 0;
            AppRxSize = s_tAppControlBlock.hwAppLenth ;
            #if DEBUG_EN
            SerialPutString(IAP_UART, "User program reception complete!\r\n");
//                printf("APP Size:%dBytes\r\n", s_tAppControlBlock.hwAppLenth);
            #endif
            return fsm_rt_cpl;
        }
        else
        {
            s_tReceived.hwLastCount = UartRxCnt;    /* Receiving data */
            s_tReceived.chDelay = 0XFF;            /* Reenter delay waiting */
            s_tReceived.tState = DELAY;
        }
        break;
    }
    return fsm_rt_on_going;     /* State machine  is in operation */
}


/******************************************************************************
 * @brief  The actual handler of UART interrupts
 * @param  None
 * @retval None
 * @attention  None
******************************************************************************/
static void IAP_UART_Processing(void)
{
    uint8_t ResByte;
    if ((IAP_UART->ISR & 0x0002) != (uint16_t)RESET) {
        IAP_UART->ICR = 0x0002;
        ResByte = (uint8_t)(IAP_UART->RDR & (uint8_t)0xFF);
        if (UartRxCnt < UART_REC_LEN) {
            UartRxBuf[UartRxCnt] = ResByte;
            UartRxCnt++;
        }
    }
}


/******************************************************************************
 * @brief  Program a line of hex file data in flash
 * @param  pHexData: The start address of a line of hex file to be burned
 * @param  AddrSelect: Address selection method
 * @param  UserAddr: Specified flash address
 * @retval Program result
 * @attention  None
******************************************************************************/
static uint8_t ProgramHexToFlash(uint8_t *pHexData, uint32_t AddrSelect,      \
    volatile uint32_t UserAddr)
{
    volatile uint8_t    ErrorCode = 0;
    t_HexLineInfo   HexDataInfo;
    /* Judge whether all data has been parsed and stored in the cache flag */
    uint8_t CanGetData = TRUE;
    uint32_t    HexDataIndexTemp = 0;
    uint32_t    HexDataTempIndexTemp = 0;
    uint32_t    i = 0;
    uint32_t    DST = 0;
    volatile uint32_t   FirstAddr = 0;
    volatile uint8_t    HaveGetAddrFlag = FALSE;
    volatile uint32_t   Sector = 0;

    for (i = 0; i < BUFFER_SIZE; i++)
    {  /* Initialize the data buffer that needs to be written to flash */
        ProgramFlashDataTemp[i] = 0xFF;
    }
    /* Parse the hex data in the storage block pointed to by phexdata         \
    into the buffer pointed to by programflashdatatemp */
    while (CanGetData == TRUE)
    {
        /* Get hex data information */
        ErrorCode = GetHexLineInfo(pHexData + HexDataIndexTemp, &HexDataInfo);
        if ((HaveGetAddrFlag == FALSE) && (HexDataInfo.Rectype == RCD_DATA))
        {
            FirstAddr = HexDataInfo.OffsetAddr;
            HaveGetAddrFlag = TRUE;
        }
        HexDataIndexTemp += ((HexDataInfo.DataLength) * 2 + 13);
        if (ErrorCode == RIGHT)
        {
            if (HexDataInfo.Rectype == RCD_DATA)
            {
          /* Put the hex data out and store it in the hexdatatemp data buffer */
                ErrorCode = GetHexData(&HexDataInfo, ProgramFlashDataTemp +   \
                    HexDataTempIndexTemp);
                HexDataTempIndexTemp += HexDataInfo.DataLength;
            }
            else if (HexDataInfo.Rectype == RCD_END_FILE)
            {  /* Jump out of loop at end of received file */
                CanGetData = FALSE;
            }
            else if (HexDataInfo.Rectype == RCD_EXT_LINEAR_ADDR)
            {
                //CanGetData=FALSE;
            }
            else if (HexDataInfo.Rectype == RCD_START_LINEAR_ADDR)
            {

            }
            else if (HexDataInfo.Rectype == RCD_EXT_SEGMENT_ADDR)
            {
                //CanGetData=FALSE;
            }
            else if (HexDataInfo.Rectype == RCD_START_SEGMENT_ADDR)
            {

            }
            else
            {
                CanGetData = FALSE;
            }
        }
        else
        {
            CanGetData = FALSE;
        }
        if (pHexData[HexDataIndexTemp] != ':')
        {  /* Judge whether the received data has been processed */
            CanGetData = FALSE;
        }
    }
    /*Write the buffer data to flash*/
    if ((ErrorCode == RIGHT) && (AddrSelect == USE_USER_DEFINED_ADDR))
    {/*The data is correct and the data is written with the specified address*/
        DST = (HexAddrBase << 16) + (FirstAddr + UserAddr);
        ErrorCode = ProgramDatatoFlash(&DST, (uint16_t *)ProgramFlashDataTemp, \
            BUFFER_SIZE);
    }
    else if ((ErrorCode == RIGHT) && (AddrSelect == USE_DFT_ADDR))
    {/*The data is correct and the address in hex file data                   \
        is used to write the data */
        DST = (HexAddrBase << 16) + FirstAddr;
        ErrorCode = ProgramDatatoFlash(&DST, (uint16_t *)ProgramFlashDataTemp, \
            BUFFER_SIZE);
    }
    else
    {
        ErrorCode = 1;
    }
    return ErrorCode;
}

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/
extern void M0_NVIC_Init(uint32_t NVIC_IRQChannelPriority,                     \
    IRQn_Type NVIC_IRQChannel,FunctionalState NVIC_IRQChannelCmd) ;
/* Exported functions --------------------------------------------------------*/

/******************************************************************************
 * @brief  Converts an ASCII character code to a numeric value
 * @param  pAsciiData: Converted ASCII data address
 * @retval Converted value
 * @attention  None
******************************************************************************/
uint8_t AsciiToDec(uint8_t *pAsciiData)
{
    if (pAsciiData != NULL)
    {
        if ((*pAsciiData >= '0') && (*pAsciiData <= '9'))
        {
            return ((*pAsciiData) - '0');
        }
        else if ((*pAsciiData >= 'A') && (*pAsciiData <= 'F'))
        {
            return ((*pAsciiData) - 'A' + 10);
        }
        else if ((*pAsciiData >= 'a') && (*pAsciiData <= 'f'))
        {
            return ((*pAsciiData) - 'a' + 10);
        }
        else
        {
            return (*pAsciiData);
        }
    }
    else
    {
        return ERROR;
    }
}

/******************************************************************************
 * @brief  Get the information of a row of data in hex file
 * @param  pHexData: Converted ASCII data address
           pHexDataInfo: A row of hex data information
 * @retval The result of converting
 * @attention  None
******************************************************************************/
uint8_t GetHexLineInfo(uint8_t *pHexData, t_HexLineInfo *pHexDataInfo)
{
    uint8_t i = 0;
    uint8_t cc = 0;
    uint8_t ErrorCode = 0;
    if ((pHexData != NULL) && (pHexDataInfo != NULL))
    {
        /* Determine whether the data starts with ':' */
        if ((*pHexData) != ':')
        {
            ErrorCode = 1;
            return ErrorCode;
        }
        else
        {  /* Judge whether the data verification is correct */
            for (i = 0; i < (AsciiToDec(pHexData + 1) << 4) +                 \
                AsciiToDec(pHexData + 2) + 4; i++)
            {
                cc += (AsciiToDec(pHexData + 1 + i * 2) << 4) +               \
                    AsciiToDec(pHexData + 2 + i * 2);
            }
            cc = ~(cc % 0x100) + 1;
        }
        pHexDataInfo->SumCheckOk = FALSE;
        if (cc == (AsciiToDec(pHexData + i * 2 + 1) << 4) +                   \
            AsciiToDec(pHexData + i * 2 + 2))
        {  /* Data verification is correct */
            switch (*(pHexData + 8))
            {
            case RCD_DATA:
                pHexDataInfo->DataLength = (AsciiToDec(pHexData + 1) << 4) +  \
                    AsciiToDec(pHexData + 2);
                pHexDataInfo->OffsetAddr = (AsciiToDec(pHexData + 3) << 12) + \
                    (AsciiToDec(pHexData + 4) << 8) +                         \
                    (AsciiToDec(pHexData + 5) << 4) +                         \
                    (AsciiToDec(pHexData + 6) << 0);
                pHexDataInfo->Rectype = *(pHexData + 8);
                pHexDataInfo->pData = pHexData + 9;
                pHexDataInfo->SumCheckOk = TRUE;
                ErrorCode = 0;
                if (HaveGetAddr == 0)
                {
                 /*   UserProgramAddr = (HexAddrBase << 16) +                 \
                        pHexDataInfo->OffsetAddr; */
                    HaveGetAddr = 1;
                }
                break;
            case RCD_END_FILE:
                pHexDataInfo->DataLength = (AsciiToDec(pHexData + 1) << 4) +  \
                    AsciiToDec(pHexData + 2);
                pHexDataInfo->Rectype = *(pHexData + 8);
                pHexDataInfo->SumCheckOk = TRUE;
                ErrorCode = 0;
                break;
            case RCD_EXT_SEGMENT_ADDR:
                pHexDataInfo->Rectype = *(pHexData + 8);
                pHexDataInfo->SumCheckOk = TRUE;
                ErrorCode = 0;
                break;
            case RCD_START_SEGMENT_ADDR:
                pHexDataInfo->Rectype = *(pHexData + 8);
                pHexDataInfo->SumCheckOk = TRUE;
                ErrorCode = 0;
                break;
            case RCD_EXT_LINEAR_ADDR:
                pHexDataInfo->Rectype = *(pHexData + 8);
                pHexDataInfo->DataLength = (AsciiToDec(pHexData + 1) << 4) +  \
            AsciiToDec(pHexData + 2);

                pHexDataInfo->AddrBase = (AsciiToDec(pHexData + 9) << 12) +   \
                    (AsciiToDec(pHexData + 10) << 8) +
                    (AsciiToDec(pHexData + 11) << 4) +                        \
                    (AsciiToDec(pHexData + 12) << 0);
                HexAddrBase = pHexDataInfo->AddrBase;
                pHexDataInfo->SumCheckOk = TRUE;
                ErrorCode = 0;
                break;
            case RCD_START_LINEAR_ADDR:
                pHexDataInfo->Rectype = *(pHexData + 8);
                pHexDataInfo->DataLength = (AsciiToDec(pHexData + 1) << 4) +  \
            AsciiToDec(pHexData + 2);
                pHexDataInfo->SumCheckOk = TRUE;
                ErrorCode = 0;
                break;
            default:
                pHexDataInfo->Rectype = RCD_ERROR_TYPE;
                pHexDataInfo->SumCheckOk = TRUE;
                ErrorCode = 2;
                break;
            }
        }
        else
        {
            ErrorCode = 3;
        }
    }
    else
    {
        ErrorCode = 4;
    }
    return ErrorCode;
}

/******************************************************************************
 * @brief  Get useful data for a row of hex file
 * @param  pHexData: Converted ASCII data address
           pHexDataInfo: A row of hex data information
 * @retval The result of getting data
 * @attention  None
******************************************************************************/
uint8_t GetHexData(t_HexLineInfo *pHexDataInfo, uint8_t *pHexData)
{
    uint8_t ErrorCode = 0;
    uint8_t i = 0;
    if ((pHexDataInfo != NULL) && (pHexData != NULL))
    {
        switch (pHexDataInfo->Rectype)
        {
        case RCD_DATA:
            for (i = 0; i < pHexDataInfo->DataLength; i++)
            {
                pHexData[i]= (AsciiToDec(pHexDataInfo->pData + 2 * i) << 4)+ \
                (AsciiToDec(pHexDataInfo->pData + 2 * i + 1));
            }
            break;
        case RCD_END_FILE:
            break;
        case RCD_EXT_SEGMENT_ADDR:
            break;
        case RCD_START_SEGMENT_ADDR:
            break;
        case RCD_EXT_LINEAR_ADDR:
            break;
        case RCD_START_LINEAR_ADDR:
            break;
        default:
            ErrorCode = 1;
            break;
        }
    }
    else
    {
        ErrorCode = 2;
    }
    return ErrorCode;
}

/******************************************************************************
 * @brief  Receive a file using the ascii protocol
 * @param  None
 * @retval None
 * @attention  None
******************************************************************************/
int32_t Ascii_Receive (void)
{

    static int8_t TempResult = 1;
    /* Calculation needs to erase pages (actual number of pages - 1) */
	uint8_t chEraseNumber = wAppSize / FLASH_PAGE_SIZE ;

	/* Enable the UART_IT interrupt */
    IAP_UART->IER  |= 0x0002;
	if(IAP_UART == UART1)
	{
		M0_NVIC_Init( 1,  UART1_IRQn, ENABLE) ;
		tNVIC_Table.pUART1_IRQHandler = IAP_UART_Processing;
	}
	else if(IAP_UART == UART2)
	{
		M0_NVIC_Init( 1,  UART2_IRQn, ENABLE) ;
		tNVIC_Table.pUART2_IRQHandler = IAP_UART_Processing;
	}


	FLASH_Unlock();
    do
    {
        /* Erase pages required by app */
        FLASH_ErasePage(APPLICATION_ADDRESS + chEraseNumber * FLASH_PAGE_SIZE);
        if (0 == chEraseNumber)
        {
            break;
        }
        chEraseNumber --;
    }
    while (1);
    FLASH_Lock();
    SerialPutString(IAP_UART, "Waiting for user application!\r\n");

    while(1)
	{
		if (fsm_rt_cpl == Received_App())
		{ /* Check whether the app has been received */

            ProgramHexToFlash(UartRxBuf, USE_DFT_ADDR, APPLICATION_ADDRESS);
            TempResult = 0 ;
			break ;
		}
	}

  return TempResult;
}

#endif

/******************* (C) COPYRIGHT 2020 ***************************/

