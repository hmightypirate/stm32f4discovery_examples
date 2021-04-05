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
void led_red_on(void) { gpio_set(GPIOD, GPIO14); }


/**
 * @brief blue led ON
 */
void led_blue_on(void) { gpio_set(GPIOD, GPIO15); }


/**
 * @brief green led OFF
 */
void led_green_off(void) { gpio_clear(GPIOD, GPIO12); }


/**
 * @brief orange led OFF
 */
void led_orange_off(void) { gpio_clear(GPIOD, GPIO13); }


/**
 * @brief red led OFF
 */
void led_red_off(void) { gpio_clear(GPIOD, GPIO14); }


/**
 * @brief blue led OFF
 */
void led_blue_off(void) { gpio_clear(GPIOD, GPIO15); }


/**
 * @brief green led toggle
 */
void led_green_toggle(void) { gpio_toggle(GPIOD, GPIO12); }


/**
 * @brief orange led toggle
 */
void led_orange_toggle(void) { gpio_toggle(GPIOD, GPIO13); }


/**
 * @brief red led toggle
 */
void led_red_toggle(void) { gpio_toggle(GPIOD, GPIO14); }


/**
 * @brief blue led toggle
 */
void led_blue_toggle(void) { gpio_toggle(GPIOD, GPIO15); }



