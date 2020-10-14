

/* Includes ------------------------------------------------------------------*/
#include "flash_if.h"
#include "uart.h"


  
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


//读取指定地址的半字(16位数据)
//faddr:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.
unsigned short int MMFLASH_ReadHalfWord(unsigned int faddr)
{
	return *(volatile unsigned short int*)faddr; 
}


/**
  * @brief  Unlocks Flash for write access
  * @param  None
  * @retval None
  */
void FLASH_If_Init(void)
{ 
  /* Unlock the Program memory */
  FLASH_Unlock();

  /* Clear all FLASH flags */  
  FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_WRPRTERR | FLASH_FLAG_PGERR | FLASH_FLAG_BSY);   
}

/**
  * @brief  This function does an erase of all user flash area
  * @param  StartSector: start of user flash area
  * @retval 0: user flash area successfully erased
  *         1: error occurred
  */
uint32_t FLASH_If_Erase(uint32_t StartSector)
{
  uint32_t flashaddress;
  
  flashaddress = StartSector;
        //63480                       63488
  while (flashaddress <= (uint32_t) USER_FLASH_LAST_PAGE_ADDRESS)
  {
    if (FLASH_ErasePage(flashaddress) == FLASH_COMPLETE)
    {
      //if(*(uint32_t*)flashaddress==0xFFFFFFFF)
				flashaddress += FLASH_PAGE_SIZE;
    }
    else
    {
      /* Error occurred while page erase */
      return (1);
    }
  }
  return (0);
}

/**
  * @brief  This function writes a data buffer in flash (data are 32-bit aligned).
  * @note   After writing data buffer, the flash content is checked.
  * @param  FlashAddress: start address for writing data buffer
  * @param  Data: pointer on data buffer
  * @param  DataLength: length of data buffer (unit is 32-bit word)   
  * @retval 0: Data successfully written to Flash memory
  *         1: Error occurred while writing data in Flash memory
  *         2: Written Data in flash memory is different from expected one
  */

uint32_t FLASH_If_Write(__IO uint32_t* FlashAddress, uint32_t* Data ,uint16_t DataLength, uint32_t file_size)
{
  uint32_t i = 0;
	static uint32_t total=0;
 
	FLASH_Unlock();						//解锁
  for (i = 0; (i < DataLength) && (*FlashAddress <= (USER_FLASH_END_ADDRESS-4)); i++,total++)
  {
		/*待验证*/
		if(total*4 >= file_size)//全部字节写完
		{
			total = 0;
			FLASH_Lock();             //上锁
			return (0);
		}
				
    /* the operation will be done by wor1d */ 
    if (FLASH_ProgramWord(*FlashAddress, *(uint32_t*)(Data+i)) == FLASH_COMPLETE)
    {
     /* Check the written value */		
      if (*(uint32_t*)*FlashAddress != *(uint32_t*)(Data+i))
			{
        /* Flash content doesn't match SRAM content */
        return(2);
      }
      /* Increment FLASH destination address */
      *FlashAddress += 4;
    }
    else
    {
      /* Error occurred while writing data in Flash memory */
      return (1);
    }
  }
	FLASH_Lock();             //上锁

  return (0);
}
/**
  * @brief  Disables the write protection of user desired pages
  * @param  None
  * @retval 0: Write Protection successfully disabled
  *         1: Error: Flash write unprotection failed
  *         2: Flash memory is not write protected
  */
uint32_t FLASH_If_DisableWriteProtection(void)
{
  uint32_t UserMemoryMask = 0, WRPR = 0;
  FLASH_Status status = FLASH_BUSY;

  /* Clear all FLASH flags */  
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_WRPRTERR | FLASH_FLAG_PGERR | FLASH_FLAG_BSY);

  /* Get Write protection */
  WRPR = FLASH_GetWriteProtectionOptionByte();//FLASH_OB_GetWRP
  
  /* Test if user memory is write protected */
  if (FLASH_If_GetWriteProtectionStatus() != 0x00)
  {
    /* Enable the FLASH option byte access */
    FLASH_Unlock();  // FLASH_OB_Unlock

    /* Erase option bytes */
    status = FLASH_EraseOptionBytes();//FLASH_OB_Erase

    /* Compute the User_Mask */
    UserMemoryMask = FLASH_PROTECTED_PAGES | WRPR;
    
    if (UserMemoryMask != 0xFFFFFFFF)
    {
    /* Disable Write protection */
    status = FLASH_EnableWriteProtection((uint32_t)~UserMemoryMask);//FLASH_OB_EnableWRP
    }
    
    if (status == FLASH_COMPLETE)
    {
      /* Write Protection successfully disabled */
      return (0);
    }
    else
    {
      /* Error: Flash write unprotection failed */
      return (1);
    }
  }
  else
  {
     /* Flash memory is not write protected */
     return(2);
  }
}

/**
  * @brief  Returns the write protection status of user flash area.
  * @param  None
  * @retval If the sector is write-protected, the corresponding bit in returned
  *         value is set.
  *         If the sector isn't write-protected, the corresponding bit in returned
  *         value is reset.
  */
uint32_t FLASH_If_GetWriteProtectionStatus(void)
{ 
  return(~FLASH_GetWriteProtectionOptionByte() & FLASH_PROTECTED_PAGES);
}


/**
  * @}
  */


