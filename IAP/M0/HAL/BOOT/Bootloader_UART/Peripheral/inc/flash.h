/******************************************************************************
 * @file    flash.h
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
#ifndef __FLASH_H__
#define __FLASH_H__


#ifdef __cplusplus
extern "C" {
#endif


#undef  EXTERN


#ifdef  __FLASH_C__
#define EXTERN
#else
#define EXTERN extern
#endif

/* Includes ------------------------------------------------------------------*/
#include "HAL_device.h"
#include "config.h"

/* Exported variables -------------------------------------------------------*/
/* Exported function prototypes ---------------------------------------------*/
/* Exported macro ---------------------------------------------*/
/* Exported constants -------------------------------------------------------*/

#define FLASH_PAGE_SIZE     (1024)   /* The flash page size (in Bytes) */
#define FLASH_BASE_ADDR     (0x08000000) 	/* Starting address of user code */
/* Get the number of Sector from where the user program will be loaded */
#define  FLASH_PAGE_NUMBER  (uint32_t)((APPLICATION_ADDRESS - 0x08000000) >> 12)

/* Compute the mask to test if the Flash memory, where the user program will be
   loaded, is write protected */
#define  FLASH_PROTECTED_PAGES   ((uint32_t)~((1 << FLASH_PAGE_NUMBER) - 1))

/* define the user application size */
#define USER_FLASH_SIZE   (USER_FLASH_END_ADDRESS - APPLICATION_ADDRESS + 1)

/* Exported variables -------------------------------------------------------*/
/* Exported function prototypes ---------------------------------------------*/

EXTERN void Iap_WriteAppBin(uint32_t wAppAddr, uint8_t *pchAppBuf,            \
uint32_t wAppSize) ;
EXTERN uint32_t FLASH_If_Erase(uint32_t StartSector);
EXTERN uint32_t FLASH_If_Write(__IO uint32_t* FlashAddress, uint32_t* Data ,  \
uint16_t DataLength, uint32_t FileSize);
EXTERN FLASH_Status ProgramDatatoFlash(uint32_t *Address, uint16_t *Data,     \
    uint32_t DataNum) ;

#ifdef __cplusplus
}
#endif

#endif

/******************* (C) COPYRIGHT 2020 ***************************/
