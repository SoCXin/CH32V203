/******************************************************************************
 * @file    uart.h
 * @author  MM32 AE
 * @version V1.00
 * @date    12-June-2020
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
#ifndef __UART_H__
#define __UART_H__


#ifdef __cplusplus
extern "C" {
#endif


#undef  EXTERN


#ifdef  __UART_C__
#define EXTERN
#else
#define EXTERN extern
#endif


/* Includes -----------------------------------------------------------------*/
#include "config.h"


/* Exported constants -------------------------------------------------------*/
/* Exported types -----------------------------------------------------------*/
/* Exported macro -----------------------------------------------------------*/
#define SerialPutString(UARTx,x)   UARTx_WriteBytes(UARTx,(uint8_t*)(x))

/* Exported function prototypes ---------------------------------------------*/

EXTERN void UARTx_Configure(UART_TypeDef *UARTx,uint32_t BaudRate);
EXTERN void UARTx_WriteByte(UART_TypeDef *UARTx,uint8_t c);
EXTERN void UARTx_WriteBytes(UART_TypeDef *UARTx,uint8_t *s);
EXTERN uint32_t UARTx_ReadByte(UART_TypeDef *UARTx,uint32_t Time);

#ifdef __cplusplus
}
#endif


#endif


/******************* (C) COPYRIGHT 2020 ***************************/

