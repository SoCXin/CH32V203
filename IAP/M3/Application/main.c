#include <board.h>
#include <device.h>
#include <program_protocol.h>
#include <program.h>
#include <soft_timer.h>
#include <hw.h>

int main(void)
{
	unsigned long level;
	
	/* 关中断 */
	level = hw_interrupt_disable();
	
	/* 板机硬件初始化 */
	hw_board_init();
	
	/* 软件定时器初始化 */
	soft_timer_init();
	
	/* 应用程序初始化 */
	program_protocol_init();
	program_init();
	
	/* 开中断 */
	hw_interrupt_enable(level);

	while(1)
	{
		soft_timer_handler();
		usart_poll();
		program_protocol_handler();
	}
}

