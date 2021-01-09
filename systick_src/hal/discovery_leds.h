#ifndef __DISCOVERY_LEDS_H
#define __DISCOVERY_LEDS_H


#include <libopencm3/stm32/gpio.h>


void led_green_on(void);
void led_orange_on(void);
void led_red_on(void);
void led_blue_on(void);
void led_green_off(void);
void led_orange_off(void);
void led_red_off(void);
void led_blue_off(void);
void led_green_toggle(void);
void led_orange_toggle(void);
void led_red_toggle(void);
void led_blue_toggle(void);


#endif /* __DISCOVERY_LEDS_H */
