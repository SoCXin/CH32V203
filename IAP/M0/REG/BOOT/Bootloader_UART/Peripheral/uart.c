/******************************************************************************
 * @file    uart.c
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
#define __UART_C__


/* Includes -----------------------------------------------------------------*/
#include "uart.h"
#include "tim3.h"
#include "main.h"

/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/

/* Exported variables -------------------------------------------------------*/
/* Exported function prototypes ---------------------------------------------*/
/* Exported function --------------------------------------------------------*/

/******************************************************************************
 * @brief  uart sent byte
 * @param  UARTx : x can be 1 or 2
 * @param  c : uart sent byte
 * @retval None
 * @attention  None
******************************************************************************/
void UARTx_WriteByte(UART_TypeDef *UARTx,uint8_t c)
{
    UARTx->TDR = c;
    while(1) {
        if(UARTx->ISR & 0x1) {  /* wait  send  end */
            UARTx->ICR = 0x1;
            break;
        }
    }
}

/******************************************************************************
 * @brief  Print a string on the HyperTerminal
 * @param  UARTx : x can be 1 or 2
 * @param  s: The string to be printed
 * @retval None
 * @attention  None
******************************************************************************/
void UARTx_WriteBytes(UART_TypeDef *UARTx,uint8_t *s)
{
  while (*s != '\0')
  {
    UARTx_WriteByte(UARTx,*s);
    s++;
  }
}

/******************************************************************************
 * @brief  uart poll-receive function
 * @param  UARTx : x can be 1 or 2
 * @param  Time : timeout count
 * @retval None
 * @attention  None
******************************************************************************/
uint32_t UARTx_ReadByte(UART_TypeDef *UARTx,uint32_t Time)
{
    uint32_t ch = 0;
    TIM3->CNT = 0;
    TIM3->ARR = Time;
    TIM3->CR1 |= ((uint16_t)0x0001);	/* Enable the TIM3 Counter */
    while (g_recTimeOutFlag != UART_TIME_OUT)
    {
        if ((UARTx->ISR & 0x2))
        {
            ch = (uint8_t)UARTx->RDR;
            UARTx->ICR = 0x2;   /* Clear the ISR IT flag */
            break;
        }
    }
    TIM3->CR1 &= ((uint16_t)0xFFFE);  /* Disable the TIM Counter */
    TIM3->CNT = 0;
    g_recTimeOutFlag = 0;
    return (ch);
}

/******************************************************************************
 * @brief  uart initialization
 * @param  two
 * @retval  None
 * @attention  None
******************************************************************************/
void UARTx_Configure(UART_TypeDef *UARTx,uint32_t BaudRate)
{
	#ifndef  MM32F003_Q
    if(UARTx == UART1)
    {
        /* Enable UART1 clock */
        RCC->APB2ENR |= 1 << 14;

        /* UART1 Configuration as follows */
        RCC->APB2RSTR |= 1 << 14;  /* Reset uart1 clock */
        RCC->APB2RSTR &= ~(1 << 14);  /* Stop reset */

		UART1->GCR |= 0X18;  /* Enable tx and rx */
		UART1->CCR |= 0X30;  /* Uart format: n, 8, 1 */

		/* Determine the uart_baud*/
		UART1->BRR = (SystemCoreClock  / BaudRate) / (16);
		UART1->FRA = (uint8_t)(SystemCoreClock / BaudRate) % (16);

        /* Enable GPIOA and SYSCFG clock */
        RCC->AHBENR |= 1 << 17;
        RCC->APB2ENR |= 1 << 0;

        /* Enable PA9 & PA10 alternate function */
        GPIOA->AFRH &= (~0x0FF0);
        GPIOA->AFRH |= 0x110;

        /* Configure UART1 Tx as alternate function push-pull */
        /* Configure UART1 Rx as input floating */
        GPIOA->CRH &= 0XFFFFF00F;
        GPIOA->CRH |= 0X000008B0;

        /* Enable UART1 */
        UART1->GCR |= 0x0001;
    }
	#else
    if(UARTx == UART2)
    {
        /* Enable UART2 clock */
        RCC->APB1ENR |=(0x01U << 17);

        /* UART Configuration as follows */
        RCC->APB1RSTR |= 1 << 17;  /* Reset uart1 clock */
        RCC->APB1RSTR &= ~(1 << 17);  /* Stop reset */

		UART2->GCR |= 0X18;  /* Enable tx and rx */
		UART2->CCR |= 0X30;  /* Uart format: n, 8, 1 */
		/* Determine the uart_baud*/
		UART2->BRR = (SystemCoreClock  / BaudRate) / (16);
		UART2->FRA = (uint8_t)(SystemCoreClock / BaudRate) % (16);
        /* Enable GPIOA/B and SYSCFG clock */
		RCC->AHBENR |= 1 << 17;
		RCC->AHBENR |= 1 << 18;
        RCC->APB2ENR |= 1 << 0;

        /* Enable PA6 & PB7 alternate function */
		GPIOA->AFRL &= (~0x0F000000);
		GPIOA->AFRL |= (0xF3FFFFFF);
		GPIOB->AFRL &= (~0xF0000000);
		GPIOB->AFRL |= (0x4FFFFFFF);

        /* Configure UART2 Tx PB7 as alternate function push-pull */
        /* Configure UART2 Rx PA6 as input floating */
		GPIOB->CRL &= 0x0FFFFFFF;
        GPIOB->CRL |= 0xB0000000;
        GPIOA->CRL &= 0xF0FFFFFF;
        GPIOA->CRL |= 0x08000000;

		/* Enable UART2 */
        UART2->GCR |= 0x0001;
    }
	#endif
    else
    {

    }
}


/* Add the following code to support the printf function without selecting     \
use microlib */
#if defined (PRINTF_EN)
#pragma import(__use_no_semihosting)

/* functions required by standard library */
struct __FILE
{
	int handle;
	/* Whatever you require here. If the only file you are using is */
	/* standard output using printf() for debugging, no file handling */
	/* is required. */
};

/* FILE is typedef¡¯ d in stdio.h. */
FILE __stdout;

/* to avoid entering half host mode */
void _sys_exit(int x)
{
	x = x;
}

/******************************************************************************
 * @brief       Retargets the C library printf function to the UART
 * @param
 * @retval
 * @attention
******************************************************************************/
int fputc(int ch, FILE *f)
{
    /* Send a character to the UART */
    UART_SendData(PRINTF_UART, (uint8_t)ch);

     /* Loop until the end of transmission */
    while(!UART_GetFlagStatus(PRINTF_UART, UART_FLAG_TXEPT));

    return ch;
}

#endif

/******************* (C) COPYRIGHT 2020 ***************************/

