#include <HAL_Device.h>
#include <soft_timer.h>
#include <device_led.h>

static void hw_led_init(void)
{
	
}

void led_control(unsigned char newstate)
{
	
}

void led_init(void)
{
	
	hw_led_init();
	led_control(LED_OFF);
}

