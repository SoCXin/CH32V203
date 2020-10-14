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

/* FLASH Keys */
#define RDP_Key                  ((uint16_t)0x00A5)
#define FLASH_KEY1               ((uint32_t)0x45670123)
#define FLASH_KEY2               ((uint32_t)0xCDEF89AB)
/* Flash Control Register bits */
#define CR_PG_Set                ((uint32_t)0x00000001)
#define CR_PG_Reset              ((uint32_t)0x00001FFE)
#define CR_PER_Set               ((uint32_t)0x00000002)
#define CR_PER_Reset             ((uint32_t)0x00001FFD)
#define CR_MER_Set               ((uint32_t)0x00000004)
#define CR_MER_Reset             ((uint32_t)0x00001FFB)
#define CR_OPTPG_Set             ((uint32_t)0x00000010)
#define CR_OPTPG_Reset           ((uint32_t)0x00001FEF)
#define CR_OPTER_Set             ((uint32_t)0x00000020)
#define CR_OPTER_Reset           ((uint32_t)0x00001FDF)
#define CR_STRT_Set              ((uint32_t)0x00000040)
#define CR_LOCK_Set              ((uint32_t)0x00000080)
/* Delay definition */
#define EraseTimeout             ((uint32_t)0x00000FFF)
#define ProgramTimeout           ((uint32_t)0x0000000F)

/** @defgroup FLASH_Flags
* @{
*/
/* FLASH Busy flag */
#define FLASH_FLAG_BSY           ((uint32_t)0x00000001)
/* FLASH End of Operation flag */
#define FLASH_FLAG_EOP           ((uint32_t)0x00000020)
/* FLASH Program error flag */
#define FLASH_FLAG_PGERR         ((uint32_t)0x00000004)
/* FLASH Write protected error flag */
#define FLASH_FLAG_WRPRTERR      ((uint32_t)0x00000010)
/* FLASH Option Byte error flag */
#define FLASH_FLAG_OPTERR        ((uint32_t)0x00000001)

/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/

/* Private functions --------------------------------------------------------*/

/******************************************************************************
 * @brief  Inserts a time delay.
 * @param  None
 * @retval : None
 * @attention  None
******************************************************************************/
static void delay(void)
{
    __IO uint32_t i = 0;
    for (i = 0xFF; i != 0; i--) {
    }
}

