#include "canmon.h"


int
main(void)
{
	uint8_t rx_byte = 0;
	uint8_t tx_byte = 65;

	cm_uart_init();

	sei();
	
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