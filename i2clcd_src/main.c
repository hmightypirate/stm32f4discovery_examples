#include "setup.h"
#include "lcd_control.h"
#include "hal/discovery_leds.h"

/*
 * @brief program entry point */

int main(void)
{

  setup();

  loop_hello_world_text();

  return 0;
}