/******************************************************************************
 * @brief  Returns the FLASH Status.
 * @param  None
 * @retval : FLASH Status: The returned value can be: FLASH_BUSY,
 *   FLASH_ERROR_PG, FLASH_ERROR_WRP or FLASH_COMPLETE
 * @attention  None
******************************************************************************/
static FLASH_Status FLASH_GetStatus(void)
{
    FLASH_Status flashstatus = FLASH_COMPLETE;

    if ((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY) {
        flashstatus = FLASH_BUSY;
    } else {
        if (FLASH->SR & FLASH_FLAG_PGERR) {
            flashstatus = FLASH_ERROR_PG;
        } else {
            if (FLASH->SR & FLASH_FLAG_WRPRTERR) {
                flashstatus = FLASH_ERROR_WRP;
            } else {
                flashstatus = FLASH_COMPLETE;
            }
        }
    }
    /* Return the Flash Status */
    return flashstatus;
}

/******************************************************************************
 * @brief  Waits for a Flash operation to complete or a TIMEOUT to occur.
 * @param Timeout: FLASH progamming Timeout
 * @retval : FLASH Status: The returned value can be: FLASH_BUSY,
 *   FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or
 *   FLASH_TIMEOUT.
 * @attention  None
******************************************************************************/
static FLASH_Status FLASH_WaitForLastOperation(uint32_t Timeout)
{
    FLASH_Status status = FLASH_COMPLETE;

    /* Check for the Flash Status */
    status = FLASH_GetStatus();
    /* Wait for a Flash operation to complete or a TIMEOUT to occur */
    while ((status == FLASH_BUSY) && (Timeout != 0x00)) {
        delay();
        status = FLASH_GetStatus();
        Timeout--;
    }
    if (Timeout == 0x00) {
        status = FLASH_TIMEOUT;
    }

    /* Return the operation status */
    return status;
}

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
 * @brief  Programs a half word at a specified address.
 * @param Address: specifies the address to be programmed.
 * @param Data: specifies the data to be programmed.
 * @retval : FLASH Status: The returned value can be: FLASH_BUSY,
 *   FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or
 *   FLASH_TIMEOUT.
 * @attention  None
******************************************************************************/
FLASH_Status FLASH_ProgramHalfWord(uint32_t Address, uint16_t Data)
{
    FLASH_Status status = FLASH_COMPLETE;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(ProgramTimeout);

    if (status == FLASH_COMPLETE)
    {
    /*if the previous operation is completed, proceed to program the new data */
        FLASH->CR |= CR_PG_Set;

        *(__IO uint16_t *)Address = Data;
        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation(ProgramTimeout);
        if (status != FLASH_BUSY)
        {
            /* if the program operation is completed, disable the PG Bit */
            FLASH->CR &= CR_PG_Reset;
        }
    }

    /* Return the Program Status */
    return status;
}

/**
* @brief  Programs a word at a specified address.
* @param Address: specifies the address to be programmed.
* @param Data: specifies the data to be programmed.
* @retval : FLASH Status: The returned value can be: FLASH_BUSY,
*   FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or
*   FLASH_TIMEOUT.
*/
FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data)
{
    FLASH_Status status = FLASH_COMPLETE;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(ProgramTimeout);

    if(status == FLASH_COMPLETE)
    {
     /* if the previous operation is completed, proceed to program the new first
        half word */
        FLASH->CR |= CR_PG_Set;

        *(__IO uint16_t*)Address = (uint16_t)Data;
        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation(ProgramTimeout);

        if(status == FLASH_COMPLETE)
        {
   /* if the previous operation is completed, proceed to program the new second
            half word */
            *(__IO uint16_t*)(Address + 2) = Data >> 16;

            /* Wait for last operation to be completed */
            status = FLASH_WaitForLastOperation(ProgramTimeout);

            if(status != FLASH_BUSY)
            {
                /* Disable the PG Bit */
                FLASH->CR &= CR_PG_Reset;
            }
        }
        else
        {
            if (status != FLASH_BUSY)
            {
                /* Disable the PG Bit */
                FLASH->CR &= CR_PG_Reset;
            }
        }
    }


    /* Return the Program Status */
    return status;
}

/******************************************************************************
 * @brief  Unlocks the FLASH Program Erase Controller.
 * @param  None
 * @retval : None
 * @attention  None
******************************************************************************/
void FLASH_Unlock(void)
{
    /* Authorize the FPEC Access */
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;
}

/******************************************************************************
 * @brief  Locks the FLASH Program Erase Controller.
 * @param  None
 * @retval : None
 * @attention  None
******************************************************************************/
void FLASH_Lock(void)
{
    /* Set the Lock Bit to lock the FPEC and the FCR */
    FLASH->CR |= CR_LOCK_Set;
}

/******************************************************************************
 * @brief  Erases a specified FLASH page.
 * @param Page_Address: The page address to be erased.
 * @retval : FLASH Status: The returned value can be: FLASH_BUSY,
 *   FLASH_ERROR_PG, FLASH_ERROR_WRP, FLASH_COMPLETE or
 *   FLASH_TIMEOUT.
 * @attention  None
******************************************************************************/
FLASH_Status FLASH_ErasePage(uint32_t Page_Address)
{
    FLASH_Status status = FLASH_COMPLETE;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(EraseTimeout);

    if (status == FLASH_COMPLETE)
    {
        /* if the previous operation is completed, proceed to erase the page */
        FLASH->CR |= CR_PER_Set;
        FLASH->AR = Page_Address;
        FLASH->CR |= CR_STRT_Set;

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation(EraseTimeout);
        if (status != FLASH_BUSY)
        {
            /* if the erase operation is completed, disable the PER Bit */
            FLASH->CR &= CR_PER_Reset;
        }
    }

    /* Return the Erase Status */
    return status;
}


