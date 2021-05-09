#include "setup.h"

/** Exception priorities uses only the upper 4 bits of the register
                                                                                           
   That means the priorities should multiplied by this factor                     
   (shifting 4 bits to the left)
                                                         
*/
#define PRIORITY_FACTOR 16


static void rcc_clock_setup_hsi_3v3(const struct rcc_clock_scale *clock)
{
	/* Enable internal high-speed oscillator (HSI). */
	rcc_osc_on(RCC_HSI);
	rcc_wait_for_osc_ready(RCC_HSI);

	/* Select HSI as SYSCLK source. */
	rcc_set_sysclk_source(RCC_CFGR_SW_HSI);

	/* Set the VOS scale mode */
	rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_PWR);
	pwr_set_vos_scale(clock->voltage_scale);

	/*
	 * Set prescalers for AHB, ADC, ABP1, ABP2.
	 * Do this before touching the PLL (TODO: why?).
	 */
	rcc_set_hpre(clock->hpre);
	rcc_set_ppre1(clock->ppre1);
	rcc_set_ppre2(clock->ppre2);

	/* Disable PLL oscillator before changing its configuration. */
	rcc_osc_off(RCC_PLL);

	/* Configure the PLL oscillator. */
	rcc_set_main_pll_hsi(clock->pllm, clock->plln, clock->pllp, clock->pllq,
			     clock->pllr);

	/* Enable PLL oscillator and wait for it to stabilize. */
	rcc_osc_on(RCC_PLL);
	rcc_wait_for_osc_ready(RCC_PLL);

	/* Configure flash settings. */
	if (clock->flash_config & FLASH_ACR_DCEN)
		flash_dcache_enable();
	else
		flash_dcache_disable();
	if (clock->flash_config & FLASH_ACR_ICEN)
		flash_icache_enable();
	else
		flash_icache_disable();
	flash_set_ws(clock->flash_config);

	/* Select PLL as SYSCLK source. */
	rcc_set_sysclk_source(RCC_CFGR_SW_PLL);

	/* Wait for PLL clock to be selected. */
	rcc_wait_for_sysclk_status(RCC_PLL);

	/* Set the peripheral clock frequencies used. */
	rcc_ahb_frequency = clock->ahb_frequency;
	rcc_apb1_frequency = clock->apb1_frequency;
	rcc_apb2_frequency = clock->apb2_frequency;

	/* Disable internal high-speed oscillator. */
	rcc_osc_off(RCC_HSI);
}



/**
 * @brief Initial clock setup.
 *
 * Use the Internal High Speed clock (HSI), at 16 MHz, and set the SYSCLK
 * at 168 MHz.
 *
 * The peripheral clocks are set to:
 *
 * - AHB to 168 MHz (max. is 180 MHz)
 * - APB1 to 42 MHz
 * - APB2 to 84 MHz
 *
 * Enable required clocks for the GPIOs and timers as well.
 *
 * A pull-up resistor is used in RX to avoid a floating input when no
 * bluetooth is connected, which could trigger incorrect interruptions.
 *
 * @see Reference manual (RM0090), in particular "Reset and clock control for
 * STM32F405xx" section.
 */
static void setup_clock(void)
{
	rcc_clock_setup_hsi_3v3(&rcc_hse_16mhz_3v3[RCC_CLOCK_3V3_168MHZ]);

	/* GPIOs */
	rcc_periph_clock_enable(RCC_GPIOD); // DISCOVERY LEDS

	/* Enabling GPIOs SCL y SDA */
	rcc_periph_clock_enable(RCC_GPIOB);

	/* Set clock for I2C */
	rcc_periph_clock_enable(RCC_I2C2);
	
	/* Enable clock cycle counter */
	dwt_enable_cycle_counter();
}



/**
 * @brief Initial GPIO configuration.
 *
 * Set GPIO modes and initial states.
 *
 * @see STM32F405RG datasheet and in particular the "Alternate function
 * mapping" section.
 */
