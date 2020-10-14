/******************************************************************************
 * @file    binar.c
 * @author  MM32 AE
 * @version V1.00
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
#define __BINARY_C__


/* Includes -----------------------------------------------------------------*/
#include    "config.h"
#include    "flash.h"
#include    "stdbool.h"
#include    "uart.h"
#include    "string.h"
#include    <stdio.h>
#include    "main.h"
#include    "m0_interrupt_table_redefine.h"
#include    "queue.h"

#if defined (BINARY_EN)

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
static QUEUE8_t QueueUartRx         = {0};
static uint8_t  UartRxBuf[1500]      = {0};
static uint8_t APPBuf[1024] = {0};
static uint32_t RxLength;
static uint16_t EraseCounter = 0 ;
static uint16_t UartRxCnt = 0;

static AppControlBlock s_tAppControlBlock =
{
    APPLICATION_ADDRESS,      /* APP starting address */
    0,                        /* APP size(in Bytes) */
    0,                        /* Key value */
    AUTO,                     /* Automatic reset */
    RESET,                    /* None event */
    false,                    /* Whether to turn on read protection */
};

/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/


/******************************************************************************
 * @brief  Read data from UART receive buffer
 * @param  buffter , buffterSize
 * @retval The result of reading
 * @attention  None
******************************************************************************/
static uint32_t UART_RxRead(uint8_t *Buffter, uint32_t BuffterSize)
{
    return Queue_PacketOut(&QueueUartRx, Buffter, BuffterSize);
}

/******************************************************************************
 * @brief  Write data to UART receive buffer
 * @param  buffter , writeLen
 * @retval The result of writing
 * @attention  None
******************************************************************************/
static uint32_t UART_RxWrite(uint8_t *Buffter, uint32_t WriteLen)
{
    return Queue_PacketIn(&QueueUartRx, Buffter, WriteLen);
}

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
    if ((IAP_UART->CSR & 0x0002) != (uint16_t)RESET)
    {
        IAP_UART->ICR = 0x0002;
        ResByte = (uint8_t)(IAP_UART->RDR & (uint8_t)0xFF);
       /* Push the data into the queue. The queue size is currently set to 2K */
        UART_RxWrite(&ResByte, 1);
        UartRxCnt++;
    }
}

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported function prototypes ---------------------------------------------*/
extern void M0_NVIC_Init(uint32_t NVIC_IRQChannelPriority,                     \
    IRQn_Type NVIC_IRQChannel,FunctionalState NVIC_IRQChannelCmd) ;

/* Exported function ---------------------------------------------------------*/

/******************************************************************************
 * @brief  Receive a file using the binary protocol
 * @param  None
 * @retval The resule of transmission
 * @attention  None
******************************************************************************/
int32_t Binary_Receive(void)
{
    static int8_t TempResult = 1;
    /* Calculation needs to erase pages (actual number of pages - 1) */
    uint8_t chEraseNumber = wAppSize / FLASH_PAGE_SIZE ;

    /* Create receive queue */
    Queue_PacketCreate(&QueueUartRx, UartRxBuf, sizeof(UartRxBuf));

    /* Enable the UART_IT interrupt */
    IAP_UART->IER  |= 0x0002;
    #ifdef MM32F003_Q
    if (IAP_UART == UART2)
    {
        M0_NVIC_Init( 1,  UART2_IRQn, ENABLE) ;
        tNVIC_Table.pUART2_IRQHandler = IAP_UART_Processing;
    }
    #else
    if (IAP_UART == UART1)
    {
        M0_NVIC_Init( 1,  UART1_IRQn, ENABLE) ;
        tNVIC_Table.pUART1_IRQHandler = IAP_UART_Processing;
    }
    else if (IAP_UART == UART2)
    {
        M0_NVIC_Init( 1,  UART2_IRQn, ENABLE) ;
        tNVIC_Table.pUART2_IRQHandler = IAP_UART_Processing;
    }
    #endif

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
    while (1)
    {
        /* Get queue data length */
        RxLength = Queue_PacketLengthGet(&QueueUartRx);
        if (fsm_rt_cpl == Received_App())
        {
            /* Check whether the app has been received */

            if (RxLength > 0 && RxLength < 1025)
            {
                /* Read queue data into the APPBuf */
                UART_RxRead(APPBuf, RxLength);
                Iap_WriteAppBin((APPLICATION_ADDRESS + EraseCounter * 1024),   \
                APPBuf,  RxLength) ;
                EraseCounter = 0;
                memset(APPBuf, 0, 1024);
                TempResult = 0 ;
            }
            else
                TempResult = 3 ;
            break ;
        }
        else
        {
            if (RxLength > 1024)
            {
                UART_RxRead(APPBuf, 1024);
                Iap_WriteAppBin((APPLICATION_ADDRESS + EraseCounter * 1024),   \
                APPBuf,  1024) ;
                EraseCounter++;
             /*   memset(APPBuf, 0, 1024); */
                #if DEBUG_EN
                SerialPutString(IAP_UART, "Firmware receiving>>>\r\n");
                #endif
            }
        }
    }

    return TempResult;
}


#endif


/******************* (C) COPYRIGHT 2020 ***************************/

