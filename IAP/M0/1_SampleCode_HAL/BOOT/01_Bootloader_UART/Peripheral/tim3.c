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
volatile uint8_t RecTimeOutFlag = 0;
/* Exported function prototypes ---------------------------------------------*/
/* Exported function --------------------------------------------------------*/

/******************************************************************************
 * @brief  Update interrupt handler
 * @param  None
 * @retval  None
 * @attention  This is the actual TIM3_UP interrupt processing entry
******************************************************************************/
void TIM3_Processing(void)
{
    if ((TIM_GetITStatus(TIM3,TIM_IT_Update)) != RESET)
    {
        /* Clear the IT pending Bit */
        TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
        RecTimeOutFlag = UART_TIME_OUT;
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
    NVIC_InitTypeDef  NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    
    /* TIM3clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
    
    tNVIC_Table.pTIM3_IRQHandler = TIM3_Processing;

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    TIM_DeInit(TIM3);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = 100;
    TIM_TimeBaseStructure.TIM_Prescaler = SystemCoreClock / 2000;  /* 0.5ms */
    /* Setting Clock Segmentation */
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    /* TIM Upward Counting Mode */
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    /* Clear TIM3 update pending flag */
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);      
    /* Enable the Interrupt sources */
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
    TIM_SetCounter(TIM3,0);  /* Clear the tim3 counter */
    TIM_Cmd(TIM3,DISABLE);  /* Disable the TIM Counter */
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

