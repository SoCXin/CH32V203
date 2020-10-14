/******************************************************************************
 * @file    asm_config.h
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
#ifndef __ASM_CONFIG_H__
#define __ASM_CONFIG_H__


APPLICATION_ADDR  EQU  0x08001400

APPADDR_NMI_Handler        EQU  APPLICATION_ADDR+0x08   
APPADDR_HardFault_Handler  EQU  APPLICATION_ADDR+0x0C   
APPADDR_SVC_Handler        EQU  APPLICATION_ADDR+0x2C   
APPADDR_PendSV_Handler     EQU  APPLICATION_ADDR+0x38 
APPADDR_SysTick_Handler    EQU  APPLICATION_ADDR+0x3C 

#endif

/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

