#include "canmon.h"
	

int
main(void)
{
	uint8_t byte, checksum = 0, sequence = 0, length = 0, index = 0;

	enum {
		CMD_STATE_START = 1,
		CMD_STATE_SEQ,
		CMD_STATE_LENGTH,
		CMD_STATE_BODY,
		CMD_STATE_CHECKSUM,
		CMD_STATE_ERROR,
		CMD_STATE_COMPLETE
	} cmd_state;

	char command_body[CM_MSG_MAX_LENGTH];
	char reply_body[CM_MSG_MAX_LENGTH];

	cm_uart_init();

	sei();

	for (;;) {

		/*********************************************************************** 
		 * Stream CAN Messages
		 **********************************************************************/

		/*********************************************************************** 
		 * Read Commands
		 **********************************************************************/

		/* Start state machine waiting for start byte */
		cmd_state = CMD_STATE_START;

		/* Run state machine until the received command needs processing */
		while (cmd_state != CMD_STATE_COMPLETE) {

			byte = cm_uart_read();

			if (cm_uart_err == E_UART_RX_BUF_EMPTY) continue;

			switch (cmd_state) {

				/* Initial (reset) state, read start byte */
				case CMD_STATE_START:
					if (byte == CM_MSG_COMMAND) {
						cmd_state = CMD_STATE_SEQ;
						checksum = CM_MSG_COMMAND ^ 0;
					}
					break;

				/* Read sequence byte */
				case CMD_STATE_SEQ:
					sequence = byte;
					cmd_state = CMD_STATE_LENGTH;
					checksum ^= byte;
					break;

				/* Read message length */
				case CMD_STATE_LENGTH:
					length = byte;

					if (length > CM_MSG_MAX_LENGTH) {
						cmd_state = CMD_STATE_ERROR;
					} else {
						cmd_state = CMD_STATE_BODY;
						index = 0;
						checksum ^= byte;
					}

					break;

				/* Read message body bytes */
				case CMD_STATE_BODY:
					command_body[index++] = byte;
					checksum ^= byte;

					if (index == length) cmd_state = CMD_STATE_CHECKSUM;
					break;

				/* Read checksum byte */
				case CMD_STATE_CHECKSUM:

					// TODO: Read and process checksum

					cmd_state = CMD_STATE_COMPLETE;
					break;

				/* Message error */
				case CMD_STATE_ERROR:

					// TODO: Handle error

					cmd_state = CMD_STATE_COMPLETE;
					break;

				/* Complete, so start processing */
				case CMD_STATE_COMPLETE:
					break;
			}
		}


		/*********************************************************************** 
		 * Process Command
		 **********************************************************************/

		switch (command_body[0]) {
			case CM_CMD_IDENT:
				length = 6;
				strcpy(reply_body, "CANmon");
				break;
		}


		/*********************************************************************** 
		 * Send Reply
		 **********************************************************************/

		/* Write start byte */
		cm_uart_write(CM_MSG_REPLY);
		checksum = CM_MSG_REPLY ^ 0;

		/* Write sequence number of message this response is for */
		cm_uart_write(sequence);
		checksum ^= sequence;

		/* Write response length */
		cm_uart_write(length);
		checksum ^= length;

		/* Write response body bytes */
		for (index = 0; index < length; index++) {
			cm_uart_write(reply_body[index]);
			checksum ^= reply_body[index];
		}

		/* Write response checksum */
		cm_uart_write(checksum);
	}

	return 0;
}