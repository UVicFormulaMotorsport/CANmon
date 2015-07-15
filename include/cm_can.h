#ifndef __CM_CAN_H_
#define __CM_CAN_H__


typedef enum {
	E_CAN_UNKNOWN = 1
} cm_can_err_e;

typedef enum {
	MOB_0,
	MOB_1,
	MOB_2,
	MOB_3,
	MOB_4,
	MOB_5
} cm_can_mob_e;


void cm_can_init(void);


#endif