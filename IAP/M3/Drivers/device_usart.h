#ifndef _DEVICE_USART_H
#define _DEVICE_USART_H

/* ���ڷ���GPIO */
#define USART_TX_PORT_RCC			RCC_APB2Periph_GPIOB
#define USART_TX_PORT				GPIOB
#define USART_TX_PIN				GPIO_Pin_10

/* ���ڽ���GPIO */
#define USART_RX_PORT_RCC			RCC_APB2Periph_GPIOB
#define USART_RX_PORT				GPIOB
#define USART_RX_PIN				GPIO_Pin_11

/* ����Ӳ������ */
#define USART_HW_RCC				RCC_APB1Periph_UART3
#define USART_HW_SRUCTURE			UART3
#define USART_IRQ					UART3_IRQn
#define USART_IRQ_HANDLER			UART3_IRQHandler

/* ���ڷ���ʹ��DMA���� */
#define USART_DMA_TX_CHANNEL_NO		2
#define USART_DMA_TX_CHANNEL		DMA1_Channel2
#define USART_DMA_TX_IRQ			DMA1_Channel2_IRQn
#define USART_DMA_TX_FLAG			(DMA1_FLAG_TC2 | DMA1_FLAG_HT2 | DMA1_FLAG_TE2)
#define USART_DMA_TX_HANDLER		DMA1_Channel2_IRQHandler

/* �������ݽ���ʹ��DMA���� */
#define USART_DMA_RX_CHANNEL_NO		3
#define USART_DMA_RX_CHANNEL		DMA1_Channel3
#define USART_DMA_RX_IRQ			DMA1_Channel3_IRQn
#define USART_DMA_RX_FLAG			(DMA1_FLAG_TC3 | DMA1_FLAG_HT3 | DMA1_FLAG_TE3)
#define USART_DMA_RX_HANDLER		DMA1_Channel3_IRQHandler

/* ����״̬λ */
#define USART_STATUS_RECV_CHAR		0x01		// ���յ�һ���ֽ�����
#define USART_STATUS_RECV_FINISH	0x02		// ���յ�һ֡����

#define USART_SEND_BUFF_SIZE		1024		// ���ڷ��ͻ�������С
#define USART_RECV_BUFF_SIZE		1024		// ���ڽ��ջ�������С

/* �������ò���API */
void device_usart_init(void);
void usart_send(const unsigned char *buffer, unsigned short length);
unsigned short usart_read(unsigned char *buffer, unsigned short length);
unsigned char usart_status_get(unsigned char set);
void usart_status_clear(unsigned char set);
void usart_status_set(unsigned char set);
void usart_poll(void);

#endif
