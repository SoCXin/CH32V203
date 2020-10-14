/******************************************************************************
 * @file    ymodem.c
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
#define __YMODEM_C__


/* Includes ------------------------------------------------------------------*/
#include    "config.h"
#include    "main.h"
#include    "flash.h"
#include    "tim3.h"
#include    "uart.h"
#include    "string.h"
#include    <stdio.h>
#include    "check.h"

#if defined (YMODEM_EN) || defined (YMODEM1K_EN)
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static uint8_t Tab_1K[1024] = {0};
static uint8_t FileName[FILE_NAME_LENGTH];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************
 * @brief  Test to see if a key has been pressed on the HyperTerminal
 * @param  key: The key pressed
 * @retval 1: Correct
 *         0: Error
 * @attention  None
******************************************************************************/
static uint32_t SerialKeyPressed(uint8_t *key)
{
    if ((IAP_UART->CSR & 0x0002) != (uint16_t)RESET)
    {
        *key = (uint16_t)(IAP_UART->RDR & (uint16_t)0x00FF);
        return 1;
    }
    else
    {
        return 0;
    }
}


/******************************************************************************
 * @brief  Receive byte from sender
 * @param  c: Character
 * @param  timeout: Timeout
 * @retval 0: Byte received
 *         -1: Timeout
 * @attention  None
******************************************************************************/
static  int32_t Receive_Byte(uint8_t *c, uint32_t TimeOut)
{
    while (TimeOut-- > 0)
    {
        if (SerialKeyPressed(c) == 1)
        {
            return 0;
        }
    }
    return -1;
}


/******************************************************************************
 * @brief  Receive a packet from sender
 * @param  data
 * @param  length
 * @param  timeout
 *          0: end of transmission
 *          -1: abort by sender
 *          >0: packet length
 * @retval 0: normally return
 *         -1: timeout or packet error
 *         1: abort by user
 * @attention  None
******************************************************************************/
static int32_t Receive_Packet(uint8_t *Data, int32_t *Length, uint32_t TimeOut)
{
    uint16_t i ;
    uint8_t  c ;
    uint16_t PacketSize  ;
    uint16_t ComputedCRC ;
    
    
    *Length = 0;
    if (Receive_Byte(&c, TimeOut) != 0)
    {
        return -1;
    }

    switch (c)
    {
    case SOH:
        PacketSize = PACKET_SIZE;
        break;
    case STX:
        PacketSize = PACKET_1K_SIZE;
        break;
    case EOT:
        return 0;
    case CA:
        if ((Receive_Byte(&c, TimeOut) == 0) && (c == CA))
        {
            *Length = -1;
            return 0;
        }
        else
        {
            return -1;
        }
    case ABORT1:
    case ABORT2:
        return 1;
    default:
        return -1;
    }
    *Data = c;
    for (i = 1; i < (PacketSize + PACKET_OVERHEAD); i ++)
    {
        if (Receive_Byte(Data + i, TimeOut) != 0)
        {
            return -1;
        }
    }
    /* Check whether the number complement is correct */
    if (Data[PACKET_SEQNO_INDEX] != ((Data[PACKET_SEQNO_COMP_INDEX] ^ 0xff)    \
    & 0xff))
    {
        return -1;
    }

    /* Compute the CRC */
    ComputedCRC = Cal_CRC16(&Data[PACKET_HEADER], (uint32_t)PacketSize);
    /* Check that received CRC match the already computed CRC value
       Data[PacketSize+3]<<8) | Data[PacketSize+4] contains the received CRC
       ComputedCRC contains the computed CRC value */
    if (ComputedCRC != (uint16_t)((Data[PacketSize + 3] << 8) |                \
    Data[PacketSize + 4]))
    {
        /* CRC error */
        return -1;
    }

    *Length = PacketSize;
    return 0;
}

