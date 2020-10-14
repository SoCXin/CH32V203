/******************************************************************************
 * @file    tim1.c
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
#define __TIM1_C__


/* Includes -----------------------------------------------------------------*/
#include "tim1.h"
#include "main.h"
#include "m0_interrupt_table_redefine.h"
#include "bsp_led.h"

/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/


/* Private variables --------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/


/* Exported variables -------------------------------------------------------*/
/* Exported function prototypes ---------------------------------------------*/
/* Exported function --------------------------------------------------------*/

/******************************************************************************
 * @brief  Update interrupt handler
 * @param  None
 * @retval  None
 * @attention  This is the actual TIM1_UP interrupt processing entry
******************************************************************************/
void TIM1_Processing(void)
{
    if ((TIM_GetITStatus(TIM1,TIM_IT_Update)) != RESET)
    {
        /* Clear the IT pending Bit */
        TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
        LED2_TOGGLE();
    }
}

/******************************************************************************
 * @brief  Initialize  timer1
 * @param  None
 * @retval  None
 * @attention  None
 *
******************************************************************************/
void TIM1_Configure(uint16_t hwPrescaler, uint16_t hwPeriod)
{
    NVIC_InitTypeDef  NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /* TIM1clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    tNVIC_Table.pTIM1_BRK_UP_TRG_COM_IRQHandler = TIM1_Processing;

    NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_UP_TRG_COM_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_DeInit(TIM1);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = hwPeriod;
    TIM_TimeBaseStructure.TIM_Prescaler = hwPrescaler;
    /* Setting Clock Segmentation */
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    /* TIM Upward Counting Mode */
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    /* Clear TIM1 update pending flag */
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);
    /* Enable the Interrupt sources */
    TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
    TIM_SetCounter(TIM1,0);  /* Clear the TIM1 counter */
    TIM_Cmd(TIM1,ENABLE);  /* Enable the TIM1 Counter */
}


/******************* (C) COPYRIGHT 2020 ***************************/

