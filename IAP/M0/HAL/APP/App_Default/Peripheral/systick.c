/******************************************************************************
 * @file    systick.c
 * @author  MM32 AE
 * @version V1.00
 * @date    10-June-2020
 * @brief   ......
 ******************************************************************************
 * @attention
 *
 * THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
 * CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
 * TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
 * HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
 * CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.
 * <H2><CENTER>&COPY; COPYRIGHT 2020 MINDMOTION </CENTER></H2>
******************************************************************************/


/* Define to prevent recursive inclusion ------------------------------------*/
#define __SYSTICK_C__


/* Includes -----------------------------------------------------------------*/
#include "systick.h"


/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/


/* Private variables --------------------------------------------------------*/
volatile uint32_t SysTick_DelayTicks = 0;


/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/


/* Exported variables -------------------------------------------------------*/
extern uint32_t SystemCoreClock;

/* Exported function prototypes ---------------------------------------------*/


/******************************************************************************
 * @brief
 * @param
 * @retval
 * @attention
******************************************************************************/
void SysTick_Init(uint32_t ticks)
{
    if(SysTick_Config(SystemCoreClock / ticks))  /*  RCC_GetSysClockFreq */
    {
        /* Capture error */
        while(1);
    }

    NVIC_SetPriority(SysTick_IRQn, 0);
}


/******************************************************************************
 * @brief
 * @param
 * @retval
 * @attention
******************************************************************************/
void SysTick_Handler(void)
{
    if(SysTick_DelayTicks)
    {
        SysTick_DelayTicks--;
    }
}


/******************************************************************************
 * @brief
 * @param
 * @retval
 * @attention
******************************************************************************/
void SysTick_DelayMs(uint32_t ms)
{
    SysTick_DelayTicks  =  ms;
    while(SysTick_DelayTicks);
}


/******************* (C) COPYRIGHT 2020 ***************************/

