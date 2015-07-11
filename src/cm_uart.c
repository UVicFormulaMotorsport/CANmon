#include "canmon.h"

void
cm_uart_init(void) {
	/* Set transmit pin as output */
	DDRE = (1 << DDE1);

	/* Enable transmit and receive */
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);

	/* Set baud rate */
	UBRR0L  = UBRRL_VALUE;
	UBRR0H  = UBRRH_VALUE;
}

void
cm_uart_send(uint8_t byte) {
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = byte;
}