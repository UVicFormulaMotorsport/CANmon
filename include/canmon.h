#ifndef __CANMON_H_
#define __CANMON_H__


#define CANMON_VERSION "0.1.0"

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

/* Message header bytes */
#define CM_MSG_COMMAND 0x1B
#define CM_MSG_REPLY   0x1C
#define CM_MSG_STREAM  0x1D

/* System configuration commands */
#define CM_CMD_IDENT 0x3F
#define CM_CMD_START 0x30
#define CM_CMD_STOP  0x31

/* CAN bus commands */
#define CM_CMD_CAN_STATUS 0x4F
#define CM_CMD_CAN_SEND   0x40
#define CM_CMD_CAN_RECV   0x41
#define CM_CMD_CAN_FILTER 0x42

/* Reset device */
#define CM_CMD_RESET 0xFF


#endif