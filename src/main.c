#include "hal/setup.h"
#include "hal/discovery_leds.h"

/**
 * @brief Empty main
 */
int main(void)
{

  setup();

  led_green_on();
  led_orange_on();
  led_red_on();
  led_blue_on();
  
  return 0;
}
