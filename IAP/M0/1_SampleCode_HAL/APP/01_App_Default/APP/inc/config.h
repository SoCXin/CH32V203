/******************************************************************************
 * @file    config.h
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

// #define ASCII_EN    /* transfer protocol define */

/* transfer Baud rate define , 9600 or 115200  */
#define IAP_BAUD                115200

#define PARA_ADDRESS            (uint32_t)0x8001000
#define PARA_SIZE               (12)
#define APPLICATION_ADDRESS     (uint32_t)0x8001400

/* The parameter arrangement is :
UpgradeReqFlag + AppExsitFlag + AppBinCheck + UpBaud + бнбн
Note: Each parameter takes only one byte!
*/
#ifdef  MM32F003_Q
#define IAP_UART  UART2   /* Used for IAP */
#define DEBUG_EN
#ifdef DEBUG_EN
#define DEBUG_UART  UART2   /* Used for printf */
#endif

#else

#define IAP_UART  UART1   /* Used for IAP */
#define DEBUG_EN
#ifdef DEBUG_EN
#define DEBUG_UART  UART1   /* Used for printf */
#endif

#endif



/* Exported macro -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif


#endif


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

