#include <board.h>
#include <device.h>
#include <program_protocol.h>
#include <program.h>
#include <soft_timer.h>
#include <hw.h>

int main(void)
{
	unsigned long level;
	
	/* ���ж� */
	level = hw_interrupt_disable();
	
	/* ���Ӳ����ʼ�� */
	hw_board_init();
	
	/* �����ʱ����ʼ�� */
	soft_timer_init();
	
	/* Ӧ�ó����ʼ�� */
	program_protocol_init();
	program_init();
	
	/* ���ж� */
	hw_interrupt_enable(level);

	while(1)
	{
		soft_timer_handler();
		usart_poll();
		program_protocol_handler();
	}
}

