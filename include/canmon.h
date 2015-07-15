#ifndef __CANMON_H_
#define __CANMON_H__


#define BAUD 500000


#include <stdint.h>

#include <avr/interrupt.h>
#include <avr/io.h>

#include <util/atomic.h>
#include <util/setbaud.h>

#include "cm_can.h"
#include "cm_uart.h"


#define CM_MSG_START 0x1B

typedef enum {
	MSG_STATE_READ_START = 1,
	MSG_STATE_READ_SEQ,
	MSG_STATE_READ_LENGTH,
	MSG_STATE_READ_BODY,
	MSG_STATE_READ_CHECKSUM,
	MSG_STATE_COMPLETE
} cm_msg_state_e;


#endif