////////////////////////////////////////////////////////////////////////////////
/// @file     HAL_MISC.C
/// @author   AE TEAM
/// @brief    THIS FILE PROVIDES ALL THE MSIC FIRMWARE FUNCTIONS.
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
#define _HAL_MISC_C_

// Files includes
#include "hal_misc.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MSIC_HAL
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MISC_Exported_Functions
/// @{


////////////////////////////////////////////////////////////////////////////////
/// @brief  NVIC initialization.
/// @param  pInitStruct
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void NVIC_Init(NVIC_InitTypeDef* pInitStruct)
{

    if (pInitStruct->NVIC_IRQChannelCmd != DISABLE) {
        NVIC->IP[pInitStruct->NVIC_IRQChannel >> 0x02] =
            (NVIC->IP[pInitStruct->NVIC_IRQChannel >> 0x02] &
             (~(((u32)0xFF) << ((pInitStruct->NVIC_IRQChannel & 0x03) * 8)))) |
            ((((u32)pInitStruct->NVIC_IRQChannelPriority << 6) & 0xFF) << ((pInitStruct->NVIC_IRQChannel & 0x03) * 8));

        NVIC->ISER[0] = 0x01 << (pInitStruct->NVIC_IRQChannel & 0x1F);
    }
    else {
        NVIC->ICER[0] = 0x01 << (pInitStruct->NVIC_IRQChannel & 0x1F);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  NVIC initialized extension function.
/// @param  pInitStruct
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void exNVIC_Init(exNVIC_Init_TypeDef* pInitStruct)
{

    if (pInitStruct->NVIC_IRQChannelCmd != DISABLE) {
        NVIC->IP[pInitStruct->NVIC_IRQChannel >> 0x02] =
            (NVIC->IP[pInitStruct->NVIC_IRQChannel >> 0x02] &
             (~(((u32)0xFF) << ((pInitStruct->NVIC_IRQChannel & 0x03) * 8)))) |
            //           ((((u32)pInitStruct->NVIC_IRQChannelPriority << 6)
            //& 0xFF)
            ((((u32)pInitStruct->NVIC_IRQChannelSubPriority << 6) & 0xFF) << ((pInitStruct->NVIC_IRQChannel & 0x03) * 8));

        NVIC->ISER[0] = 0x01 << (pInitStruct->NVIC_IRQChannel & 0x1F);
    }
    else {
        NVIC->ICER[0] = 0x01 << (pInitStruct->NVIC_IRQChannel & 0x1F);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  System low power mode configuration.
/// @param  LowPowerMode
///         This parameter can be any combination of the following values:
///    @arg NVIC_LP_SEVONPEND
///    @arg NVIC_LP_SLEEPDEEP
///    @arg NVIC_LP_SLEEPONEXIT
/// @param  state: new state of the DMA Channeln.
///         This parameter can be: ENABLE or DISABLE.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void NVIC_SystemLPConfig(u8 LowPowerMode, FunctionalState state)
{
    (state) ? (SCB->SCR |= LowPowerMode) : (SCB->SCR &= ~(u32)LowPowerMode);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  SysTick clock source configuration.
/// @param  SysTick_CLKSource
///         This parameter can be any combination of the following values:
///    @arg SysTick_CLKSource_HCLK_Div8
///    @arg SysTick_CLKSource_HCLK
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void SysTick_CLKSourceConfig(u32 SysTick_CLKSource)
{
    (SysTick_CLKSource == SysTick_CLKSource_HCLK) ? (SysTick->CTRL |= SysTick_CLKSource_HCLK)
    : (SysTick->CTRL &= SysTick_CLKSource_HCLK_Div8);
}

/// @}

/// @}

/// @}
