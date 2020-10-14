#include <HAL_Device.h>
#include <device.h>
#include <hardware.h>

extern void soft_timer_check(void);

/* ϵͳ�����ж� */
void SysTick_Handler(void)
{
    soft_timer_check();
}

/* �����жϹ����� */
void NVIC_Configuration(void)
{
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
}

void hw_board_init(void)
{
	/* �жϹ�������ʼ�� */
	NVIC_Configuration();
	
	/* ��ʼ�����Ķ�ʱ�� */
	extern u32 SystemCoreClock;
	SysTick_Config( SystemCoreClock / 1000 );
	
	/* ����Ӳ����ʼ�� */
	led_init();
	device_usart_init();
}

