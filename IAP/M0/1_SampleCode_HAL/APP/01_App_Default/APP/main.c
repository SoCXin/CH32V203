/******************************************************************************
 * @file    main.c
 * @author  MM32 AE
 * @version V1.00
 * @date    12-June-2020
 * @brief   The app main.c of the IAP_UART project.
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
#include "tim1.h"
#include "flash.h"
#include "systick.h"
#include "bsp_led.h"

/* Includes -----------------------------------------------------------------*/

/* Private typedef ----------------------------------------------------------*/
typedef void (*pFunction)(void);

typedef struct
{
    uint8_t UpgradeReqFlag;
    uint8_t AppExsitFlag;
    uint8_t AppBinCheck;
    uint8_t UpBaud;
} ParaBlock ;

/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
#define DISABLE_INT_USER() {                                                  \
    SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;                                  \
    TIM_ClearITPendingBit(TIM1,TIM_IT_Update);                                \
    TIM_ITConfig(TIM1,TIM_IT_Update,DISABLE);                                 \
    UART_ClearITPendingBit(IAP_UART,UART_IT_RXIEN);                           \
    UART_ITConfig(IAP_UART,UART_IT_RXIEN,DISABLE);}

/* Private variables --------------------------------------------------------*/
pFunction Jump_To_Application;
__IO uint32_t JumpAddress;
static RCC_ClocksTypeDef get_rcc_clock;  /* Get system clock status */
#ifndef  MM32F003_Q
#ifndef USE_IAR
const uint8_t ConstData[13*1011] __attribute__((at(0x8001800))) = {0x55} ; 
#else
const uint8_t ConstData[1]                                               \
    __attribute__((section(".ARM.__at_0x8001800"))) = {0x55} ;
#endif
#else
#ifndef USE_IAR
const uint8_t ConstData[1] __attribute__((at(0x8001800))) = {0x55} ;
#else
const uint8_t ConstData[1]                                                \
    __attribute__((section(".ARM.__at_0x8001800"))) = {0x55} ;
#endif
#endif
#ifndef ASCII_EN
static uint8_t NeedProtect = 0 ;  /* Is read protection required */
static ParaBlock IAPParaBlock_s =
{
    0,      /* Whether or not there is request of upgrading  0:NO ; 1:YES */
    1,      /* Whether or not there is APP   0:NO ; 1:YES  */
    0,      /* AppBinCheckSUM : Cumulative sum */
    1,      /* UART_BAUD  0: 9600 ; 1: 115200 */    
};

#endif
/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/

/******************************************************************************
 * @brief  App_Default implement
 * @param  None
 * @retval  None
 * @attention  None
******************************************************************************/
static void App_Default_Entry(void)
{   
    #ifdef ASCII_EN 
    static uint32_t DelayCount = 10000;
    #endif
    while (1)            
    {
        #ifndef ASCII_EN 
        if(RxOverFlag)
        {  /* Test data is :  55 01 00 4B 01 88 99 AA BB CC FF */
            RxOverFlag = 0 ;
            if(RxBuff[0])
            {                               
                printf("PARA received over £¡\r\n");
                /* Write the IAP parameters to the PARA_ADDRESS  */            
                Iap_WriteAppBin(PARA_ADDRESS,RxBuff,PARA_SIZE) ;
                SysTick_DelayMs(100); 
                DISABLE_INT_USER() ;   
                /* __disable_irq(); */                                 
                NVIC_SystemReset() ;/* Reset then jump into the Bootloader */              
                /* In addition ,jumping into the Bootloader Directly */
                /* Need disable all interrupt before this operation */               
                /* Iap_JumpToAddress(FLASH_BASE_ADDR) ;  */            
            }
        }
        
		if(ConstData[0])
		{
			LED1_TOGGLE();
		}
        LED3_TOGGLE();      
        SysTick_DelayMs(900); 
        #else
        LED3_TOGGLE();
        while(DelayCount--)  __NOP;
        DelayCount = 10000;        
        #endif        
    }
}

/* Exported variables -------------------------------------------------------*/
extern uint32_t SystemCoreClock;
uint8_t RxBuff[PARA_SIZE+2] = {0} ;  /* Cache parameter packet data */
uint8_t RxOverFlag = 0 ;  /* The flag of parameter packet data received over */

/* define Interrupt handler pointer table£¬which is at 0X20000000 */
#ifndef USE_IAR
NVIC_TABLE_t tNVIC_Table __attribute__((at(0x20000000)));
#else
NVIC_TABLE_t tNVIC_Table __attribute__((section(".ARM.__at_0x20000000")));
#endif

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
    int32_t status;
    
   /* __enable_irq() ; */  /* If use __disable_irq() before jumping App */
    /* Init SysTick timer 1ms for SysTick_DelayMs */
    SysTick_Init(1000);
    
    /* Config IAP_UART with parameter(IAP_BAUD, N, 8, 1) for printf */
    UARTx_Configure( IAP_UART, IAP_BAUD) ;

    /* Init Test TIM1 */
    TIM1_Configure(1000-1,SystemCoreClock / 1000 -1 );
    
    /* Init LEDs GPIO */
    BSP_LED_Init();	
	
    #ifndef ASCII_EN 
		RCC_GetClocksFreq(&get_rcc_clock);
    printf("\r\n====== MM32 M0 IAP UART DEMO =====\r\n");
    printf("SystemCoreClock = %d\r\n", SystemCoreClock);
    printf("SYSCLK_Frequency = %d\r\n", get_rcc_clock.SYSCLK_Frequency);
    printf("HCLK_Frequency = %d\r\n", get_rcc_clock.HCLK_Frequency);
    printf("PCLK1_Frequency = %d\r\n", get_rcc_clock.PCLK1_Frequency);
    printf("PCLK2_Frequency = %d\r\n", get_rcc_clock.PCLK2_Frequency);
    
    
    if(NeedProtect)
    {
        /* Check the Read Protect */
        status = FLASH_CheckReadProtect();
        if (status == 0) 
        { /* not protect */
            FLASH_Unlock();
            /* Need disable all interrupt before this operation*/
            status = FLASH_EnableReadProtect();/* Set the Read Protect */
            FLASH_Lock();
            if (status == 0) 
            {
                status = 3;
            }
        }
        else
        {
            status = 3;     
        }
        /*During debugging the program, you can shield the above operations and 
            make status = 3 */
    }
    else
    {
        status = 3 ;
    }
    /* Write the UpgradeReqFlag into the PARA_ADDRESS */
    RxBuff[0] = IAPParaBlock_s.UpgradeReqFlag ;  
    /* Write the AppExsitFlag into the PARA_ADDRESS */
    RxBuff[1] = IAPParaBlock_s.AppExsitFlag ;  
    Iap_WriteAppBin(PARA_ADDRESS,RxBuff,PARA_SIZE) ;
    #else
    status = 3 ;
    #endif
    
    if (status == 3) 
    { /* success */
        /* App default entrance */
        App_Default_Entry();
    }
    else
    {
        printf("Set the Read Protect failed £¡\r\n");
    }
}


void HardFault_Handler(void)
{
    printf("HardFault_Handler in\r\n");
    NVIC_SystemReset();
}

/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

