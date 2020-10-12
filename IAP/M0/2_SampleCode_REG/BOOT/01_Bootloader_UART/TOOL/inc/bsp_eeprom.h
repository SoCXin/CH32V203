/******************************************************************************
 * @file    bsp_eeprom.h
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
#ifndef __BSP_EEPROM_H__
#define __BSP_EEPROM_H__


#ifdef __cplusplus
extern "C" {
#endif


#undef  EXTERN


#ifdef  __BSP_EEPROM_C__
#define EXTERN
#else
#define EXTERN extern
#endif


/* Includes -----------------------------------------------------------------*/
#include "config.h"


/* Exported constants -------------------------------------------------------*/
#define EEPROM_I2C_ADDRESS      0xA0
#define EEPROM_PAGE_SIZE        0x08

#define EEPROM_WP_RCC           RCC_AHBPeriph_GPIOA
#define EEPROM_WP_PORT          GPIOA
#define EEPROM_WP_PIN           GPIO_Pin_0


/* Exported types -----------------------------------------------------------*/


/* Exported macro -----------------------------------------------------------*/


/* Exported functions -------------------------------------------------------*/
EXTERN void BSP_EEPROM_Init(void);
EXTERN void BSP_EEPROM_Entry(void);


#ifdef __cplusplus
}
#endif


#endif


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

