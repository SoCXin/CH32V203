/******************************************************************************
 * @file    flash.c
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
#define __FLASH_C__


/* Includes -----------------------------------------------------------------*/
#include "config.h"
#include "flash.h"

/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/

/* Private functions --------------------------------------------------------*/
/******************************************************************************
 * @brief  Write flash with not checked method
 * @param  wWriteAddr , phwBuffer , hwNumber
 * @retval None
 * @attention  None
******************************************************************************/
static void Flash_Write_NoCheck(uint32_t wWriteAddr, uint16_t *phwBuffer,      \
    uint16_t hwNumber)
{
    uint16_t i;
    if (NULL == phwBuffer)
    {
        return;
    }
    for (i = 0; i < hwNumber; i++)
    {
        FLASH_ProgramHalfWord(wWriteAddr, phwBuffer[i]);
        wWriteAddr += 2;
    }
}


/* Exported variables -------------------------------------------------------*/
/* Exported function --------------------------------------------------------*/

/******************************************************************************
 * @brief  This function is check the read protect status.
 * @param  None 
 * @retval None
 * @attention  None
******************************************************************************/
uint32_t FLASH_CheckReadProtect(void)
{
    uint32_t protectstatus = 0;
    uint16_t data1;
    uint32_t i = 0;

    if ((FLASH->OBR & 0x02) != (u32)RESET) 
    {
        /* Read Protect on 0x1FFFF800 is set */
        protectstatus = 1;
    }
    else 
    {
        for (i = 0; i < 8; i++) 
        {
            /* Address must be an integer multiple of 2 */
            data1 = *(uint16_t*)(0x1ffe0000 + i * 2);  
            if (data1 != 0xFFFF) 
            {
                protectstatus = 2;
                break;
            }
        }
    }
    return protectstatus;
}

/******************************************************************************
 * @brief  This function is portect Full main Flash enable.
 * @param  None 
 * @retval ret
 * @attention  None
******************************************************************************/
int32_t FLASH_EnableReadProtect(void)
{
    FLASH_Status status = FLASH_COMPLETE;
    int32_t ret = 0;
    status = FLASH_ProgramOptionHalfWord(0x1ffe0000, 0x7F80);
    if (status != FLASH_COMPLETE)
        ret = 1;
    status = FLASH_ProgramOptionHalfWord(0x1ffe0002, 0xFF00);
    if (status != FLASH_COMPLETE)
        ret = 1;
    return ret;
}

/******************************************************************************
 * @brief  Application code writing into the user flash
 * @param  wAppxAddr , pchAppBuf , wAppSize(Byte)
 * @retval None
 * @attention  None
******************************************************************************/
void Iap_WriteAppBin(uint32_t wAppxAddr, uint8_t *pchAppBuf, uint32_t wAppSize)
{
#if 1
    /*  Calculate pages to erase(actually num - 1)  */
    uint8_t chEraseNumber = wAppSize / FLASH_PAGE_SIZE ;
    if (NULL != pchAppBuf)
    {
        FLASH_Unlock();
        do    /* Reversely erasing app code using address page */
        {
            FLASH_ErasePage(wAppxAddr + chEraseNumber * FLASH_PAGE_SIZE);
            if (0 == chEraseNumber)
            {
                break;
            }
            chEraseNumber --;
        }while (1);
        Flash_Write_NoCheck(wAppxAddr, (uint16_t *)pchAppBuf, wAppSize / 2);
        FLASH_Lock();
    }
#else
    if (NULL != pchAppBuf)
    {
        FLASH_Unlock();
        Flash_Write_NoCheck(wAppxAddr, (uint16_t *)pchAppBuf, wAppSize / 2);
        FLASH_Lock();
    }
#endif
}

/******************************************************************************
 * @brief  This function does an erase of all user flash area
 * @param  StartSector: start of user flash area
 * @retval 0: user flash area successfully erased
 *         1: error occurred
 * @attention  None
******************************************************************************/
uint32_t FLASH_If_Erase(uint32_t StartSector)
{
    uint32_t flashaddress;

    flashaddress = StartSector;

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

/******************************************************************************
 * @brief  This function writes a data buffer in flash(data are 32-bit aligned).
 * @note   After writing data buffer, the flash content is checked.
 * @param  FlashAddress: start address for writing data buffer
 * @param  Data: pointer on data buffer
 * @param  DataLength: length of data buffer (unit is 32-bit word)
 * @retval 0: Data successfully written to Flash memory
 *         1: Error occurred while writing data in Flash memory
 *         2: Written Data in flash memory is different from expected one
 * @attention  None
******************************************************************************/
uint32_t FLASH_If_Write(__IO uint32_t *FlashAddress, uint32_t *Data,           \
    uint16_t DataLength, uint32_t file_size)
{
    uint32_t i = 0;
    static uint32_t total = 0;

    FLASH_Unlock();
    for (i = 0; (i < DataLength) && (*FlashAddress <= (USER_FLASH_END_ADDRESS- \
        4)); i++, total++)
    {

        if (total * 4 >= file_size)
        {
            total = 0;
            FLASH_Lock();
            return (0);
        }

        /* the operation will be done by wor1d */
        if (FLASH_ProgramWord(*FlashAddress, *(uint32_t *)(Data + i)) ==       \
            FLASH_COMPLETE)
        {
            /* Check the written value */
            if (*(uint32_t *)*FlashAddress != *(uint32_t *)(Data + i))
            {
                /* Flash content doesn't match SRAM content */
                return (2);
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
    FLASH_Lock();

    return (0);
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

