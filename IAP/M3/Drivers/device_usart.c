#include <HAL_Device.h>
#include <device_usart.h>
#include <string.h>
#include <soft_timer.h>

static unsigned char usart_send_buff[USART_SEND_BUFF_SIZE];
static unsigned char usart_recv_buff[USART_RECV_BUFF_SIZE];
static unsigned char usart_status;
static unsigned short usart_recv_count;
static unsigned short usart_send_count;
static unsigned short usart_send_length;

void device_usart_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	UART_InitTypeDef UART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(USART_TX_PORT_RCC | USART_RX_PORT_RCC |RCC_APB2Periph_AFIO, ENABLE);
	
	/* 配置串口GPIO */
	GPIO_InitStructure.GPIO_Pin = USART_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
    GPIO_Init(USART_TX_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = USART_RX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(USART_RX_PORT, &GPIO_InitStructure);
	
	/* 配置串口硬件(115200,n,8,1) */
	RCC_APB1PeriphClockCmd(USART_HW_RCC, ENABLE);
	UART_InitStructure.UART_BaudRate = 115200;
	UART_InitStructure.UART_WordLength = UART_WordLength_8b;
	UART_InitStructure.UART_StopBits = UART_StopBits_1;
	UART_InitStructure.UART_Parity = UART_Parity_No;
	UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
	UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;	
	UART_Init(USART_HW_SRUCTURE, &UART_InitStructure);
	UART_Cmd(USART_HW_SRUCTURE, ENABLE);
	
	/* 使能串口发送和接收中断 */
	UART_ITConfig(USART_HW_SRUCTURE, UART_IT_RXIEN | UART_IT_TXIEN, ENABLE);		
	
	/* 配置串口中断 */
    NVIC_InitStructure.NVIC_IRQChannel = USART_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void usart_send(const unsigned char *buffer, unsigned short length)
{
	memcpy(usart_send_buff, buffer, length);
	usart_send_count = 0;
	usart_send_length = length;
	if(length > 0)
	{
		UART_SendData(USART_HW_SRUCTURE, usart_send_buff[usart_send_count++]);
	}
}

unsigned short usart_read(unsigned char *buffer, unsigned short length)
{
	unsigned short recv_length;
	
	if(length < usart_recv_count)
		usart_recv_count = length;
	
	recv_length = usart_recv_count;
	memcpy(buffer, usart_recv_buff, usart_recv_count);
	usart_recv_count = 0;
	
	return recv_length;
}

unsigned char usart_status_get(unsigned char set)
{
	return (usart_status & set);
}

void usart_status_clear(unsigned char set)
{
	usart_status &= ~set;
}

void usart_status_set(unsigned char set)
{
	usart_status |= set;
}

void usart_recv_timeout(void *parameter)
{
	usart_status_set(USART_STATUS_RECV_FINISH);
}

void usart_poll(void)
{
	if(usart_status_get(USART_STATUS_RECV_CHAR))
	{
		usart_status_clear(USART_STATUS_RECV_CHAR);
		soft_timer_start(DEVICE_USART_RECV_TIMEOUT_TIMER_ID, usart_recv_timeout, NULL, 5);
	}
}

void USART_IRQ_HANDLER(void)
{
	volatile unsigned char chr;
	
	/* 处理串口溢出 */
	if(UART_GetITStatus(USART_HW_SRUCTURE, UART_OVER_ERR) == SET)
    {
        if(USART_HW_SRUCTURE->CSR & UART_IT_RXIEN)
		{
			chr = USART_HW_SRUCTURE->RDR;
		}
		UART_ClearITPendingBit(USART_HW_SRUCTURE, UART_OVER_ERR);
    }
	
	/* 处理串口接收数据中断 */ 
	if(UART_GetITStatus(USART_HW_SRUCTURE, UART_IT_RXIEN) == SET)
	{
		if((usart_recv_count < USART_RECV_BUFF_SIZE) && (!usart_status_get(USART_STATUS_RECV_FINISH)))
		{
			usart_recv_buff[usart_recv_count++] = UART_ReceiveData(USART_HW_SRUCTURE);
		}
		usart_status_set(USART_STATUS_RECV_CHAR);
		UART_ClearITPendingBit(USART_HW_SRUCTURE, UART_IT_RXIEN);
	}
	
	/* 串口发送完成中断 */
	if (UART_GetITStatus(USART_HW_SRUCTURE, UART_IT_TXIEN) != RESET)
	{
		if(usart_send_count < usart_send_length)
		{
			UART_SendData(USART_HW_SRUCTURE, usart_send_buff[usart_send_count++]);
		}
		else
		{
			usart_send_count = 0;
			usart_send_length = 0;
		}
		
		UART_ClearITPendingBit(USART_HW_SRUCTURE, UART_IT_TXIEN);
	}
}
