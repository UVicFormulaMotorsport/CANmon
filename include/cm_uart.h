#ifndef __CM_UART_H_
#define __CM_UART_H__


#define CM_UART_TX_BUF_SIZE 64
#define CM_UART_RX_BUF_SIZE 64


typedef enum {
	E_UART_UNKNOWN = 1,
	E_UART_TX_BUF_EMPTY,
	E_UART_TX_BUF_FULL,
	E_UART_RX_BUF_EMPTY,
	E_UART_RX_BUF_FULL,
	E_UART_RX_OVERRUN,
	E_UART_FRAME_ERROR,
	E_UART_PARITY_ERROR
} cm_uart_err_e;


void cm_uart_init(void);
void cm_uart_write(uint8_t byte);
uint8_t cm_uart_read(void);

extern volatile cm_uart_err_e cm_uart_err;


#endif