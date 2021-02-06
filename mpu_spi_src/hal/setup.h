#ifndef __SETUP_H
#define __SETUP_H


#include <libopencm3/cm3/dwt.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/pwr.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/usart.h>

#include "mpu.h"

#define SYSCLK_FREQUENCY_HZ 168000000
#define SYSTICK_FREQUENCY_HZ 1000
#define MICROSECONDS_PER_SECOND 1000000  // Used by mmlib
#define PI 3.1415  // Used by mmlib

void setup(void);
void setup_spi_low_speed(void);   // Used by mmlib (mpu)
void setup_spi_high_speed(void);  // Used by mmlib (mpu)

#endif /* __SETUP_H */
