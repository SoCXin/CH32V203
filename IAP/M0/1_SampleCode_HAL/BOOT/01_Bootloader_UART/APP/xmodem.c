/******************************************************************************
 * @file    xmodem.c
 * @author  MM32 AE
 * @version V1.01
 * @date    24-June-2020
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
#define __XMODEM_C__


/* Includes -----------------------------------------------------------------*/
#include    "config.h"
#include    "main.h"
#include    "flash.h"
#include    "tim3.h"
#include    "uart.h"
#include    "string.h"
#include    <stdio.h>
#include    "check.h"

#if defined (XMODEM_EN) || defined (XMODEM1K_EN)

#if defined (XMODEM_EN)
    static uint8_t BinDataTemp[153];  /* Receive buffer */
#elif defined (XMODEM1K_EN)
    static uint8_t BinDataTemp[2530];  /* Receive buffer */
#endif

/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
static FLASH_Status FLASHStatus = FLASH_COMPLETE ;  /* Flash operation status*/
static uint8_t EraseCounter = 0 ;  /* Erased flash page counter */
static uint8_t CheckTypeTemp ='C';/*Verification method of received documents*/
static uint8_t FillingDataCount = 0 ;   /* The populated data 0x1A counters */

/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/

/******************************************************************************
 * @brief  If the CRC is true, it is CRC check; otherwise, it is the check sum
 * @param  crc , buf ,  sz
 * @retval If there is no error, return to true, otherwise return to false
 * @attention  None
******************************************************************************/
static int Packet_Check(uint8_t BoolCRC, const uint8_t *Buf, int Size)
{
    uint16_t CalCRC16 ;
    uint16_t BufCRC ;
    if (BoolCRC)
    {
        CalCRC16 = Cal_CRC16(Buf, Size);
        BufCRC = (Buf[Size] << 8) + Buf[Size + 1];
        if (CalCRC16 == BufCRC)
            return TRUE;
    }
    else
    {
        if (CalChecksum(Buf, Size) == *(Buf + Size))
            return TRUE;
    }
    return FALSE;
}

/* Exported variables -------------------------------------------------------*/
/* Exported function prototypes ---------------------------------------------*/
/* Exported function --------------------------------------------------------*/

