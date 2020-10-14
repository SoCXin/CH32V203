#ifndef _SOFT_TIMER_H
#define _SOFT_TIMER_H

#define SOFT_TIMER_FLAG_TIMEOUT		0x01

enum timer_id
{
	PROGRAM_TIMEROUT_TIMER_ID = 0,
	DEVICE_LED_TIMER_ID,
	DEVICE_USART_RECV_TIMEOUT_TIMER_ID,
	TIMER_ID_UNKNOW
};

void soft_timer_init(void);
void soft_timer_start(unsigned char id, void (*callback)(void*), void *parameter, unsigned long timeout);
void soft_timer_stop(unsigned char id);
void soft_timer_check(void);
void soft_timer_handler(void);

#endif
