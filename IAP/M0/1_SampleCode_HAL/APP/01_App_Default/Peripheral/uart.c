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
#include "main.h"
#include "m0_interrupt_table_redefine.h"

/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/

/* Exported variables -------------------------------------------------------*/
extern uint32_t SystemCoreClock;

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
 * @brief  The actual handler of UART interrupts
 * @param  None
 * @retval None
 * @attention  None
******************************************************************************/
void IAP_UART_Processing(void)
{
    uint8_t ResByte;
    static uint8_t RxIndex = 0xFF;
    if (UART_GetFlagStatus(IAP_UART, UART_FLAG_RXAVL) != RESET)
    {      
        UART_ClearITPendingBit(IAP_UART,UART_IT_RXIEN);  
        ResByte = (uint8_t)(UART_ReceiveData(IAP_UART) & (uint8_t)0xFF);   
        if(0x55 == ResByte && RxIndex == 0xFF )      
        {  /* Header of frame*/
            RxIndex = 0 ;         
        }   
        else if(0xFF == ResByte)
        {  /* End of frame */
            RxIndex = 0xFF ;
            RxOverFlag = 0x01 ;            
        }
        else
        {
            if(RxIndex != 0xFF && RxIndex < 12)
            {
                RxBuff[RxIndex++] = ResByte ;               
            }
            else 
            {
                RxIndex = 0xFF ;
            }
        }
    }
}

/******************************************************************************
 * @brief  uart initialization    
 * @param  two       
 * @retval  None 
 * @attention  None
******************************************************************************/
void UARTx_Configure(UART_TypeDef *UARTx,uint32_t BaudRate)
{    
	NVIC_InitTypeDef NVIC_InitStruct;
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
        
        NVIC_InitStruct.NVIC_IRQChannel = UART1_IRQn;
        tNVIC_Table.pUART1_IRQHandler = IAP_UART_Processing;
    }
    else if(UARTx == UART2)
    {
        /* Enable UART2 clock */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART2, ENABLE);

        /* UART Configuration ad follow */
        UART_StructInit(&UART_InitStructure);
        UART_InitStructure.UART_BaudRate = BaudRate;
        UART_Init(UART2, &UART_InitStructure);

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
        
        NVIC_InitStruct.NVIC_IRQChannel = UART2_IRQn;
        tNVIC_Table.pUART2_IRQHandler = IAP_UART_Processing;
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
        
        NVIC_InitStruct.NVIC_IRQChannel = UART2_IRQn;
        tNVIC_Table.pUART2_IRQHandler = IAP_UART_Processing;
    }
	#endif
    else
    {
		
    }
    /* Enable the UART_IT interrupt */
    UART_ITConfig(IAP_UART,UART_IT_RXIEN,ENABLE);
    
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 1;
    NVIC_Init(& NVIC_InitStruct);
    
    /* Enable UART */
    UART_Cmd(UARTx, ENABLE);
}


/* Add the following code to support the printf function                       \ 
without selecting use microlib */	  
#if defined (DEBUG_EN)
#ifdef __GNUC__

#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

#endif 
#ifdef USE_IAR
PUTCHAR_PROTOTYPE
{
    /* Send a character to the UART */
    UART_SendData(DEBUG_UART, (uint8_t)ch);

     /* Loop until the end of transmission */
    while(!UART_GetFlagStatus(DEBUG_UART, UART_FLAG_TXEPT));     
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
    UART_SendData(DEBUG_UART, (uint8_t)ch);

     /* Loop until the end of transmission */
    while(!UART_GetFlagStatus(DEBUG_UART, UART_FLAG_TXEPT));

    return ch;
}
#endif 

#endif 

/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

