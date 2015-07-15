#include "canmon.h"


#define CM_UART_TX_BUF_MASK (CM_UART_TX_BUF_SIZE - 1)
#define CM_UART_RX_BUF_MASK (CM_UART_RX_BUF_SIZE - 1)


static volatile uint8_t cm_uart_tx_buf[CM_UART_TX_BUF_SIZE];
static volatile uint8_t cm_uart_rx_buf[CM_UART_RX_BUF_SIZE];

static volatile uint8_t cm_uart_tx_buf_head;
static volatile uint8_t cm_uart_tx_buf_tail;
static volatile uint8_t cm_uart_rx_buf_head;
static volatile uint8_t cm_uart_rx_buf_tail;

volatile cm_uart_err_e cm_uart_err = 0;


void
cm_uart_init(void)
{
	/* Initialize empty circular buffer */
	cm_uart_tx_buf_head = 0;
	cm_uart_tx_buf_tail = 0;
	cm_uart_rx_buf_head = 0;
	cm_uart_rx_buf_tail = 0;

	/* Set transmit pin as output */
	DDRE = (1 << DDE1);

	/* Enable transmit and receive, receive interrupt */
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);

#if USE_2X
	/* Use double transfer rate */
	UCSR0A |= (1 << U2X);
#endif

	/* Set baud rate */
	UBRR0L = UBRRL_VALUE;
	UBRR0H = UBRRH_VALUE;

	/* TODO: Set frame format */
}


void
cm_uart_write(uint8_t byte)
{
	uint8_t head;

	/* Calculate new TX buffer head */
	head = (cm_uart_tx_buf_head + 1) & CM_UART_TX_BUF_MASK;

	if (head == cm_uart_tx_buf_tail) {
		/* Error: TX buffer full */
		cm_uart_err = E_UART_TX_BUF_FULL;

		/* TODO: Handle this condition better? */
		return;
	} else {
		/* Clear any previous error */
		cm_uart_err = 0;
	}

	/* Write byte to the TX buffer and save new TX buffer head */
	cm_uart_tx_buf[head] = byte;
	cm_uart_tx_buf_head = head;

	/* Enable DRE interrupt */
	UCSR0B |= (1 << UDRIE0);
}


uint8_t
cm_uart_read(void)
{
	uint8_t tail;

	if (cm_uart_rx_buf_head == cm_uart_rx_buf_tail) {
		/* Error: RX buffer empty */
		cm_uart_err = E_UART_RX_BUF_EMPTY;
		
		/* Nothing to return, so return nothing */
		return 0;
	} else {
		/* Clear any previous error */
		cm_uart_err = 0;
	}

	/* Calculate and save new RX buffer tail */
	tail = (cm_uart_rx_buf_tail + 1) & CM_UART_RX_BUF_MASK;
	cm_uart_rx_buf_tail = tail;

	/* Read byte from the RX buffer */
	return cm_uart_rx_buf[tail];
}


ISR(USART0_UDRE_vect)
{
	uint8_t tail;

	if (cm_uart_tx_buf_head == cm_uart_tx_buf_tail) {
		/* TX buffer empty, disable DRE interrupt */
		UCSR0B &= ~(1 << UDRIE0);
	} else {
		/* Calculate and save new TX buffer tail */
		tail = (cm_uart_tx_buf_tail + 1) & CM_UART_TX_BUF_MASK;
		cm_uart_tx_buf_tail = tail;

		/* Write byte from the TX buffer to the UART */
		UDR0 = cm_uart_tx_buf[tail];
	}
}


ISR(USART0_RX_vect)
{
	uint8_t head, byte, ucsr0a;

	/* Check UART status register */
	ucsr0a = UCSR0A;

	/* Flag any receive errors */
	if (ucsr0a & (1 << FE0)) {
		cm_uart_err = E_UART_FRAME_ERROR;
	} else if (ucsr0a & (1 << DOR0)) {
		cm_uart_err = E_UART_RX_OVERRUN;
	} else if (ucsr0a & (1 << UPE0)) {
		cm_uart_err = E_UART_PARITY_ERROR;
	}

	/* XXX: Should the above errors return, i.e. is the byte usable? */

	/* Copy byte from UART */
	byte = UDR0;

	/* Calculate new RX buffer head */
	head = (cm_uart_rx_buf_head + 1) & CM_UART_RX_BUF_MASK;

	if (head == cm_uart_rx_buf_tail) {
		/* Error: RX buffer full */
		cm_uart_err = E_UART_RX_BUF_FULL;
	} else {
		/* Clear any previous error */
		cm_uart_err = 0;

		/* Save new RX buffer head and received byte to RX buffer */
		cm_uart_rx_buf_head  = head;
		cm_uart_rx_buf[head] = byte;
	}
}