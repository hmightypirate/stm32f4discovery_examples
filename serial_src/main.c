/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 * Copyright (C) 2011 Stephen Caudle <scaudle@doceme.com>
 * Copyright (C) 2013 Piotr Esden-Tempski <piotr@esden.net>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>

static void clock_setup(void)
{
	/* Enable GPIOD clock for LED & USARTs. */
	rcc_periph_clock_enable(RCC_GPIOD);
	rcc_periph_clock_enable(RCC_GPIOA);

	/* Enable clocks for USART2. */
	rcc_periph_clock_enable(RCC_USART2);
}

static void usart_setup(void)
{
	/* Enable the USART2 interrupt. */
	nvic_enable_irq(NVIC_USART2_IRQ);

	/* Setup GPIO pins for USART2 transmit. */
	gpio_mode_setup(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO5);

	/* Setup GPIO pins for USART2 receive. */
	gpio_mode_setup(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO6);
	gpio_set_output_options(GPIOD, GPIO_OTYPE_OD, GPIO_OSPEED_25MHZ, GPIO6);

	/* Setup USART2 TX and RX pin as alternate function. */
	gpio_set_af(GPIOD, GPIO_AF7, GPIO5);
	gpio_set_af(GPIOD, GPIO_AF7, GPIO6);

	/* Setup USART2 parameters. */
	usart_set_baudrate(USART2, 115200);
	usart_set_databits(USART2, 8);
	usart_set_stopbits(USART2, USART_STOPBITS_1);
	usart_set_mode(USART2, USART_MODE_TX_RX);
	usart_set_parity(USART2, USART_PARITY_NONE);
	usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

	/* Enable USART2 Receive interrupt. */
	usart_enable_rx_interrupt(USART2);

	/* Finally enable the USART. */
	usart_enable(USART2);
}

static void gpio_setup(void)
{
	/* Setup GPIO pin GPIO12 on GPIO port D for LED. */
	gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12);
}


static void dma_setup(void)
{
	/* DAC channel 1 uses DMA controller 1 Stream 5 Channel 7. */
	/* Enable DMA1 clock and IRQ */
	rcc_periph_clock_enable(RCC_DMA1);
	nvic_enable_irq(NVIC_DMA1_STREAM5_IRQ);
	dma_stream_reset(DMA1, DMA_STREAM5);
	dma_set_priority(DMA1, DMA_STREAM5, DMA_SxCR_PL_LOW);
	dma_set_memory_size(DMA1, DMA_STREAM5, DMA_SxCR_MSIZE_8BIT);
	dma_set_peripheral_size(DMA1, DMA_STREAM5, DMA_SxCR_PSIZE_8BIT);
	dma_enable_memory_increment_mode(DMA1, DMA_STREAM5);
	dma_enable_circular_mode(DMA1, DMA_STREAM5);
	dma_set_transfer_mode(DMA1, DMA_STREAM5,
				DMA_SxCR_DIR_MEM_TO_PERIPHERAL);
	/* The register to target is the DAC1 8-bit right justified data
	   register */
	dma_set_peripheral_address(DMA1, DMA_STREAM5, (uint32_t) &DAC_DHR8R1);
	/* The array v[] is filled with the waveform data to be output */
	dma_set_memory_address(DMA1, DMA_STREAM5, (uint32_t) waveform);
	dma_set_number_of_data(DMA1, DMA_STREAM5, 256);
	dma_enable_transfer_complete_interrupt(DMA1, DMA_STREAM5);
	dma_channel_select(DMA1, DMA_STREAM5, DMA_SxCR_CHSEL_7);
	dma_enable_stream(DMA1, DMA_STREAM5);
}

int main(void)
{
	clock_setup();
	gpio_setup();
	usart_setup();
	dma_setup();

	while (1) {
		__asm__("NOP");
	}

	return 0;
}

void usart2_isr(void)
{
	static uint8_t data = 'A';

	/* Check if we were called because of RXNE. */
	if (((USART_CR1(USART2) & USART_CR1_RXNEIE) != 0) &&
	    ((USART_SR(USART2) & USART_SR_RXNE) != 0)) {

		/* Indicate that we got data. */
		gpio_toggle(GPIOD, GPIO12);

		/* Retrieve the data from the peripheral. */
		data = usart_recv(USART2);

		/* Enable transmit interrupt so it sends back the data. */
		usart_enable_tx_interrupt(USART2);
	}

	/* Check if we were called because of TXE. */
	if (((USART_CR1(USART2) & USART_CR1_TXEIE) != 0) &&
	    ((USART_SR(USART2) & USART_SR_TXE) != 0)) {

		/* Put data into the transmit register. */
		usart_send(USART2, data);

		/* Disable the TXE interrupt as we don't need it anymore. */
		usart_disable_tx_interrupt(USART2);
	}
}