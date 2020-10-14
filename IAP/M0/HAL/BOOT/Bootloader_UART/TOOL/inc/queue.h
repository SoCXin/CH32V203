/******************************************************************************
 * @file    queue.h
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _QUEUE_H_
#define _QUEUE_H_


#ifdef __cplusplus
extern "C" {
#endif


#undef  EXTERN


#ifdef  __QUEUE_C__
#define EXTERN
#else
#define EXTERN extern
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"

/* Exported constants -------------------------------------------------------*/

#define DEBUG_FULL_ASSERT       0

/* Exported typedef -------------------------------------------------------*/

typedef volatile struct
{
    volatile uint32_t   BufSize;        /* Queue buffer size */
    volatile uint8_t    *pStart;        /* Queue buffer starting address */
    volatile uint8_t    *pEnd;          /* Queue buffer ending address */
    volatile uint8_t    *pBuf;          /* Queue buffer point */
} QUEUE8_t;


/* Exported macro ---------------------------------------------*/
/* Exported function prototypes ---------------------------------------------*/

EXTERN uint32_t Queue_PacketCreate(QUEUE8_t *pQ8, uint8_t *pBuf,               \
uint32_t BufSize);
EXTERN uint32_t Queue_PacketIn(QUEUE8_t *pQ8, uint8_t *pData, uint32_t Len);
EXTERN uint32_t Queue_PacketOut(QUEUE8_t *pQ8, uint8_t *pData,                 \
uint32_t DataLen);
EXTERN uint32_t Queue_PacketLengthGet(QUEUE8_t *pQ8);


#ifdef  __cplusplus
}
#endif

#endif

/******************* (C) COPYRIGHT 2020 ***************************/

