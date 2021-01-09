#include "hal/setup.h"
#include "hal/discovery_leds.h"
#include "hal/button.h"

/**
 * @brief Empty main
 */
int main(void)
{

  setup();

  while(1) {
    while(!button_read_user()) {
      led_blue_on();
      led_green_off();
      led_orange_on();
      led_red_off();
    }
    while(button_read_user()) {
      led_blue_off();
      led_green_on();
      led_orange_off();
      led_red_on();      
    }
  }
  return 0;
}
