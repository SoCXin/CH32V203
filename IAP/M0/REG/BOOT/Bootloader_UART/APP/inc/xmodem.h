/******************************************************************************
 * @file    xmodem.h
 * @author  MM32 AE
 * @version V1.01
 * @date    24-June-2020
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
#ifndef __XMODEM_H__
#define __XMODEM_H__


#ifdef __cplusplus
extern "C" {
#endif


#undef  EXTERN


#ifdef  __XMODEM_C__
#define EXTERN
#else
#define EXTERN extern
#endif

/* Includes -----------------------------------------------------------------*/
#include "config.h"

/* Exported variables -------------------------------------------------------*/
/* Exported macro ---------------------------------------------*/
/* Exported constants -------------------------------------------------------*/

/* check type of CRC define ,0x43 for CRC , 0x15 for SUM*/
#define XM_CHECKTYPE  0x43

#define PACKET_HEADER_SIZE      ((uint32_t)3)
#define PACKET_DATA_INDEX       ((uint32_t)4)
#define PACKET_START_INDEX      ((uint32_t)1)
#define PACKET_NUMBER_INDEX     ((uint32_t)2)
#define PACKET_CNUMBER_INDEX    ((uint32_t)3)
#define PACKET_TRAILER_SIZE     ((uint32_t)2)
#define PACKET_OVERHEAD_SIZE    (PACKET_HEADER_SIZE + PACKET_TRAILER_SIZE - 1)
#define PACKET_SEQNO_INDEX      (1)
#define PACKET_SEQNO_COMP_INDEX (2)
#define PACKET_HEADER           (3)
#define PACKET_TRAILER          (2)
#define PACKET_OVERHEAD         (PACKET_HEADER + PACKET_TRAILER)
#define PACKET_SIZE             (128)
#define PACKET_1K_SIZE          (1024)
#define XPACKET_SEZE		133

#define SOH  0x01
#define STX  0x02
#define EOT  0x04
#define ACK  0x06
#define NAK  0x15
#define CAN  0x18
#define CTRLZ 0x1A
#define CA	 0x18
#define C	 0x43

#define CHECK_CRC       0x43
#define CHECK_SUM       0x15

#define SUCCESSFULL     0X00
#define TIME_OUT        0X01
#define USER_CANCELED   0X02
#define PARAMETER_ERROR 0X03

#ifndef NULL
    #define NULL    ((void *)0)
#endif
#ifndef FALSE
    #define FALSE   (0)
#endif
#ifndef TRUE
    #define TRUE    (1)
#endif
#ifndef ERROR
    #define ERROR    (255)
#endif
#ifndef RIGHT
    #define RIGHT    (0)
#endif

#define DLY_1S 1000
#if (IAP_BAUD>9600)
#define DLY_1MS 1
#else
#define DLY_1MS 1*13
#endif

#define MAXRETRANS 25
#define CONNECT_TIMES 1000

/* Exported function prototypes ---------------------------------------------*/
EXTERN uint8_t Xmodem_Receive(uint8_t CheckType);


#ifdef __cplusplus
}
#endif


#endif


/******************* (C) COPYRIGHT 2020 ***************************/
