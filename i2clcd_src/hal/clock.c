#include "clock.h"

volatile uint32_t counter = 0;

/*
 * @brief return the number of calls of the systick routine
 */
uint32_t get_counter(void) {
  return counter;
}

/*
 *  @brief systick interruption routine. It counts from 0 to 10000
 */
void sys_tick_handler(void) {

  // FIXME: app dependent function
  counter++;

  if (counter == 60000){
    counter = 0;
  }

			   
}
