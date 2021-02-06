#include "setup.h"


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
	rcc_periph_clock_enable(RCC_GPIOA); // BUTTON

	/* Bluetooth */
	rcc_periph_clock_enable(RCC_USART2);

	/* Gyroscope */
	rcc_periph_clock_enable(RCC_SPI3);

	/* DMA */
	rcc_periph_clock_enable(RCC_DMA2);

	/* Enable clock cycle counter */
	dwt_enable_cycle_counter();
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
 * - USART2 interrupt.
 *
 * @see Programming Manual (PM0214).
 */
static void setup_exceptions(void)
{

  nvic_set_priority(NVIC_DMA2_STREAM5_IRQ, 0);
  nvic_set_priority(NVIC_DMA2_STREAM7_IRQ, 0);
  nvic_set_priority(NVIC_USART2_IRQ, 0);
  
  nvic_enable_irq(NVIC_DMA2_STREAM7_IRQ);
  nvic_enable_irq(NVIC_DMA2_STREAM5_IRQ);
  nvic_enable_irq(NVIC_USART2_IRQ);

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

  gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO0);

	/* Setup GPIO pins for USART2 transmit. */
	gpio_mode_setup(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO5);

	/* Setup GPIO pins for USART2 receive. */
	gpio_mode_setup(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO6);
	gpio_set_output_options(GPIOD, GPIO_OTYPE_OD, GPIO_OSPEED_25MHZ, GPIO6);

	/* Setup USART2 TX and RX pin as alternate function. */
	gpio_set_af(GPIOD, GPIO_AF7, GPIO5);
	gpio_set_af(GPIOD, GPIO_AF7, GPIO6);



  /* MPU */
  gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO15);
  gpio_set(GPIOA, GPIO15);
  gpio_mode_setup(GPIOC, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN,
			GPIO10 | GPIO11 | GPIO12);
  gpio_set_af(GPIOC, GPIO_AF6, GPIO10 | GPIO11 | GPIO12);
}


/**
 * @brief Setup SPI.
 *
 * SPI is configured as follows:
 *
 * - Master mode.
 * - Clock baud rate: PCLK1 / speed_div; PCLK1 = 36MHz.
 * - Clock polarity: 0 (idle low; leading edge is a rising edge).
 * - Clock phase: 0 (out changes on the trailing edge and input data
 *   captured on rising edge).
 * - Data frame format: 8-bits.
 * - Frame format: MSB first.
 *
 * NSS is configured to be managed by software.
 */
static void setup_spi(uint8_t speed_div)
{
	spi_reset(SPI3);

	spi_init_master(SPI3, speed_div, SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE,
			SPI_CR1_CPHA_CLK_TRANSITION_1, SPI_CR1_DFF_8BIT,
			SPI_CR1_MSBFIRST);

	spi_enable_software_slave_management(SPI3);
	spi_set_nss_high(SPI3);

	spi_enable(SPI3);
}


/**
 * @brief Setup SPI for gyroscope read, less than 20 MHz.
 *
 * The clock baudrate is 42 MHz / 4 = 10.5 MHz.
 */
void setup_spi_high_speed(void)
{
  setup_spi(SPI_CR1_BAUDRATE_FPCLK_DIV_4);
}


/**
 * @brief Setup SPI for gyroscope Write, less than 1 MHz.
 *
 * The clock baudrate is 42 MHz / 64 = 0.65625 MHz.
 */
void setup_spi_low_speed(void)
{
  setup_spi(SPI_CR1_BAUDRATE_FPCLK_DIV_64);
}

/**
 * @brief Setup USART for bluetooth communication.
 */
static void setup_usart(void)
{
	usart_set_baudrate(USART2, 115200); //921600
	usart_set_databits(USART2, 8);
	usart_set_stopbits(USART2, USART_STOPBITS_1);
	usart_set_parity(USART2, USART_PARITY_NONE);
	usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);
	usart_set_mode(USART2, USART_MODE_TX_RX);

	//usart_enable_tx_interrupt(USART2);
	//usart_enable_rx_interrupt(USART2);
	usart_enable(USART2);

}

/**
 * @brief Execute all setup functions.
 */
void setup(void)
{
	setup_clock();
	setup_exceptions();
	setup_gpio();
	setup_usart();
	setup_mpu();
}
