#include "canmon.h"

int main(void) {
	uint8_t byte = 65;

	cm_uart_init();
	
	for (;;) {
		cm_uart_send(byte);

		if (byte == 90) byte = 65;
		else byte++;
	}

	return 0;
}