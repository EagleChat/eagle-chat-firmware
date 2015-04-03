#define F_CPU	32000000L

#include "asf.h"
#include <avr/io.h>
#include "cdc.h"
#include <util/delay.h>

#include <string.h>

#include "radio.h"
#include "fifo.h"


uint32_t TRANSMITPERIOD = 150; //transmit a frame to gateway so often (in ms)
uint8_t payload[250];
uint8_t buff[20];
uint8_t sendSize=0;
uint8_t requestACK = true;

int main(void);
int main ()
{
	cpu_irq_enable();

	irq_initialize_vectors();

	sysclk_init();

	ioport_init();

	rtc_init();

	cdc_start();

	while (!cdc_opened());
	rtc_set_time(0);
	cdc_log_int("About to instantiate module ", rtc_get_time());

	setupRadio();

	uint8_t address = 0;

	while(1) {
		cdc_write_string("My address: ");
		cdc_read_string(payload, 3);
		address = (uint8_t) atoi((const char *)payload);
		cdc_newline();

		if (address > 0) {
			cdc_log_int("You said: ", address);
			break;
		}

		cdc_write_line("Invalid address.");
	}

	setAddress(address);

	uint8_t mode = 0;
	while (mode != 'S' && mode != 'R') {
		cdc_write_string("Enter S for send or R for receive, or F for fifo test: ");
		mode = udi_cdc_getc();
		//mode = 'R';
	}

	if (mode == 'S') {

		uint32_t start_time;
		uint16_t  failures;
		uint16_t count;
		bool     acked;


		memset(payload, 'A', sizeof(payload));
		payload[sizeof(payload)-1] = 0;

		while (1) {
			while(1) {
				cdc_write_string("To address: ");
				cdc_read_string(payload, 4);
				address = (uint8_t) atoi((const char *)payload);

				if (address > 0) {
					cdc_log_int("You said: ", address);
					break;
				}
				cdc_write_line("Invalid address.");
			}

			cdc_newline();
			failures = 0;

			for (count = 0; count <= 0xFF; ++count) {
				cdc_log_int("\nSend attempt: ", count);

				uint32_t send_time = rtc_get_time();

				sendFrame(address, payload, sizeof(payload));
				acked = false;
				while (rtc_get_time() - send_time < 50) {
					if (acked = ackReceived(address))
						break;
				}
				if (!acked) {
					++failures;
					//cdc_write_line("Got no ack, bro.");
					cdc_write_line("Failed");
				} else {
					cdc_log_int("Round trip: ", rtc_get_time() - send_time);
				}
			}

			cdc_log_int("Time to send 255 frames: ", rtc_get_time() - start_time);
			cdc_log_int("Failures: ", failures);
			//_delay_ms(3000);
		}

	} else if (mode == 'R') {
		//cdc_write_string("\nRead mode.");
		uint8_t senderId;
		uint8_t length;
		uint8_t rbuf[256];
		bool needsAck;

		uint32_t p = 0;
		uint16_t now = rtc_get_time();
		while(1) {
			if (rtc_get_time() - now > 1000) {
				now = rtc_get_time();
				//cdc_write_line("MAIN LOOP");
			}
			while (framesToRead()) {
				cdc_log_int("frames: ", p);
				++p;
				getNextFrame(&senderId, &length, rbuf, &needsAck);
				if (needsAck) {
					sendAck(senderId);
					cdc_write_line("Sending ack.");
				}
				cdc_log_int("From: ", senderId);
				cdc_newline();
			}

		}

    } else if (mode == 'F') {

    	// Play around with FIFO

    	fifo_t F;

    	fifo_init(&F);

    	fifo_write(&F, payload, 30);
    	fifo_write(&F, payload, 30);

    	//uint8_t buf[FIFO_UNIT_LEN];

    	fifo_read(&F);

    }

	while (1);
}
