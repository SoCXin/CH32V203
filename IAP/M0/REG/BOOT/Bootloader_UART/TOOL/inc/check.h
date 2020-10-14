/******************************************************************************
 * @file    check.h
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
#ifndef __CHECK_H__
#define __CHECK_H__


#ifdef __cplusplus
extern "C" {
#endif


#undef  EXTERN


#ifdef  __CHECK_C__
#define EXTERN
#else
#define EXTERN extern
#endif


/* Includes -----------------------------------------------------------------*/
#include "config.h"

/* Exported constants -------------------------------------------------------*/
/* Exported types -----------------------------------------------------------*/
/* Exported macro -----------------------------------------------------------*/
/* Exported variables -------------------------------------------------------*/

/* Exported function prototypes ---------------------------------------------*/

EXTERN uint16_t Cal_CRC16(const uint8_t* data, uint32_t size);
EXTERN uint8_t CalChecksum(const uint8_t* data, uint32_t size);
EXTERN void Int2Str(uint8_t* str, int32_t intnum) ;
EXTERN uint32_t Str2Int(uint8_t *inputstr, int32_t *intnum);

#ifdef __cplusplus
}
#endif


#endif


/******************* (C) COPYRIGHT 2020 ***************************/

