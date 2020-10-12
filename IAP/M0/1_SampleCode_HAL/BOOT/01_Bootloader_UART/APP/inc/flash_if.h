

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_IF_H
#define __FLASH_IF_H

/* Includes ------------------------------------------------------------------*/
#include "MM32L073PF.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define USER_FLASH_LAST_PAGE_ADDRESS  0x0800F800
#define USER_FLASH_END_ADDRESS        0x0800FFFF /* 64 KBytes */
#define FLASH_PAGE_SIZE               0x400      /* 1 Kbytes */

/* define the address from where user application will be loaded,
   the application address should be a start sector address */
#define APPLICATION_ADDRESS     (uint32_t)0x8005000

/* Get the number of Sector from where the user program will be loaded */
#define  FLASH_PAGE_NUMBER      (uint32_t)((APPLICATION_ADDRESS - 0x08000000) >> 12)

/* Compute the mask to test if the Flash memory, where the user program will be
   loaded, is write protected */
#define  FLASH_PROTECTED_PAGES   ((uint32_t)~((1 << FLASH_PAGE_NUMBER) - 1))

/* define the user application size */
#define USER_FLASH_SIZE   (USER_FLASH_END_ADDRESS - APPLICATION_ADDRESS + 1)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void FLASH_If_Init(void);
unsigned short int MMFLASH_ReadHalfWord(unsigned int faddr);
uint32_t FLASH_If_Erase(uint32_t StartSector);
uint32_t FLASH_If_Write(__IO uint32_t* FlashAddress, uint32_t* Data, uint16_t DataLength, uint32_t file_size);
uint32_t FLASH_If_DisableWriteProtection(void);
uint32_t FLASH_If_GetWriteProtectionStatus(void);

#endif  /* __FLASH_IF_H */


