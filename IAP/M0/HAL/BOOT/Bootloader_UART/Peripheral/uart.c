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
 * @param  Data : uart sent byte 
 * @retval None     
 * @attention  None 
******************************************************************************/
void UARTx_WriteByte(UART_TypeDef *UARTx,uint8_t Data)
{ 
    /* send a character to the UART */
    UART_SendData(UARTx, Data);

     /* Loop until the end of transmission */
    while(UART_GetFlagStatus(UARTx, UART_FLAG_TXEPT) == RESET);
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
    TIM_SetCounter(TIM3,0);  /* Clear the tim3 counter */
    TIM_SetAutoreload(TIM3,Time);
    TIM_Cmd(TIM3,ENABLE);	/* Enable the TIM3 Counter */
    while (RecTimeOutFlag != UART_TIME_OUT)
    {
        if (UART_GetFlagStatus(UARTx, UART_FLAG_RXAVL) != RESET)
        {
            ch = UART_ReceiveData(UARTx);
            /* Clear the ISR IT flag */
            UART_ClearITPendingBit(UARTx,UART_IT_RXIEN);  
            break;
        }
    }
    TIM_Cmd(TIM3,DISABLE);  /* Disable the TIM Counter */
    TIM_SetCounter(TIM3,0);
    RecTimeOutFlag = 0;
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
    GPIO_InitTypeDef GPIO_InitStructure;
    UART_InitTypeDef UART_InitStructure;
    
	#ifndef  MM32F003_Q 
    if(UARTx == UART1)
    {
        /* Enable UART1 clock */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);

        /* UART1 Configuration as follows */     
        UART_DeInit(UARTx);        
		UART_StructInit(&UART_InitStructure);		
		UART_InitStructure.UART_BaudRate = BaudRate;                                  

        UART_Init(UART1, &UART_InitStructure);                                      
                                                      
        /* Enable GPIOA and SYSCFG clock */
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
		
        /* Enable PA9 & PA10 alternate function */
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

        /* Configure UART1 Tx(PA9) as alternate function push-pull */
        /* Configure UART1 Rx(PA10) as input floating */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                                   
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                             
        GPIO_Init(GPIOA, &GPIO_InitStructure); 
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                                  
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;                   
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        /* Enable UART1 */
        UART_Cmd(UART1, ENABLE);  
    }
    else if(UARTx == UART2)
    {
        /* Enable UART2 clock */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART2, ENABLE);

        /* UART Configuration ad follow */
        UART_StructInit(&UART_InitStructure);
        UART_InitStructure.UART_BaudRate = BaudRate;
        UART_Init(UART2, &UART_InitStructure);

        /* Enable UART */
        UART_Cmd(UART2, ENABLE);

        /* Enable GPIO and SYSCFG clock */
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,    ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

        /* Enable PA2 & PA3 alternate function */
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource2,  GPIO_AF_1);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource3,  GPIO_AF_1);

        /* Configure UART Tx(PA2) as alternate function push-pull */
        GPIO_StructInit(&GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

         /* Configure UART Rx(PA3) as input floating */
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
	#else
    if(UARTx == UART2)
    {
        /* Enable UART2 clock */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART2, ENABLE);

        /* UART Configuration ad follow */
        UART_StructInit(&UART_InitStructure);
        UART_InitStructure.UART_BaudRate = BaudRate;
        UART_Init(UART2, &UART_InitStructure);

        /* Enable UART */
        UART_Cmd(UART2, ENABLE);

        /* Enable GPIO and SYSCFG clock */
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB,ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

        /* Enable PB7 & PA6 alternate function */
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource6,  GPIO_AF_3);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource7,  GPIO_AF_4);

        /* Configure UART Tx(PB7) as alternate function push-pull */
        GPIO_StructInit(&GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        /* Configure UART Rx(PA6) as input floating */
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
	#endif
    else
    {
		
    }
}


/* Add the following code to support the printf function                       \ 
without selecting use microlib */	  
#if defined (PRINTF_EN)
#ifdef __GNUC__

#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

#endif 
#ifdef USE_IAR
PUTCHAR_PROTOTYPE
{
    /* Send a character to the UART */
    UART_SendData(PRINTF_UART, (uint8_t)ch);

     /* Loop until the end of transmission */
    while(!UART_GetFlagStatus(PRINTF_UART, UART_FLAG_TXEPT));
    return ch;
}

#else
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

#endif 

/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

