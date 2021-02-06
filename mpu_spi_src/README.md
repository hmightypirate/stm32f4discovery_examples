# Button Example

This example show how to read the button of the STM32F4DISCOVERY

Build and Flash the example.

With user button pressed:
GREEN ON
RED ON

With user button released:
BLUE ON
ORANGE ON


TL;DR

Read button:

    gpio_get(GPIOA, GPIO0);


Setup button:

    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO0);
