/******************************************************************************
 * @file    bsp_eeprom.c
 * @author  King
 * @version V1.00
 * @date    20-May-2020
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
#define __BSP_EEPROM_C__


/* Includes -----------------------------------------------------------------*/
#include "bsp_eeprom.h"
#include "i2c.h"
#include "systick.h"


/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/


/* Exported variables -------------------------------------------------------*/
/* Exported function prototypes ---------------------------------------------*/


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void BSP_EEPROM_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable EEPROM_WP GPIO Clock */
    RCC_AHBPeriphClockCmd(EEPROM_WP_RCC, ENABLE);

    /* EEPROM_WP Pin Configuration */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = EEPROM_WP_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(EEPROM_WP_PORT, &GPIO_InitStructure);

    /* EEPROM I2C Configuration */
    I2C_Configure(EEPROM_I2C_ADDRESS);
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void BSP_EEPROM_WP_SetHigh(void)
{
    GPIO_WriteBit(EEPROM_WP_PORT, EEPROM_WP_PIN, Bit_SET);
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void BSP_EEPROM_WP_SetLow(void)
{
    GPIO_WriteBit(EEPROM_WP_PORT, EEPROM_WP_PIN, Bit_RESET);
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void BSP_EEPROM_ReadBytes(uint8_t Addr, uint8_t *pBuf, uint8_t Len)
{
    I2C_ReadBytes(Addr, pBuf, Len);
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void BSP_EEPROM_WriteBytes(uint8_t Addr, uint8_t *pBuf, uint8_t Len)
{
    uint8_t Start, StartCnt, PageNum, FinalCnt;

    if((Addr % EEPROM_PAGE_SIZE) == 0)
    {
        StartCnt = 0;
        PageNum  = Len / EEPROM_PAGE_SIZE;
        FinalCnt = Len % EEPROM_PAGE_SIZE;
    }
    else
    {
        Start = Addr % EEPROM_PAGE_SIZE;

        if(((Start + Len) / EEPROM_PAGE_SIZE) == 0)
        {
            StartCnt = Len;
            PageNum  = 0;
            FinalCnt = 0;
        }
        else
        {
            StartCnt = EEPROM_PAGE_SIZE - Start;
            PageNum  = (Len - StartCnt) / EEPROM_PAGE_SIZE;
            FinalCnt = (Len - StartCnt) % EEPROM_PAGE_SIZE;
        }
    }

    BSP_EEPROM_WP_SetLow();

    if(StartCnt)
    {
        I2C_WriteBytes(Addr, pBuf, StartCnt);

        Addr += StartCnt;
        pBuf += StartCnt;

        SysTick_DelayMs(50);
    }

    while(PageNum--)
    {
        I2C_WriteBytes(Addr, pBuf, EEPROM_PAGE_SIZE);

        Addr += EEPROM_PAGE_SIZE;
        pBuf += EEPROM_PAGE_SIZE;

        SysTick_DelayMs(50);
    }

    if(FinalCnt)
    {
        I2C_WriteBytes(Addr, pBuf, FinalCnt);
    }

    BSP_EEPROM_WP_SetHigh();
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void BSP_EEPROM_Entry(void)
{
    uint8_t ReadBuf[20], WriteBuf[20];
    uint8_t i = 0, Addr = 0;

    /* Init ReadBuffer & WriteBuffer */
    for(i = 0; i < 20; i++)
    {
        ReadBuf[i]  = 0;
        WriteBuf[i] = i;
    }

    printf("\r\n\r\nEEPROM Write : ");

    /* Write Data To EEPROM At Start Address */
    BSP_EEPROM_WriteBytes(Addr, WriteBuf, 20);

    printf("OK\r\n");

    printf("\r\nEEPROM Read : \r\n");

    /* Read EEPROM Data To ReadBuffer From Start Address */
    BSP_EEPROM_ReadBytes(Addr, ReadBuf, 20);

    for(i = 0; i < 20; i++)
    {
        printf("0x%02x ", ReadBuf[i]);

        if(((i + 1) % 10) == 0)
        {
            printf("\r\n");
        }
    }

    printf("\r\n\r\n");
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

