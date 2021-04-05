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
 * @brief Execute all setup functions.
 */
void setup(void)
{
	setup_clock();
	setup_exceptions();
	setup_gpio();
	setup_systick();
}

