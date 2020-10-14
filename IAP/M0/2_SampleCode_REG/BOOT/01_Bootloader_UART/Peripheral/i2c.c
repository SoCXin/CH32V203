/******************************************************************************
 * @file    i2c.c
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
#define __I2C_C__


/* Includes -----------------------------------------------------------------*/
#include "i2c.h"


/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/

/* Private functions --------------------------------------------------------*/
static void Flash_Write_NoCheck(uint32_t wWriteAddr, uint16_t *phwBuffer, uint16_t hwNumber)
{
    uint16_t i;
    if (NULL == phwBuffer) {
        return;
    }
    for (i = 0; i < hwNumber; i++) {
        FLASH_ProgramHalfWord(wWriteAddr, phwBuffer[i]);
        wWriteAddr += 2;                                    //地址增加2.
    } 
}


/* Exported variables -------------------------------------------------------*/
/* Exported function prototypes ---------------------------------------------*/


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
//应用程序代码写入
//wAppxAddr:应用程序的起始地址
//pchAppBuf:应用程序CODE.
//wAppSize:应用程序大小(字节).
void Iap_WriteAppBin(uint32_t wAppxAddr, uint8_t *pchAppBuf, uint32_t wAppSize)
{
//    uint8_t chEraseNumber = wAppSize / FLASH_SECTOR_SIZE - 1 ;   //计算需要擦除页（实际页数 - 1）
    if (NULL != pchAppBuf) {
        FLASH_Unlock();                                         //解锁
//        do {
//            FLASH_ErasePage(wAppxAddr + chEraseNumber * FLASH_SECTOR_SIZE);     //擦除APP代码使用地址页，倒序擦除
//            if (0 == chEraseNumber) {
//                break;
//            }
//            chEraseNumber --;
//        }while(1);
        Flash_Write_NoCheck(wAppxAddr, (uint16_t *)pchAppBuf, wAppSize / 2);    //写入APP代码
        FLASH_Lock();                                           //上锁
    }
}

/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void I2C_ReleaseBus(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable GPIOB Clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

    /* Configure I2C GPIO In Input Mode , Release I2C Bus Control */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_20MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void I2C_Configure(uint8_t Addr)
{
    I2C_InitTypeDef I2C_InitStructure;

    /* Release I2C Bus */
    I2C_ReleaseBus();

    /* Enable I2C1 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    /* I2C1 Configuration : Master Mode At 100kHz */
    I2C_StructInit(&I2C_InitStructure);
    I2C_InitStructure.Mode          = I2C_Mode_MASTER;
    I2C_InitStructure.OwnAddress    = 0;
    I2C_InitStructure.Speed         = I2C_Speed_STANDARD;
    I2C_InitStructure.ClockSpeed    = 100000;
    I2C_Init(I2C1, &I2C_InitStructure);

    /* Set Slave Address */
    I2C_Send7bitAddress(I2C1, Addr, I2C_Direction_Transmitter);

    /* Enable I2C1 */
    I2C_Cmd(I2C1, ENABLE);

    /* Configure I2C1 GPIO */
    I2C_GPIO_Configure();  
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void I2C_ReadBytes(uint8_t Addr, uint8_t *pBuf, uint8_t Len)
{
    uint8_t i = 0, Flg = 0, Cnt = 0;

    /* Send 24C02 Memory Address */
    I2C_SendData(I2C1, Addr);
    while(!I2C_GetFlagStatus(I2C1, I2C_STATUS_FLAG_TFE));

    for(i = 0; i < Len; i++)
    {
        while(1)
        {
            /* Write command is sent when RX FIFO is not full */
            if((I2C_GetFlagStatus(I2C1, I2C_STATUS_FLAG_TFNF)) && (Flg == 0))
            {
                I2C_ReadCmd(I2C1);

                Cnt++;

                if(Cnt == Len)
                {
                    Flg = 1;
                }
            }

            /* Check receive FIFO not empty */
            if(I2C_GetFlagStatus(I2C1, I2C_STATUS_FLAG_RFNE))
            {
                pBuf[i] = I2C_ReceiveData(I2C1); break;
            }
        }
    }

    /* Generate STOP Single */
    I2C_GenerateSTOP(I2C1, ENABLE);
    while(!I2C_GetITStatus(I2C1, I2C_IT_STOP_DET));
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void I2C_WriteBytes(uint8_t Addr, uint8_t *pBuf, uint8_t Len)
{
    uint8_t i = 0;

    /* Send 24C02 Memory Address */
    I2C_SendData(I2C1, Addr);
    while(!I2C_GetFlagStatus(I2C1, I2C_STATUS_FLAG_TFE));

    /* Write Data */
    for(i = 0; i < Len; i++)
    {
        I2C_SendData(I2C1, *pBuf++);
        while(!I2C_GetFlagStatus(I2C1, I2C_STATUS_FLAG_TFE));
    }

    /* Generate STOP Single */
    I2C_GenerateSTOP(I2C1, ENABLE);
    while(!I2C_GetITStatus(I2C1, I2C_IT_STOP_DET));
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

