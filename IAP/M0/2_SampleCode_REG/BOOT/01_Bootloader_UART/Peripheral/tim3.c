/******************************************************************************
 * @file    tim3.c
 * @author  MM32 AE
 * @version V1.00
 * @date    10-June-2020
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
#define __TIM3_C__


/* Includes -----------------------------------------------------------------*/
#include "tim3.h"
#include "main.h"
#include "m0_interrupt_table_redefine.h"

/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/


/* Private variables --------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/


/* Exported variables -------------------------------------------------------*/

volatile uint8_t g_recTimeOutFlag = 0;
/* Exported function prototypes ---------------------------------------------*/
extern void M0_NVIC_Init(uint32_t NVIC_IRQChannelPriority,                     \
    IRQn_Type NVIC_IRQChannel,FunctionalState NVIC_IRQChannelCmd) ;

/* Exported function --------------------------------------------------------*/

/******************************************************************************
 * @brief  Update interrupt handler
 * @param  None
 * @retval  None
 * @attention  This is the actual TIM3_UP interrupt processing entry
******************************************************************************/
void TIM3_Processing(void)
{
    if ((TIM3->SR & 0x01U) != RESET)
    {
        /* Clear the IT pending Bit */
        TIM3->SR = (uint16_t)~(0x01U);
        g_recTimeOutFlag = UART_TIME_OUT;
    }
}

/******************************************************************************
 * @brief  Initialize timeout receive timer
 * @param  None
 * @retval  None
 * @attention  The timer can prevent the receiving state machine from having 
 *              an error state when the frame data is received incorrectly
******************************************************************************/
void TIM3_Configure(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; /* TIM3clock enable */

    TIM3->ARR = 100;
    TIM3->PSC = SystemCoreClock / 2000;  /* 0.5ms */
    /* Clear TIM3 update pending flag */
    TIM3->SR = (uint16_t)~(uint16_t)0x0001;
    /* Enable the Interrupt sources */
    TIM3->DIER |= (uint16_t)0x0001;
    TIM3->CNT = 0;
    TIM3->CR1 &= ((uint16_t)0xFFFE);  /* Disable the TIM Counter */

    tNVIC_Table.pTIM3_IRQHandler = TIM3_Processing;

    M0_NVIC_Init( 1,  TIM3_IRQn, ENABLE) ;
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