/******************************************************************************
 * @brief  Download a file using the ymodem protocol
 * @param  Buf: Address of the first byte
 * @retval The size of the file
 * @attention  None
******************************************************************************/
static int32_t Ymodem_Download(uint8_t *Buf)
{
    uint8_t PacketData[PACKET_1K_SIZE + PACKET_OVERHEAD];
    uint8_t FileSize[FILE_SIZE_LENGTH];
    uint8_t *pFilePtr;
    uint8_t *pBufPtr ;
    int32_t PacketLength, PacketsReceived, Errors, SessionBegin, Size = 0;
    int32_t SessionDone, FileDone ;
    uint32_t FlashDestination, RamSource;
    int32_t i;
    
    /* Initialize flashdestination variable */
    FlashDestination = APPLICATION_ADDRESS;

    for (SessionDone = 0, Errors = 0, SessionBegin = 0; ;)
    {
        for (PacketsReceived = 0, FileDone = 0, pBufPtr = Buf; ;)
        {

            switch (Receive_Packet(PacketData, &PacketLength, NAK_TIMEOUT))
            {
            case 0:  /* 1K data received successfully */
                Errors = 0;
                switch (PacketLength)
                {
                /* Abort by sender */
                case - 1:
                    UARTx_WriteByte(IAP_UART, ACK);
                    return 0;
                /* End of transmission */
                case 0:
                    UARTx_WriteByte(IAP_UART, ACK);
                    FileDone = 1;
                    break;
                /* Normal packet */
                default:
                    if ((PacketData[PACKET_SEQNO_INDEX] & 0xff) !=             \
                    (PacketsReceived & 0xff))
                    {
                        UARTx_WriteByte(IAP_UART, NAK);
                    }
                    else
                    {  /* File name (first package) */
                        if (PacketsReceived == 0)  
                        {
                            /* Filename packet */
                            if (PacketData[PACKET_HEADER] != 0)/* File name */
                            {
                                /* Filename packet has valid data */
                                for (i = 0, pFilePtr = PacketData +            \
                                PACKET_HEADER; (*pFilePtr != 0) &&             \
                                (i < FILE_NAME_LENGTH);)
                                {  /* Save file name */
                                    FileName[i++] = *pFilePtr++;  
                                }
                                FileName[i++] = '\0';
                                for (i = 0, pFilePtr ++; (*pFilePtr != ' ') && \
                                (i < (FILE_SIZE_LENGTH - 1));)
                                {  /* File size */
                                    FileSize[i++] = *pFilePtr++;  
                                }
                                FileSize[i++] = '\0';
                                /* Convert a string to an integer */
                                Str2Int(FileSize, &Size);  

                                /* Test the size of the image to be sent */
                                /* Image size is greater than Flash size */
                                if (Size > (USER_FLASH_SIZE + 1))
                                {
                                    /* End session */
                                    UARTx_WriteByte(IAP_UART, CA);
                                    UARTx_WriteByte(IAP_UART, CA);
                                    return -1;
                                }
                                /* erase user application area */
                                FLASH_Unlock();
                                FLASH_If_Erase(APPLICATION_ADDRESS);
                                FLASH_Lock();

                                UARTx_WriteByte(IAP_UART, ACK);
                                UARTx_WriteByte(IAP_UART, CRC16);
                            }
                            /* Filename packet is empty, end session */
                            else
                            {
                                UARTx_WriteByte(IAP_UART, ACK);
                                FileDone = 1;
                                SessionDone = 1;
                                break;
                            }
                        }
                        /* Data packet */
                        else
                        {
                        /* Start receiving data after saving file information */
                            memcpy(pBufPtr, PacketData + PACKET_HEADER,        \
                            PacketLength);
                            RamSource = (uint32_t)Buf;
                            /* Write received data in Flash */
                            if (FLASH_If_Write(&FlashDestination,              \
                            (uint32_t *)RamSource, (uint16_t)PacketLength/4,  \
                            Size)  == 0)
                            {
                                UARTx_WriteByte(IAP_UART, ACK);
                            }
                            else 
                            {/*An error occurred while writing to Flash memory*/
                                /* End session */
                                UARTx_WriteByte(IAP_UART, CA);
                                UARTx_WriteByte(IAP_UART, CA);
                                return -2;
                            }
                        }
                        PacketsReceived ++;
                        SessionBegin = 1;
                    }
                }
                break;
            case 1:
                UARTx_WriteByte(IAP_UART, CA);
                UARTx_WriteByte(IAP_UART, CA);
                return -3;
            default:
                if (SessionBegin > 0)
                {
                    Errors ++;
                }
                if (Errors > MAX_ERRORS)
                {
                    UARTx_WriteByte(IAP_UART, CA);
                    UARTx_WriteByte(IAP_UART, CA);
                    return 0;
                }
                UARTx_WriteByte(IAP_UART, CRC16);
                break;
            }

            if (FileDone != 0)
            {
                break;
            }
        }
        if (SessionDone != 0)
        {
            break;
        }
    }
    return (int32_t)Size;
}


/* Exported variables -------------------------------------------------------*/
/* Exported function prototypes ---------------------------------------------*/
/* Exported function --------------------------------------------------------*/

/******************************************************************************
 * @brief  Receive a file via serial port
 * @param  None
 * @retval None
 * @attention  None
******************************************************************************/
uint8_t Ymodem_Receive(void)
{
    #if DEBUG_EN
    uint8_t Number[10] = {0};
    #endif
    int32_t Size = 0;   
    
    /* Start receiving the file, the file size will be returned after success */
    Size = Ymodem_Download(&Tab_1K[0]);

    if (Size > 0)  /* receive successfully */
    {
        AppRxSize = Size ;
        #if DEBUG_EN
        SerialPutString(IAP_UART,                                              \
        "\r\n Programming Completed Successfully!\n\r-------------\r\n Name: ");        
        SerialPutString(IAP_UART, "\n\r->");
        SerialPutString(IAP_UART, FileName);
        Int2Str(Number, Size);
        SerialPutString(IAP_UART, "\n\r Size: ");
        SerialPutString(IAP_UART, Number);
        SerialPutString(IAP_UART, " Bytes\r\n");
        SerialPutString(IAP_UART, "-------------------\n");
        #endif
        return 0;
    }
    else if (Size == -1)
    {
        #if DEBUG_EN
        SerialPutString(IAP_UART,                                              \
        "\n\n\rThe image size is higher than the allowed space memory!\n\r");
        #endif
        return 1;
    }
    else if (Size == -2)
    {
        #if DEBUG_EN
        SerialPutString(IAP_UART, "\n\n\rVerification failed!\n\r");
        #endif
        return 2;
    }
    else if (Size == -3)
    {
        #if DEBUG_EN
        SerialPutString(IAP_UART, "\r\n\nAborted by user.\n\r");
        #endif
        return 3;
    }
    else
    {
        #if DEBUG_EN
        SerialPutString(IAP_UART, "\n\rFailed to receive the file!\n\r");
        #endif
        return 4;
    }
}


#endif

/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

