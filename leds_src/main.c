#include "hal/setup.h"
#include "hal/discovery_leds.h"

void wait(void);
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

  while(1) {
    wait(); led_blue_on();
    wait(); led_green_on();
    wait(); led_orange_on();
    wait(); led_red_on();
    wait(); led_blue_off();
    wait(); led_green_off();
    wait(); led_orange_off();
    wait(); led_red_off();
  }

  return 0;
}