static void setup_gpio(void)
{
  /* LEDs */
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
		  GPIO12 | GPIO13 | GPIO14 | GPIO15);
  gpio_clear(GPIOD, GPIO12 | GPIO13 | GPIO14 | GPIO15);

  /* I2C SCL and SDA */
  gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, //GPIO_PUPD_PULLUP,
		  GPIO10 | GPIO11);

  gpio_set_output_options(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ,
			  GPIO10 | GPIO11);
  
  gpio_set_af(GPIOB, GPIO_AF4, GPIO10 | GPIO11);
    
}

/**
 * @brief Exceptions configuration.
 *
 * This function configures Nested Vectored Interrupt Controller for IRQ and
 * System Control Block for system interruptions.
 *
 * Interruptions enabled:
 *
 * - DMA 2 stream 7 interrupt.
 * - USART1 interrupt.
 *
 * @see Programming Manual (PM0214).
 */
static void setup_exceptions(void)
{

  nvic_set_priority(NVIC_SYSTICK_IRQ, 1 * PRIORITY_FACTOR);  // systick interruptions are enabled in main (original bulebule code)

}


/**
 * @brief Set SysTick interruptions frequency and enable SysTick counter.
 *
 * SYSCLK is at 168 MHz as well as the Advanced High-permormance Bus (AHB)
 * because, by default, the AHB divider is set to 1, so the AHB clock has the
 * same frequency as the SYSCLK.
 *
 * SysTick interruption frequency is set to `SYSTICK_FREQUENCY_HZ`.
 *
 * @see RM0090 reference manual and in particular the "Clock tree" figure.
 */
static void setup_systick(void)
{
	systick_set_frequency(SYSTICK_FREQUENCY_HZ, SYSCLK_FREQUENCY_HZ);
	systick_counter_enable();
}


/**
 * @brief setup I2C for connection with a SSD1306 peripheral
 *
 * I2C uses the APB1 peripheral bus at 42 MHz
 *
 * @see STM32F405xx datasheet
 *
 */
static void setup_i2c(void)
{
  i2c_peripheral_disable(I2C2);

  i2c_reset(I2C2);
  
  /* APB1 is running at 42 MHZ */
  i2c_set_clock_frequency(I2C2, 42); // FIXME: use clock struct?

  /* up to 400KHz - I2C Fast Mode */
  i2c_set_fast_mode(I2C2);
  
  /*
   * peripheric clock (APB1) is 42MHz -> cycle time 23.8095 ns
   *
   * The frequency of the I2C clock in fast mode is 400Khz -> t_i2c = 2500ns
   *  t_i2c = t_low + t_high 
   *  Following 782 page of the Reerence Manual (stm32f4) t_low = 2 * t_high or t_low = 16/9 t_high (fast_mode)
   * t_low = 2/3 * 2500 = 1666 ns 
   * t_high = 1/3 * 2500 = 833 ns
   *
   * Now we have to configure the STM with this values (as closer as possible)
   * tp_clk1 = 23.8095
   * Thus (page 782 of the manual) CCR = t_low /(tp_clk1 * 2) = 34.98 (0x23 in hex)
   * 
   */
  i2c_set_ccr(I2C2, 0x23); 

  /*
   * fclock for I2C is 42MHz -> cycle time 24ns, rise time for
   * 400kHz => 300ns and 100kHz => 1000ns; 300ns/24ns = 13;
   * Incremented by 1 -> 14.
   */
  i2c_set_trise(I2C2, 0x43); //0x0e); // FIXME: check!

  /*
   * Enable ACK on I2C
   */
  i2c_enable_ack(I2C2);


  /*
   * This is our slave address - needed only if we want to receive from
   * other masters.
   */
  i2c_set_own_7bit_slave_address(I2C2, 0x32);  // FIXME: CHECK


  /* If everything is configured -> enable the peripheral. */
  i2c_peripheral_enable(I2C2);

}


/**
 * @brief Execute all setup functions.
 */
void setup(void)
{
	setup_clock();
	setup_exceptions();
	setup_gpio();
	setup_i2c();
	setup_systick();

}

