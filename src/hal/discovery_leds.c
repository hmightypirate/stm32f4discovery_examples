#include "discovery_leds.h"



/**
 * @brief green led ON
 */
void led_green_on(void) { gpio_set(GPIOD, GPIO12); }


/**
 * @brief orange led ON
 */
void led_orange_on(void) { gpio_set(GPIOD, GPIO13); }


/**
 * @brief red led ON
 */
void led_red_on(void) { gpio_clear(GPIOD, GPIO14); }


/**
 * @brief blue led ON
 */
void led_blue_on(void) { gpio_clear(GPIOD, GPIO15); }


