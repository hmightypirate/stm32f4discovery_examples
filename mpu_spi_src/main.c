#include "hal/setup.h"
#include "hal/discovery_leds.h"
#include "hal/mpu.h"
#include <libopencm3/stm32/usart.h>

void wait(void);

/**
 * @brief Basic wait function
 */
void wait(void)
{
  for (int i = 0; i < 10000000; ++i)
	__asm__("nop");
}

/**
 * @brief Empty main
 */
int main(void)
{

  setup();

  uint8_t control = 0x8F;
  uint8_t data = 0xFF;

  while(1) {
    wait(); led_orange_on(); led_red_off(); led_blue_off(); led_green_off();
    wait(); led_orange_off(); led_red_on(); led_blue_off(); led_green_off();
    wait(); led_orange_off(); led_red_off(); led_blue_on(); led_green_off();
    wait(); led_orange_off(); led_red_off(); led_blue_off(); led_green_on();

    data = mpu_who_am_i();
    usart_send_blocking(USART2, control);
    usart_send_blocking(USART2, data);
  }

  return 0;


}