/******************************************************************************
 * @brief  Application code writing into the user flash
 * @param  wAppxAddr , pchAppBuf , wAppSize(Byte)
 * @retval None
 * @attention  None
******************************************************************************/
void Iap_WriteAppBin(uint32_t wAppAddr, uint8_t *pchAppBuf, uint32_t wAppSize)
{
#if 0
    /*  Calculate pages to erase(actually num - 1)  */
    uint8_t chEraseNumber = wAppSize / FLASH_SECTOR_SIZE ;
    if (NULL != pchAppBuf)
    {
        FLASH_Unlock();
        do    /* Reversely erasing app code using address page */
        {
            FLASH_ErasePage(wAppxAddr + chEraseNumber * FLASH_SECTOR_SIZE);
            if (0 == chEraseNumber)
            {
                break;
            }
            chEraseNumber --;
        }
        while (1);
        Flash_Write_NoCheck(wAppxAddr, (uint16_t *)pchAppBuf, wAppSize / 2);
        FLASH_Lock();
    }
#else
    if (NULL != pchAppBuf)
    {
        FLASH_Unlock();
        Flash_Write_NoCheck(wAppAddr, (uint16_t *)pchAppBuf, wAppSize / 2);
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
    uint32_t FlashAddress;

    FlashAddress = StartSector;

    while (FlashAddress <= (uint32_t) USER_FLASH_LAST_PAGE_ADDRESS)
    {
        if (FLASH_ErasePage(FlashAddress) == FLASH_COMPLETE)
        {
            //if(*(uint32_t*)FlashAddress==0xFFFFFFFF)
            FlashAddress += FLASH_PAGE_SIZE;
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
 * @param  FileSize: size of data file
 * @retval 0: Data successfully written to Flash memory
 *         1: Error occurred while writing data in Flash memory
 *         2: Written Data in flash memory is different from expected one
 * @attention  None
******************************************************************************/
uint32_t FLASH_If_Write(__IO uint32_t *FlashAddress, uint32_t *Data, uint16_t  \
DataLength, uint32_t FileSize)
{
    uint32_t i = 0;
    static uint32_t Total = 0;

    FLASH_Unlock();
    for (i = 0; (i < DataLength) && (*FlashAddress <= (USER_FLASH_END_ADDRESS- \
    4)); i++, Total++)
    {

        if (Total * 4 >= FileSize)
        {
            Total = 0;
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

/******************************************************************************
 * @brief  This function writes a data buffer in flash(data are 32-bit aligned).
 * @note   After writing data buffer, the flash content is checked.
 * @param  Address: start address for writing data buffer
 * @param  Data: pointer on data buffer
 * @param  DataNum: length of data buffer (unit is 32-bit word)
 * @retval 0: Data successfully written to Flash memory
 *         1: Error occurred while writing data in Flash memory
 *         2: Written Data in flash memory is different from expected one
 * @attention  None
******************************************************************************/
FLASH_Status ProgramDatatoFlash(uint32_t *Address, uint16_t *Data,            \
    uint32_t DataNum)
{
    FLASH_Status FLASHStatus = FLASH_COMPLETE;
    uint32_t i;
    uint16_t *pData = Data;

    FLASH_Unlock();

    for (i = 0; i < (DataNum >> 1); i++)
    {
        FLASHStatus = FLASH_ProgramHalfWord(*Address, *pData);
        if (FLASHStatus != FLASH_COMPLETE)
        {
            return  FLASHStatus;
        }
        *Address += 2;
        pData++;
    }
    return  FLASHStatus;

}

/******************* (C) COPYRIGHT 2020 ***************************/

