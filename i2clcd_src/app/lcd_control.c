#include "lcd_control.h"
#include "hal/discovery_leds.h"

volatile int8_t step = 0;


/*
 * @brief loop a text on the screen
 */

void loop_hello_world_text(void) {

  
  led_green_on();
  
  ssd1306_init(I2C2, DEFAULT_7bit_OLED_SLAVE_ADDRESS, 128, 32);

  step = 1;
  int16_t y = 0;

  led_red_on();
  
  while (1) {
    if (step!=0) {
      for (int i =0; i<8; i++) {
	y += step;
	ssd1306_clear();
	ssd1306_drawWCharStr(0, y, white, wrapDisplay,
			     L"This is small sentence" \
			     "to test the screen with the standard library." \
			     "Lorem fistrum torpedo te va a hasé pupitaa pupita " \
			     "ese pedazo de a wan fistro se calle ustée por la gloria " \
			     "de mi madre te voy a borrar el cerito. Amatomaa condemor " \
			     "benemeritaar ese que llega benemeritaar jarl. Fistro tiene " \
			     "musho peligro de la pradera caballo blanco caballo negroorl " \
			     "ahorarr tiene musho peligro ese hombree torpedo por la gloria " \
			     "de mi madre amatomaa ahorarr. Mamaar no puedor pecador ese " \
			     "hombree amatomaa apetecan se calle ustée pecador te va a hasé " \
			     "pupitaa. Al ataquerl quietooor diodenoo jarl por la gloria de " \
			     "mi madre diodenoo a peich.");

	ssd1306_refresh();
	for (uint32_t loop = 0; loop < 1000000; ++loop) {
	  __asm__("nop");
        }
      }
      if (step<0)
        step += 1;
      else
        step -= 1;
    }
  }			     
}
