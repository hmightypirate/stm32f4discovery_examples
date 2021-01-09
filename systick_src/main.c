#include "hal/setup.h"
#include "hal/discovery_leds.h"
#include "hal/clock.h"

void wait(void);
void wait(void)
{
  for (int i = 0; i < 100; ++i)
	__asm__("nop");
}


/**
 * @brief Empty main
 */
int main(void)
{

  setup();
  
  uint32_t consolidated_counter = 0;

  systick_interrupt_enable();
  
  while(1) {
    wait();

    uint32_t led_counter = get_counter();

    if (led_counter != consolidated_counter)
      {
	consolidated_counter = led_counter;
	if (consolidated_counter % 250 == 0) {
	  led_green_toggle();
	  
	}

	if (consolidated_counter % 500 == 0) {
	  led_blue_toggle();
	  
	}

	if (consolidated_counter % 1000 == 0) {
	  led_red_toggle();
	  
	}

	// toggle orange every 15 seconds
	if (consolidated_counter % 15000 == 0) {
	  led_orange_toggle();
	}

      }
  }

  return 0;
}
