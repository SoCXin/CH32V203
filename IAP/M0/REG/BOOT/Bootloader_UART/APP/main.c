/******************************************************************************
 * @file    main.c
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
#define __MAIN_C__


/* Includes -----------------------------------------------------------------*/
#include "main.h"
#include "m0_interrupt_table_redefine.h"

/* Includes -----------------------------------------------------------------*/
#include "uart.h"
#include "tim3.h"
#include "flash.h"
#include "check.h"

/* Includes -----------------------------------------------------------------*/

/* Private typedef ----------------------------------------------------------*/
typedef void (*pFunction)(void);
typedef enum
{
    WAIT_UNAUTH = -1,
    WAIT_SUCCESS,
    WAIT_FAILED_CONN,
    WAIT_DISCONN,
    WAIT_ERRORS,
    WAIT_NO_NEED
} WaitResult;

typedef struct
{
    uint8_t UpgradeReqFlag;
    uint8_t AppExsitFlag;
    uint8_t AppBinCheck;
    uint8_t UpBaud;
} ParaBlock ;

/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
#define DISABLE_INT_USER() {TIM3->SR = 0;TIM3->DIER &= 1;IAP_UART ->ICR =0xFF; \
        IAP_UART->IER &= 2;}

/* Private variables --------------------------------------------------------*/
pFunction Jump_To_Application;
__IO uint32_t JumpAddress;

static ParaBlock IAPParaBlock_s =
{
    0,      /* Whether or not there is request of upgrading  0:NO ; 1:YES */
    0,      /* Whether or not there is APP   0:NO ; 1:YES  */
    0,      /* AppBinCheckSUM : Cumulative sum */
    1,      /* UART_BAUD  0: 9600 ; 1: 115200 */
};

#if (CHECKAPP_EN)
#ifndef ASCII_EN
static uint8_t TempCheckData = 0 ;
#endif
#endif

/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/

/******************************************************************************
 * @brief  Jump to the specified address
 * @param  wUserFlashAddr: 0x08000000 ~ 0x08xxxxxx
 * @retval  None
 * @attention  None
******************************************************************************/
static void Iap_Jump_To_Address(uint32_t wUserFlashAddr)
{
    if (((*(__IO uint32_t *)(wUserFlashAddr + 4)) & 0xFF000000) == 0x08000000)
    {
        /* Judge whether it is 0x08xxxxxx. */
        if (((*(__IO uint32_t *)wUserFlashAddr) & 0x2FFE0000) == 0x20000000)
        {
            /* Check if the stack top address is legal. */

            JumpAddress = *(__IO uint32_t *)(wUserFlashAddr + 4);
            Jump_To_Application = (pFunction) JumpAddress;
            /* Initialize user application's Stack Pointer */
            __set_MSP(*(__IO uint32_t *) wUserFlashAddr);
            Jump_To_Application();  /* Jump to user application */
        }
        #if DEBUG_EN
        /* It will not return to this location when jumping success */
        SerialPutString(IAP_UART, "Application error, unable to execute!\r\n");
        #endif
    }
    else
    {
        #if DEBUG_EN
        SerialPutString(IAP_UART, "Not a flash application, unable to execute! \
        \r\n");
        #endif
        NVIC_SystemReset() ;  /*Restart and wait for the next upgrade */
    }
}

