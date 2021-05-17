#include "lcd_control.h"


volatile int8_t step = 0;


const  FontChar_t icon_high={26, {0x00, 0x00, 0xe0, 0xe0, 0xf8, 0xf8,
				  0xfc, 0xfc, 0xfc, 0xfc, 0xfe, 0xfe,
				  0xf3, 0xf3, 0x7e, 0x7e, 0x7e, 0x7e,
				  0x3e, 0x3e, 0x3c, 0x3c, 0x1c, 0x1c,
				  0x18, 0x18}};

const  FontChar_t icon_low={26, {0x00, 0x00, 0x07, 0x07, 0x1f, 0x1f,
				 0x3f, 0x3f, 0x3f, 0x3f, 0x7f, 0x7f,
				 0x7f, 0x7f, 0x7e, 0x7e, 0x7e, 0x7e,
				 0x7c, 0x7c, 0x3c, 0x3c, 0x38, 0x38,
				 0x18, 0x18}};

				 


/*
 * @brief loop writing an icon on the screen
 */
void loop_intro_icon(void) {

  led_green_on();

  // Initialize the screen
  ssd1306_init(I2C2, DEFAULT_7bit_OLED_SLAVE_ADDRESS, 128, 32);

  int16_t y = 0;
  int16_t x_offset = 32; // For some reason this offset is needed in this screen 
  
  led_red_on();

  while (1) {
    for (int i =0; i<1; i++) {
      ssd1306_clear();

      y = 0;

      ssd1306_drawWCharIcon(x_offset, y, white, nowrap, &icon_high);
      
      y = 8;

      ssd1306_drawWCharIcon(x_offset, y, white, nowrap, &icon_low);
      
						       
      ssd1306_refresh();
      led_blue_on();

      for (uint32_t loop = 0; loop < 1000000; ++loop) {
	__asm__("nop");
      }
    }
  }
  
  
}


/*
 * @brief loop a text on the screen
 */

void loop_hello_world_text(void) {

  
  led_green_on();
  
  ssd1306_init(I2C2, DEFAULT_7bit_OLED_SLAVE_ADDRESS, 128, 32);

  int16_t y = 0;

  led_red_on();

  int16_t x_offset = 32; // For some reason this offset is needed in this screen
  
  while (1) {
    for (int i =0; i<1; i++) {
      ssd1306_clear();

      y = 0;
	
      ssd1306_drawWCharStr(x_offset, 0, white, nowrap,
			     L"Lorem finstrum");
      y = 8;
	
      ssd1306_drawWCharStr(x_offset, y, white, nowrap,
			     L"Tiene musho peligro");
						       
      ssd1306_refresh();
      led_blue_on();

      for (uint32_t loop = 0; loop < 1000000; ++loop) {
	__asm__("nop");
      }
    }
  }
}
