/**
 * \file
 *
 * \brief Application CDC Implementation
 *
 * Copyright (c) 2011 - 2014 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
 /**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include "nvm.h"
#include "cdc.h"

//! USBID position in user signature row
#define USER_SIGNATURE_USBID_POS   8
//! USBID size in user signature row
#define USER_SIGNATURE_USBID_SIZE 12

uint8_t cdc_serial_number[USB_DEVICE_GET_SERIAL_NAME_LENGTH + 1] = USB_SERIAL_NUMBER; // Just a random string for now

static bool cdc_opened = false;
static bool cdc_enabled = false;

void cdc_start(void)
{
	udc_start();
}

void cdc_set_dtr(bool enable)
{
	cdc_opened = enable;
}

bool cdc_enable(void)
{
	cdc_enabled = true;
	return true;
}

void cdc_disable(void)
{
	cdc_enabled = false;
}

void cdc_write_string(const char *buf) {
	iram_size_t size = 0;
	uint8_t* temp = buf;
	while (*temp != 0) {
		++temp;
		++size;
	}
	udi_cdc_write_buf((void *) buf, size);
}

void cdc_write_hex(const uint8_t c) {
	const uint8_t table[] = "0123456789ABCDEF";
	udi_cdc_putc(table[(c>>4) & 0x0F]);
	udi_cdc_putc(table[c & 0x0F]);
}

void cdc_write_hex_string(char *string, uint32_t length) {
	for (uint8_t i = 0; i < length; ++i) {
		cdc_write_hex(string[i]);
	}
}

void cdc_newline() {
	udi_cdc_putc('\n');
}

void cdc_log_int(const char *message, uint32_t value) {
	cdc_write_string((uint8_t *)message);
	uint8_t v[12];
	itoa(value, v, 10);
	cdc_write_string(v);
	cdc_newline();
}

void cdc_log_string(const char *message, const char *value) {
	cdc_write_string((uint8_t *)message);
	cdc_write_string((uint8_t *)value);
	cdc_newline();
}

void cdc_log_hex(const char *message, uint8_t value) {
	cdc_write_string((uint8_t *)message);
	cdc_write_hex(value);
	cdc_newline();
}

void cdc_log_hex_string(const char *message, uint8_t *value, uint32_t len) {
	cdc_write_string(message);
	cdc_write_hex_string(value, len);
	cdc_newline();
}

uint32_t cdc_read_string(uint8_t *buffer, uint32_t maxlen) {
	uint32_t i = 0;
    while (i < maxlen) {
        buffer[i] = udi_cdc_getc();
        if ((buffer[i] == '\n') || (buffer[i] == '\r')) {
            buffer[i] = 0;
            return i;
        }
        i++;
    }
    return maxlen;
}

/*
static void say_hello(void) {
	uint8_t msg[] = "Hello.\n";

	while (!udi_cdc_is_tx_ready());

	uint8_t * c = msg;

	while (*c != 0) {
		udi_cdc_putc(*c);
		++c;
	}
}
*/

/*
static void echo(void) {
	uint8_t read = udi_cdc_getc();
	udi_cdc_putc(read+1);
}
*/

void cdc_rx_notify(uint8_t port) {
}

