/******************************************************************************
 * @file    config.h
 * @author  MM32 AE
 * @version V1.00
 * @date    10-June-2020
 * @brief   ......
******************************************************************************/


/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef __CONFIG_H__
#define __CONFIG_H__


#ifdef __cplusplus
extern "C" {
#endif


/* Includes -----------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


/* Includes -----------------------------------------------------------------*/
#include "HAL_conf.h"
#include "HAL_device.h"


/* Exported constants -------------------------------------------------------*/

//#define ASCII_EN    /* transfer protocol define */

/* transfer Baud rate define , 9600 or 115200  */
#define IAP_BAUD                    9600
#define APPLICATION_ADDRESS         (uint32_t)0x8001400
#define PARA_ADDRESS                (uint32_t)0x8001000
#define PARA_SIZE                   (12)

/* The parameter arrangement is :
UpgradeReqFlag + AppExsitFlag + AppBinCheck + UpBaud + бнбн
Note: Each parameter takes only one byte!
*/
#ifdef  MM32F003_Q
    #define IAP_UART    UART2   /* Used for IAP */
    #define DEBUG_EN
    #ifdef  DEBUG_EN
        #define DEBUG_UART  UART2   /* Used for printf */
    #endif
#else
    #define IAP_UART    UART1   /* Used for IAP */
    #define DEBUG_EN
    #ifdef  DEBUG_EN
        #define DEBUG_UART  UART1   /* Used for printf */
    #endif
#endif

#ifdef __cplusplus
}
#endif
#endif


/******************* (C) COPYRIGHT 2020 ***************************/

