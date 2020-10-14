#include <define.h>
#include <soft_timer.h>
#include <hardware.h>

struct soft_timer
{
	unsigned char flag;
	unsigned long count;
	void (*callback)(void *parameter);
	void *parameter;
};

static struct soft_timer soft_timer_pool[TIMER_ID_UNKNOW];

void soft_timer_init(void)
{
	memset(soft_timer_pool, 0, sizeof(soft_timer_pool));
}

void soft_timer_start(unsigned char id, void (*callback)(void*), void *parameter, unsigned long timeout)
{
	unsigned long level;
	
	level = hw_interrupt_disable();
	soft_timer_pool[id].callback = callback;
	soft_timer_pool[id].parameter = parameter;
	soft_timer_pool[id].count = timeout;
	hw_interrupt_enable(level);
}

void soft_timer_stop(unsigned char id)
{
	unsigned long level;
	
	level = hw_interrupt_disable();
	soft_timer_pool[id].callback = NULL;
	soft_timer_pool[id].count = 0;
	hw_interrupt_enable(level);
}

void soft_timer_check(void)
{
	unsigned char i;
	
	for(i = 0; i < ARRAY_SIZE(soft_timer_pool); i++)
	{
		if(soft_timer_pool[i].count > 0)
		{
			if(--soft_timer_pool[i].count == 0)
			{
				soft_timer_pool[i].flag |= SOFT_TIMER_FLAG_TIMEOUT;
			}
		}
	}
}

void soft_timer_handler(void)
{
	unsigned char i;
	
	for(i = 0; i < ARRAY_SIZE(soft_timer_pool); i++)
	{
		if(soft_timer_pool[i].flag & SOFT_TIMER_FLAG_TIMEOUT)
		{
			if(soft_timer_pool[i].callback)
			{
				soft_timer_pool[i].callback(soft_timer_pool[i].parameter);
			}
			soft_timer_pool[i].flag &= ~SOFT_TIMER_FLAG_TIMEOUT;
		}
	}
}
