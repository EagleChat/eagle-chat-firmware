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
#include "cdc.h"
#include <string.h>

#include "sha204.h"


int main (void)
{
	// Insert system clock initialization code here (sysclk_init()).

	cpu_irq_enable();

	irq_initialize_vectors();

	sysclk_init();

	board_init();

	cdc_start();

	cdc_write_line("Waiting 1s");
	sha204h_delay_ms(1000);

	uint8_t result;
	while(udi_cdc_getc() == 0);
	cdc_write_line("\nPress 'a' to perform lock procedure on connected board. Press 'b' to run demo");
	result = udi_cdc_getc();


	if (result == 'a') {

		uint8_t r = fullLock();

		cdc_log_int("Lock returned: ", r);

		for (int i = 0; i < 50; i ++) {
			cdc_write_string("Done! ");
		}

	} else if (result == 'b') {
		
		cdc_write_line("Press a to generate random number");
		
		uint8_t rand[32];

		while (true) {

			while(udi_cdc_getc() != 'a');

			cdc_log_int("Wakeup: ", sha204p_wakeup());

			cdc_log_int("Device Revision: ", getDeviceRevision());
			getRandom32(&rand);

			cdc_log_hex_string("Random: ", rand, 32);

			sha204p_sleep();

		}
	}


}
