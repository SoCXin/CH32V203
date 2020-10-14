#ifndef _DEVICE_LED_H
#define _DEVICE_LED_H

/* ¶¨ÒåLED×´Ì¬ */
#define LED_OFF				0
#define LED_ON				(!LED_OFF)

void led_init(void);
void led_control(unsigned char newstate);

#endif
