/******************************************************************************
 * @file    queue.c
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
#define __QUEUE_C__


/* Includes -----------------------------------------------------------------*/
#include "queue.h"
#include "config.h"
#include <stdio.h>
#include <string.h>

#if defined (BINARY_EN)

/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/


/* Exported function --------------------------------------------------------*/

/******************************************************************************
 * @brief  Create queue
 * @param  
 * @retval 
 * @attention  
******************************************************************************/
uint32_t Queue_PacketCreate(QUEUE8_t *pQ8, uint8_t *pBuf, uint32_t BufSize)
{    
    pQ8->BufSize    = BufSize;
    pQ8->pBuf       = pBuf;
    pQ8->pStart     = pBuf;
    pQ8->pEnd       = pBuf;

    return 0;
}

/******************************************************************************
 * @brief  Write data into queue
 * @param  
 * @retval 
 * @attention  
******************************************************************************/
uint32_t Queue_PacketIn(QUEUE8_t *pQ8, uint8_t *pData, uint32_t Len)
{
    volatile uint8_t    *pEnd   = NULL;  
    volatile uint32_t    Index   = 0;    

    pEnd = pQ8->pEnd;

    for (Index = 0; Index < Len; Index++)
    {
        if (++pEnd >= pQ8->pBuf + pQ8->BufSize)
        {
            pEnd = pQ8->pBuf;
        }

        if (pEnd == pQ8->pStart)
        {
            break;
        }

        *pQ8->pEnd = *pData++;
        pQ8->pEnd = pEnd;
    }

    return Index;
}

/******************************************************************************
 * @brief  Read data out from queue
 * @param  
 * @retval 
 * @attention  
******************************************************************************/
uint32_t Queue_PacketOut(QUEUE8_t *pQ8, uint8_t *pData, uint32_t DataLen)
{
    volatile uint8_t    *pStart   = NULL;  
    volatile uint32_t Index = 0;

    pStart = pQ8->pStart;

    while ((pStart != pQ8->pEnd) && (Index < DataLen) && (Index < pQ8->BufSize))
    {
        pData[Index++] = *pStart++;
        if (pStart >= pQ8->pBuf + pQ8->BufSize)
        {
            pStart = pQ8->pBuf;
        }
    }

    pQ8->pStart = pStart;

    return Index;
}

/******************************************************************************
 * @brief  Get data length in the queue
 * @param  
 * @retval 
 * @attention  
******************************************************************************/
uint32_t Queue_PacketLengthGet(QUEUE8_t *pQ8)
{
    volatile uint8_t    *pStart     = NULL;
    uint32_t            Index       = 0;

    pStart = pQ8->pStart;

    while ((pStart != pQ8->pEnd) && (Index < pQ8->BufSize))
    {
        Index++;
        if (++pStart >= pQ8->pBuf + pQ8->BufSize)
        {
            pStart = pQ8->pBuf;
        }
    }

    return Index;
}

#endif

/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/
