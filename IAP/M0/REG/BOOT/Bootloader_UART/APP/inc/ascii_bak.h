/******************************************************************************
 * @file    ascii.h
 * @author  MM32 AE
 * @version V1.01
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
#ifndef _ASCII_H_
#define _ASCII_H_


#ifdef __cplusplus
extern "C" {
#endif


#undef  EXTERN


#ifdef  __ASCII_C__
#define EXTERN
#else
#define EXTERN extern
#endif

/* Includes ------------------------------------------------------------------*/
#include "config.h"
#include "main.h"
#include "stdbool.h"

/* Exported variables -------------------------------------------------------*/
/* Exported function prototypes ---------------------------------------------*/
/* Exported macro ---------------------------------------------*/

/* Exported constants -------------------------------------------------------*/
/*Use the default address value in the hex file*/
#define USE_DFT_ADDR                0x01
/*Use the address value you specify*/
#define USE_USER_DEFINED_ADDR       0x02
/* Cache size of UART received data */
#define UART_REC_LEN     3100
/*Buffer size of temporary hex file data*/
#define BUFFER_SIZE      3000

#define SET     true
#define RESET   false
#define AUTO    true
#define MANUAL  false

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

/*Used to record data, most records of hex file are data records*/
#define     RCD_DATA                '0'
/*It is used to identify the end of the file, put at the end of the file,
and identify the end of the hex file*/
#define     RCD_END_FILE            '1'
/*Record used to identify the extension address*/
#define     RCD_EXT_SEGMENT_ADDR    '2'
/*The record used to identify the address of the starting segment*/
#define     RCD_START_SEGMENT_ADDR  '3'
/*Records used to identify extended linear addresses*/
#define     RCD_EXT_LINEAR_ADDR     '4'
/*A record used to identify the starting linear address*/
#define     RCD_START_LINEAR_ADDR   '5'
/*Error type*/
#define     RCD_ERROR_TYPE           0

/* Exported typedef -------------------------------------------------------*/

//! \name finit state machine state
//! @{
typedef enum {
   fsm_rt_err          = -1,    //!< fsm error
   fsm_rt_cpl          = 0,     //!< fsm complete
   fsm_rt_on_going     = 1,     //!< fsm on-going
} fsm_rt_t;
//! @}

typedef struct {
    bool bAutoReset;    //  reset type
    bool bIsSet;        //  initial state
} event_t;


typedef struct
{
/*The length field of this record. It represents the number of bytes of data*/
    uint32_t    DataLength;
    uint32_t    AddrBase;     /*Base address information*/
    uint32_t    OffsetAddr;   /*Offset address information*/
    uint32_t    Rectype;  /*This field represents the type of the hex record*/
    uint32_t    SumCheckOk;   /*The data and verification are correct*/
    uint8_t     *pData;       /*Data field pointer*/
    //uint8_t   *p_NextLineAddr;   /*Next row of hex data pointer*/
} t_HexLineInfo;

/* Exported function prototypes ---------------------------------------------*/

EXTERN uint8_t AsciiToDec(uint8_t *pAsciiData) ;
EXTERN uint8_t GetHexLineInfo(uint8_t *pHexData, t_HexLineInfo *pHexDataInfo) ;
EXTERN uint8_t GetHexData(t_HexLineInfo *pHexDataInfo, uint8_t *pHexData) ;
EXTERN int32_t Ascii_Receive (void);

#ifdef  __cplusplus
}
#endif

#endif

/******************* (C) COPYRIGHT 2020 ***************************/