/******************************************************************************
 * @brief  BootLoader implement
 * @param  None
 * @retval  None
 * @attention  None
******************************************************************************/
static void BootLoader_Entry(void)
{
    static int8_t LoadAppResult = -1;

    #if MERGE_HEX_EN
    if((IAPParaBlock_s.AppExsitFlag==0x01&&IAPParaBlock_s.UpgradeReqFlag==0)|| \
        (IAPParaBlock_s.AppExsitFlag==0xFF&&IAPParaBlock_s.UpgradeReqFlag==0xFF))
    #else
    if((IAPParaBlock_s.AppExsitFlag==0x01&&IAPParaBlock_s.UpgradeReqFlag==0))
    #endif
    {  /* When AppExsitFlag & UpgradeReqFlag are 0xFF at the same time,
        it represents the first run after the merged program was burned */
        LoadAppResult = WAIT_NO_NEED ;
    }
    else
    {
        if(IAPParaBlock_s.UpgradeReqFlag == 0x01  ||                          \
            IAPParaBlock_s.UpgradeReqFlag == 0xFF)
        {
            #if defined (XMODEM_EN) || defined (XMODEM1K_EN)
            #if DEBUG_EN
            SerialPutString(IAP_UART, "\r\n  ==== XmodemBootloader is running \
            ========\r\n\r\n");
            #endif
            /*Using XMODEM (1K) protocol to transmit user program data*/
            LoadAppResult = Xmodem_Receive(XM_CHECKTYPE);
            #elif defined (YMODEM_EN)  || defined (YMODEM1K_EN)
            #if DEBUG_EN
            SerialPutString(IAP_UART, "\r\n  ==== YmodemBootloader is running \
            ========\r\n\r\n");
            #endif
            /*Using YMODEM (1K) protocol to transmit user program data*/
            LoadAppResult = Ymodem_Receive();
            #elif defined (BINARY_EN)
            #if DEBUG_EN
            SerialPutString(IAP_UART, "\r\n  ==== BianryBootloader is running \
            ========\r\n\r\n");
            #endif
            /*Using Binary protocol to transmit user program data*/
            LoadAppResult = Binary_Receive();
            #elif defined (ASCII_EN)
            #if DEBUG_EN
            SerialPutString(IAP_UART, "\r\n  ==== AsciiBootloader is running  \
            =======\r\n\r\n");
            #endif
            /*Using Ascii protocol to transmit user program data*/
            LoadAppResult = Ascii_Receive();
            #else
            /*Transport protocol not defined, output warning*/
            LoadAppResult = WAIT_UNAUTH ;
            #endif
        }
        else if(IAPParaBlock_s.UpgradeReqFlag == 0x00)
        {
            LoadAppResult = WAIT_NO_NEED ;
        }
    }

    switch (LoadAppResult)
    {
    case WAIT_UNAUTH:
        #if DEBUG_EN
        SerialPutString(IAP_UART, "transport protocol not specified!!!\r\n");
        #endif
        break;
    case WAIT_SUCCESS:
        #if (CHECKAPP_EN)
        {
            #ifndef ASCII_EN
            /* Need to check the bin data.During debugging the program,you can \
               shield this operations*/
            TempCheckData = CalChecksum((const uint8_t*)(APPLICATION_ADDRESS), \
                AppRxSize);
            if( TempCheckData ==  IAPParaBlock_s.AppBinCheck                   \
                   || IAPParaBlock_s.AppBinCheck == 0xFF)
            {
                IAPParaBlock_s.AppBinCheck = TempCheckData ;
                DISABLE_INT_USER()  ;
                Iap_Jump_To_Address(APPLICATION_ADDRESS);
            }
            else
            {
                #if DEBUG_EN
                SerialPutString(IAP_UART, "APP Bin data check failed!!!\r\n");
                SerialPutString(IAP_UART, "CalChecksum is :\r\n");
                #endif
                UARTx_WriteByte(IAP_UART,TempCheckData);
                LoadAppResult = WAIT_ERRORS ;
            }
            #else
            DISABLE_INT_USER() ;
            Iap_Jump_To_Address(APPLICATION_ADDRESS);
            #endif
        }
        #else
        DISABLE_INT_USER() ;
        /* __disable_irq(); */
        Iap_Jump_To_Address(APPLICATION_ADDRESS);
        #endif

        break;
    case WAIT_FAILED_CONN:
        #if DEBUG_EN
        SerialPutString(IAP_UART, "The link is not connected!!!\r\n");
        #endif
        if(IAPParaBlock_s.AppExsitFlag != 0xFF )
        {  /* To write the AppExsitFlag=1 */
            DISABLE_INT_USER() ;
            Iap_Jump_To_Address(APPLICATION_ADDRESS);
        }
        break;
    case WAIT_DISCONN:
        #if DEBUG_EN
        SerialPutString(IAP_UART, "The transmission was terminated!!!\r\n");
        #endif
        break;
    case WAIT_ERRORS:
        #if DEBUG_EN
        SerialPutString(IAP_UART, "Too much error during transmission!!!\r\n");
        #endif
        break;
    case WAIT_NO_NEED:
        DISABLE_INT_USER() ;
        Iap_Jump_To_Address(APPLICATION_ADDRESS);
        break;
    }
    if(LoadAppResult !=WAIT_SUCCESS && LoadAppResult != WAIT_NO_NEED)
    {
        #if DEBUG_EN
        SerialPutString(IAP_UART, "Manual restart required!!!\r\n");
        #endif
        LoadAppResult = -1 ;
        FLASH_Unlock();
        FLASH_ErasePage(PARA_ADDRESS);
        FLASH_Lock();
        NVIC_SystemReset() ;  /*Restart and wait for the next upgrade */
    }
}

/* Exported variables -------------------------------------------------------*/

uint32_t AppRxSize = 0 ;  /* Save received app size */

/* define Interrupt handler pointer table£¬which is at 0X20000000 */
#ifndef USE_IAR
NVIC_TABLE_t tNVIC_Table __attribute__((at(0x20000000)));
#else
NVIC_TABLE_t tNVIC_Table __attribute__((section(".ARM.__at_0x20000000")));
#endif
/* System Clock Frequency (Core Clock = HSI/6) */
uint32_t SystemCoreClock = 8000000 ;

/* Exported function prototypes ---------------------------------------------*/
/* Exported function --------------------------------------------------------*/

/******************************************************************************
 * @brief  main
 * @param  None
 * @retval
 * @attention
******************************************************************************/
int main(void)
{
    /* __enable_irq() ;  */   /* If use __disable_irq() before jumping boot */
    /* Config IAP_UART with parameter(IAP_BAUD, N, 8, 1) for printf */
    UARTx_Configure(IAP_UART, IAP_BAUD);

    /* Config TIM3 */
    TIM3_Configure();
    #if DEBUG_EN
    SerialPutString(IAP_UART, "BOOT is running !\r\n");
    #endif
    IAPParaBlock_s.UpgradeReqFlag = *(vu8*)(PARA_ADDRESS) ;
    IAPParaBlock_s.AppExsitFlag = *(vu8*)(PARA_ADDRESS+1) ;
    IAPParaBlock_s.AppBinCheck = *(vu8*)(PARA_ADDRESS+2) ;
    IAPParaBlock_s.UpBaud = *(vu8*)(PARA_ADDRESS+3) ;
//    Iap_Jump_To_Address(APPLICATION_ADDRESS);
    /* Download app entrance */
    BootLoader_Entry();

    return 0;
}


/******************* (C) COPYRIGHT 2020 ***************************/