/******************************************************************************
 * @brief  XMODEM protocol receives received files
 * @param  CheckType： ‘C’- CRC ; NAK - Check sum
 * @retval the result of transmission
 * @attention  None
******************************************************************************/
uint8_t Xmodem_Receive(uint8_t CheckType)
{
#if defined (XMODEM_EN)
    uint8_t XmodemBuff[133]; /* 128 for XModem  + 3 head chars + 2 crc + nul */
    uint8_t BufLen = 128;
#elif defined (XMODEM1K_EN)
    uint8_t XmodemBuff[1029];/* 128 for XModem1K  + 3 head chars + 2 crc + nul*/
    uint16_t BufLen = 1024;
#endif
    uint8_t TempCRC = 1;  /* Use CRC check */
    uint8_t PacketNO = 1;
    uint8_t PacketNoTimes = 0;
    uint32_t PageOffset = 0 ;  
/*  uint8_t ErrorCode = 0;  */
    uint8_t *pLastData;
    uint8_t chTemp = 0;  /* Char rx buffer */
    uint8_t ucIsLastPac = 0 ;
    uint16_t i = 0;
    uint16_t j = 0;
    
    pLastData = BinDataTemp;
    
    if (CheckType == CHECK_CRC) 
    {
        TempCRC = 1;
        CheckTypeTemp = 'C';
    }
    else if (CheckType == CHECK_SUM)
    {
        TempCRC = 0;
        CheckTypeTemp = NAK;
    }
    while (1)
    {
        SerialPutString(IAP_UART, "\n\r->");       
        for (i = CONNECT_TIMES; i != 0; i--) 
        {   /*   500 * 0.5ms  */
            chTemp = UARTx_ReadByte(IAP_UART, DLY_1S * 2);  
            if (chTemp > 0)
            {
                TIM_Cmd(TIM3,DISABLE);  /* Disable the TIM Counter */
                TIM_SetCounter(TIM3,0);  /* Clear the tim3 counter */
                break;
            }
            else
            {
                UARTx_WriteByte(IAP_UART, CheckTypeTemp);
            }
        }
        /* File transfer timeout */
#if defined (XMODEM_EN)
        if ((chTemp != SOH) && (i == 0))
#elif defined (XMODEM1K_EN)
        if ((chTemp != STX) && (i == 0))
#endif
//        if (((chTemp != SOH) || (chTemp != STX)) && (i == 0))
        {
            #if DEBUG_EN
            SerialPutString(IAP_UART, "\n\r->Transmit Time Out!!");
            SerialPutString(IAP_UART, "\n\r->");
            #endif
            return TIME_OUT;
        }
        /* User cancels file transfer */
        else if ((chTemp == 'B') || (chTemp == 'b'))
        {
            #if DEBUG_EN
            SerialPutString(IAP_UART, "\n\r->User Canceled!!");
            SerialPutString(IAP_UART, "\n\r->");
            #endif
            return USER_CANCELED;
        }
        else  
        {  /* Start file transfer */
#if defined (XMODEM_EN)
            while (chTemp == SOH)  /* Valid data frame header received */
#elif defined (XMODEM1K_EN)
            while (chTemp == STX)  /* Valid data frame header received */
#endif
            {
                XmodemBuff[0] = chTemp;
                for (i = 0; i < sizeof(XmodemBuff); i++)  
                {  /* Received one frame of data */
                    XmodemBuff[i + 1] = UARTx_ReadByte(IAP_UART, DLY_1MS);  
                }

                if ((XmodemBuff[1] == (uint8_t)~XmodemBuff[2]) && (PacketNO == \
                    XmodemBuff[1]) 
                        && (Packet_Check(TempCRC, &XmodemBuff[3], BufLen)))  
                {  /* Package No. and CRC is correct */                  
                    if (FLASHStatus == FLASH_COMPLETE)
                    {
#if defined (XMODEM_EN)
                        if ((PacketNO - 1) % 8 == 0) 
#elif defined (XMODEM1K_EN)
                        if ((PacketNO - 1) % 1 == 0) //8
#endif
                        {
                            FLASH_Unlock();
                            FLASHStatus = FLASH_ErasePage(APPLICATION_ADDRESS+ \
                            (FLASH_PAGE_SIZE * EraseCounter));
                            FLASH_Lock();
                            EraseCounter++;
                        }
                    }
                    if(255 == PacketNO)  
                    {
                        PacketNoTimes +=1 ;
                    }
                    PacketNO++;
                    for (j = 0; (j < BufLen) ; j++)
                    {
                        *(pLastData++) = XmodemBuff[3 + j];
                        if((CTRLZ == XmodemBuff[5 + j]) &&(XmodemBuff[4 + j]== \
                        XmodemBuff[5 + j]) && (XmodemBuff[3 + j] ==            \
                        XmodemBuff[4 + j]))
                        {
                            FillingDataCount = (3 + j) ;
                            ucIsLastPac = 1 ;
                            break;
                        }             
                    }
                    if (ucIsLastPac)
                    {
                        Iap_WriteAppBin((APPLICATION_ADDRESS + PageOffset *  \
                        BufLen),  BinDataTemp,  FillingDataCount - 3) ; 
//                        pLastData = BinDataTemp;
                        AppRxSize = ((PacketNO+256*PacketNoTimes) - 2)*BufLen+ \
                        FillingDataCount-3;
                    }
                    else
                    {
                        Iap_WriteAppBin((APPLICATION_ADDRESS + PageOffset *  \
                        BufLen),  BinDataTemp,  BufLen) ;
                        pLastData = BinDataTemp;
                        PageOffset ++ ;
                    }
                    UARTx_WriteByte(IAP_UART, ACK);
                }
                else 
                {  /* Request resend */
                    UARTx_WriteByte(IAP_UART, NAK);
                }
                do
                {
                    /* Read the frame header of the next frame data */
                    chTemp = UARTx_ReadByte(IAP_UART, 10*DLY_1MS);
                }
                while (chTemp == 0);
            }
            UARTx_WriteByte(IAP_UART, chTemp);
            if (chTemp == EOT)  
            {  /* File sending end flag */
                UARTx_WriteByte(IAP_UART, ACK);  
                if(ucIsLastPac == 0)      
                {
                     AppRxSize = ((PacketNO+256*PacketNoTimes) - 2)*BufLen +  \
                        FillingDataCount-3;
                }               
            }

            return SUCCESSFULL;
        }
    }
}

#endif


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

