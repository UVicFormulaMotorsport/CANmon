#include "canmon.h"


static cm_msg_state_e cm_msg_state;


int
main(void)
{
	uint8_t byte, checksum = 0, sequence = 0, length = 0, index = 0;
	uint8_t message[64];

	cm_uart_init();

	sei();

	for (;;) {
		/* Start state machine waiting for start byte */
		cm_msg_state = MSG_STATE_READ_START;

		/* Run state machine until the received message needs processing */
		while (cm_msg_state != MSG_STATE_COMPLETE) {

			byte = cm_uart_read();

			switch (cm_msg_state) {
				case MSG_STATE_READ_START:
					if (byte == CM_MSG_START) {
						cm_msg_state = MSG_STATE_READ_SEQ;
						checksum = CM_MSG_START ^ 0;
					}
					break;

				case MSG_STATE_READ_SEQ:
					sequence = byte;
					cm_msg_state = MSG_STATE_READ_LENGTH;
					checksum ^= byte;
					break;

				case MSG_STATE_READ_LENGTH:
					length = byte;
					cm_msg_state = MSG_STATE_READ_BODY;
					index = 0;
					checksum ^= byte;
					break;

				case MSG_STATE_READ_BODY:
					message[index++] = byte;
					checksum ^= byte;

					if (index == length) cm_msg_state = MSG_STATE_READ_CHECKSUM;

					break;

				case MSG_STATE_READ_CHECKSUM:
					break;

				case MSG_STATE_COMPLETE:
					break;
			}
		}

		switch (message[0]) {

		}

		cm_uart_write(CM_MSG_START);
		checksum = CM_MSG_START ^ 0;

		cm_uart_write(sequence);
		checksum ^= sequence;

		cm_uart_write(length);
		checksum ^= length;

		cm_uart_write(checksum);
	}

	uint8_t rx_byte = 0;
	uint8_t tx_byte = 65;

	for (;;) {
		rx_byte = cm_uart_read();
		if (rx_byte);

		do {
			cm_uart_write(tx_byte);
		} while (cm_uart_err == E_UART_TX_BUF_FULL);

		if (tx_byte == 90) tx_byte = 65;
		else tx_byte++;
	}

	return 0;
}