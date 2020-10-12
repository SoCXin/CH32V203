/******************************************************************************
 * @file    m0_isr_redefine.c
 * @author  MM32 AE
 * @version V1.00
 * @date    20-May-2020
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
#define __M0_ISR_REDEFINE_C__


/* Includes -----------------------------------------------------------------*/
#include "m0_interrupt_table_redefine.h"
//#include "main.h"

/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/


/* Exported variables -------------------------------------------------------*/
/* Exported function prototypes ---------------------------------------------*/
/* Exported function --------------------------------------------------------*/

/* Jumping to the actual interrupt handler in the following interrupt service  \
functions */
void WWDG_IRQHandler(void)
{
    tNVIC_Table.pWWDG_IRQHandler();
}

void PVD_IRQHandler(void)
{
    tNVIC_Table.pPVD_IRQHandler();
}

void FLASH_IRQHandler(void)
{
    tNVIC_Table.pFLASH_IRQHandler();
}

void EXTI0_1_IRQHandler(void)
{
    tNVIC_Table.pEXTI0_1_IRQHandler();
}

void EXTI2_3_IRQHandler(void)
{
    tNVIC_Table.pEXTI2_3_IRQHandler();
}

void EXTI4_15_IRQHandler(void)
{
    tNVIC_Table.pEXTI4_15_IRQHandler();
}

void DMA1_Channel1_IRQHandler(void)
{
    tNVIC_Table.pDMA1_Channel1_IRQHandler();
}
void DMA1_Channel2_3_IRQHandler(void)
{
    tNVIC_Table.pDMA1_Channel2_3_IRQHandler();
}

void DMA1_Channel4_5_IRQHandler(void)
{
    tNVIC_Table.pDMA1_Channel4_5_IRQHandler();
}

void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
    tNVIC_Table.pTIM1_BRK_UP_TRG_COM_IRQHandler();
}
void TIM1_CC_IRQHandler(void)
{
    tNVIC_Table.pTIM1_CC_IRQHandler();
}
void TIM2_IRQHandler(void)
{
    tNVIC_Table.pTIM2_IRQHandler();
}
void TIM3_IRQHandler(void)
{
    tNVIC_Table.pTIM3_IRQHandler();
}
void TIM14_IRQHandler(void)
{
    tNVIC_Table.pTIM14_IRQHandler();
}
void TIM16_IRQHandler(void)
{
    tNVIC_Table.pTIM16_IRQHandler();
}
void TIM17_IRQHandler(void)
{
    tNVIC_Table.pTIM17_IRQHandler();
}
void I2C1_IRQHandler(void)
{
    tNVIC_Table.pI2C1_IRQHandler();
}

void UART2_IRQHandler(void)
{
    tNVIC_Table.pUART2_IRQHandler();
}

void SPI2_IRQHandler(void)
{
    tNVIC_Table.pSPI2_IRQHandler();
}

#ifndef MM32F003_Q
void SPI1_IRQHandler(void)
{
    tNVIC_Table.pSPI1_IRQHandler();
}

void UART1_IRQHandler(void)
{
    tNVIC_Table.pUART1_IRQHandler();
}
#endif

#if defined (MM32F032_S) || defined (MM32L06XX_S)
void RTC_BKP_IRQHandler(void)
{
    tNVIC_Table.pRTC_BKP_IRQHandler();
}

void RCC_CRS_IRQHandler(void)
{
    tNVIC_Table.pRCC_CRS_IRQHandler();
}

void ADC1_COMP_IRQHandler(void)
{
    tNVIC_Table.pADC1_COMP_IRQHandler();
}

void CAN_IRQHandler(void)
{
    tNVIC_Table.pCAN_IRQHandler();
}
void USB_IRQHandler(void)
{
    tNVIC_Table.pUSB_IRQHandler();
}

#elif (MM32SPIN06XX_S)
void RTC_BKP_IRQHandler(void)
{
    tNVIC_Table.pRTC_BKP_IRQHandler();
}

void RCC_CRS_IRQHandler(void)
{
    tNVIC_Table.pRCC_CRS_IRQHandler();
}

void ADC1_COMP_IRQHandler(void)
{
    tNVIC_Table.pADC1_COMP_IRQHandler();
}

void CAN_IRQHandler(void)
{
    tNVIC_Table.pCAN_IRQHandler();
}

#elif (MM32F031_Q)
void RCC_IRQHandler(void)
{
    tNVIC_Table.pRCC_IRQHandler();
}

void ADC1_COMP_IRQHandler(void)
{
    tNVIC_Table.pADC1_COMP_IRQHandler();
}
#elif (MM32F003_Q)
void RCC_IRQHandler(void)
{
    tNVIC_Table.pRCC_IRQHandler();
}

void ADC1_COMP_IRQHandler(void)
{
    tNVIC_Table.pADC1_IRQHandler();
}
#elif (MM32SPIN05_Q)
void RCC_IRQHandler(void)
{
    tNVIC_Table.pRCC_IRQHandler();
}

void ADC1_COMP_IRQHandler(void)
{
    tNVIC_Table.pADC1_COMP_IRQHandler();
}

void HWDIV_IRQHandler(void)
{
    tNVIC_Table.pHWDIV_IRQHandler();
}

#elif (MM32SPIN27_P)
void RCC_IRQHandler(void)
{
    tNVIC_Table.pRCC_IRQHandler();
}

void PWM_IRQHandler(void)
{
    tNVIC_Table.pPWM_IRQHandler();
}

void ADC1_IRQHandler(void)
{
    tNVIC_Table.pADC1_IRQHandler();
}

void ADC2_IRQHandler(void)
{
    tNVIC_Table.pADC2_IRQHandler();
}

void HWDIV_IRQHandler(void)
{
    tNVIC_Table.pHWDIV_IRQHandler();
}

void TIM8_BRK_UP_TRG_COM_IRQHandler(void)
{
    tNVIC_Table.pTIM8_BRK_UP_TRG_COM_IRQHandler();
}

void TIM8_CC_IRQHandler(void)
{
    tNVIC_Table.pTIM8_CC_IRQHandler();
}

void COMP1_2_3_4_5_IRQHandler(void)
{
    tNVIC_Table.pCOMP1_2_3_4_5_IRQHandler();
}

#elif (MM32F031_N)
void RTC_IRQHandler(void)
{
    tNVIC_Table.pRTC_IRQHandler();
}

void RCC_IRQHandler(void)
{
    tNVIC_Table.pRCC_IRQHandler();
}

void ADC1_IRQHandler(void)
{
    tNVIC_Table.pADC1_IRQHandler();
}
#elif (MM32L0XX_N)
void RTC_IRQHandler(void)
{
    tNVIC_Table.pRTC_IRQHandler();
}

void RCC_CRS_IRQHandler(void)
{
    tNVIC_Table.pRCC_CRS_IRQHandler();
}

void ADC_COMP_IRQHandler(void)
{
    tNVIC_Table.pADC_COMP_IRQHandler();
}

void AES_IRQHandler(void)
{
    tNVIC_Table.pAES_IRQHandler();
}

void CAN_IRQHandler(void)
{
    tNVIC_Table.pCAN_IRQHandler();
}

void USB_IRQHandler(void)
{
    tNVIC_Table.pUSB_IRQHandler();
}
#endif

/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/
