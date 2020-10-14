/******************************************************************************
 * @file    bsp_led.h
 * @author  King
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
#ifndef __BSP_LED_H__
#define __BSP_LED_H__


#ifdef __cplusplus
extern "C" {
#endif


#undef  EXTERN


#ifdef  __BSP_LED_C__
#define EXTERN
#else
#define EXTERN extern
#endif


/* Includes -----------------------------------------------------------------*/
#include "config.h"


/* Exported constants -------------------------------------------------------*/
#define LED1_GPIO       GPIOA
#define LED1_PIN        GPIO_Pin_15

#define LED2_GPIO       GPIOB
#define LED2_PIN        GPIO_Pin_3

#define LED3_GPIO       GPIOB
#define LED3_PIN        GPIO_Pin_4

#define LED4_GPIO       GPIOB
#define LED4_PIN        GPIO_Pin_5


/* Exported types -----------------------------------------------------------*/


/* Exported macro -----------------------------------------------------------*/
#define LED1_ON()       GPIO_WriteBit(LED1_GPIO, LED1_PIN, Bit_RESET)
#define LED2_ON()       GPIO_WriteBit(LED2_GPIO, LED2_PIN, Bit_RESET)
#define LED3_ON()       GPIO_WriteBit(LED3_GPIO, LED3_PIN, Bit_RESET)
#define LED4_ON()       GPIO_WriteBit(LED4_GPIO, LED4_PIN, Bit_RESET)

#define LED1_OFF()      GPIO_WriteBit(LED1_GPIO, LED1_PIN, Bit_SET)
#define LED2_OFF()      GPIO_WriteBit(LED2_GPIO, LED2_PIN, Bit_SET)
#define LED3_OFF()      GPIO_WriteBit(LED3_GPIO, LED3_PIN, Bit_SET)
#define LED4_OFF()      GPIO_WriteBit(LED4_GPIO, LED4_PIN, Bit_SET)

#define LED1_TOGGLE()   (GPIO_ReadOutputDataBit(LED1_GPIO, LED1_PIN) ? LED1_ON() : LED1_OFF())
#define LED2_TOGGLE()   (GPIO_ReadOutputDataBit(LED2_GPIO, LED2_PIN) ? LED2_ON() : LED2_OFF())
#define LED3_TOGGLE()   (GPIO_ReadOutputDataBit(LED3_GPIO, LED3_PIN) ? LED3_ON() : LED3_OFF())
#define LED4_TOGGLE()   (GPIO_ReadOutputDataBit(LED4_GPIO, LED4_PIN) ? LED4_ON() : LED4_OFF())


/* Exported functions -------------------------------------------------------*/
EXTERN void BSP_LED_Init(void);


#ifdef __cplusplus
}
#endif


#endif


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

