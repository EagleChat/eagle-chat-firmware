/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
 /**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include "asf.h"
#include <avr/io.h>
#include "usart.h"

int main (void)
{
	// Insert system clock initialization code here (sysclk_init()).

	ioport_init();

	cpu_irq_enable();

	irq_initialize_vectors();

	sysclk_init();

	cdc_start();

	usart_init();

	uint8_t rx;
	while (1) {
		// Get a character on USB, echo out USART

		while (udi_cdc_getc() == 0);

		cdc_write_line("Press a: echo USB to UART");
		cdc_write_line("Press b: echo UART to USB");
		cdc_write_string("Choice: ");
		rx = udi_cdc_getc();
		cdc_newline();

		if (rx == 'a') {
			while (1) {
				rx = udi_cdc_getc();
				usart_write_char(rx);
			}
		} else if (rx == 'b') {
			while(1) {
				rx = usart_read_char();
				udi_cdc_putc(rx);
			}
		}
	}




	// Insert application code here, after the board has been initialized.
}
