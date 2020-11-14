#include <HAL_Device.h>
#include <device.h>
#include <hardware.h>

extern void soft_timer_check(void);

/* 系统节拍中断 */
void SysTick_Handler(void)
{
    soft_timer_check();
}

/* 配置中断管理器 */
void NVIC_Configuration(void)
{
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
}

void hw_board_init(void)
{
	/* 中断管理器初始化 */
	NVIC_Configuration();
	
	/* 初始化节拍定时器 */
	extern u32 SystemCoreClock;
	SysTick_Config( SystemCoreClock / 1000 );
	
	/* 其他硬件初始化 */
	led_init();
	device_usart_init();
}

