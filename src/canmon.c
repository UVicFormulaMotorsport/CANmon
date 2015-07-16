#include "canmon.h"
	

int
main(void)
{
	uint8_t byte, checksum = 0, sequence = 0, length = 0, index = 0;

	enum {
		MSG_STATE_START = 1,
		MSG_STATE_SEQ,
		MSG_STATE_LENGTH,
		MSG_STATE_BODY,
		MSG_STATE_CHECKSUM,
		MSG_STATE_ERROR,
		MSG_STATE_COMPLETE
	} msg_state;

	char message[CM_MSG_MAX_LENGTH];
	char response[CM_MSG_MAX_LENGTH];

	cm_uart_init();

	sei();

	for (;;) {

		/*********************************************************************** 
		 * Read Message
		 **********************************************************************/

		/* Start state machine waiting for start byte */
		msg_state = MSG_STATE_START;

		/* Run state machine until the received message needs processing */
		while (msg_state != MSG_STATE_COMPLETE) {

			byte = cm_uart_read();

			if (byte == 0) continue;

			switch (msg_state) {

				/* Initial (reset) state, read start byte */
				case MSG_STATE_START:
					if (byte == CM_MSG_START) {
						msg_state = MSG_STATE_SEQ;
						checksum = CM_MSG_START ^ 0;
					}
					break;

				/* Read sequence byte */
				case MSG_STATE_SEQ:
					sequence = byte;
					msg_state = MSG_STATE_LENGTH;
					checksum ^= byte;
					break;

				/* Read message length */
				case MSG_STATE_LENGTH:
					length = byte;

					if (length > CM_MSG_MAX_LENGTH) {
						msg_state = MSG_STATE_ERROR;
					} else {
						msg_state = MSG_STATE_BODY;
						index = 0;
						checksum ^= byte;
					}

					break;

				/* Read message body bytes */
				case MSG_STATE_BODY:
					message[index++] = byte;
					checksum ^= byte;

					/* Check for ESC -- state machine should be reset */
					if (byte == CM_MSG_START) msg_state = MSG_STATE_START;

					if (index == length) msg_state = MSG_STATE_CHECKSUM;
					break;

				/* Read checksum byte */
				case MSG_STATE_CHECKSUM:

					// TODO: Read and process checksum

					msg_state = MSG_STATE_COMPLETE;
					break;

				/* Message error */
				case MSG_STATE_ERROR:

					// TODO: Handle error

					msg_state = MSG_STATE_COMPLETE;
					break;

				/* Complete, so start processing */
				case MSG_STATE_COMPLETE:
					break;
			}
		}


		/*********************************************************************** 
		 * Process Message
		 **********************************************************************/

		switch (message[0]) {
			case MSG_COMMAND_IDENT:
				length = 6;
				strcpy(response, "CANmon");
				break;
		}


		/*********************************************************************** 
		 * Send Response
		 **********************************************************************/

		/* Write start byte */
		cm_uart_write(CM_MSG_START);
		checksum = CM_MSG_START ^ 0;

		/* Write sequence number of message this response is for */
		cm_uart_write(sequence);
		checksum ^= sequence;

		/* Write response length */
		cm_uart_write(length);
		checksum ^= length;

		/* Write response body bytes */
		for (index = 0; index < length; index++) {
			cm_uart_write(response[index]);
			checksum ^= response[index];
		}

		/* Write response checksum */
		cm_uart_write(checksum);
	}

	return 0;
}