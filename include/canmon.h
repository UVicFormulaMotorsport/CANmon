#ifndef __CANMON_H_
#define __CANMON_H__


#define BAUD 500000


#include <stdint.h>
#include <string.h>

#include <avr/interrupt.h>
#include <avr/io.h>

#include <util/atomic.h>
#include <util/setbaud.h>

#include "cm_can.h"
#include "cm_uart.h"


#define CM_MSG_MAX_LENGTH 32
#define CM_MSG_START 0x1B


/* System configuration commands */
#define MSG_COMMAND_IDENT 0x20

/* Data stream commands */
#define MSG_COMMAND_START 0x30
#define MSG_COMMAND_STOP  0x31

/* CAN bus monitor commands */
#define MSG_COMMAND_ADD_FILTER    0x40
#define MSG_COMMAND_REMOVE_FILTER 0x41
#define MSG_COMMAND_CLEAR_FILTERS 0x42


#endif