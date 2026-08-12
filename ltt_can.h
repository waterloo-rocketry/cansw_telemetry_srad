/*
 * File:
 * Author:
 * Comments:
 * Revision history:
 */

// This is a guard condition so that contents of this file are not included
// more than once.
#ifndef LTT_CAN_H
#define LTT_CAN_H

#include "canlib.h"
#include <xc.h> // include processor files - each processor file is guarded.

typedef enum {
	CAN_CONFIG_ID_FREQ = 0,
	CAN_CONFIG_ID_POWER = 1
} can_config_id_t;

void CAN_Init(void);
void CAN_send_messages(void);
void CAN_enqueue(const can_msg_t *);
void CAN_report_error(can_board_error_bitfield_offset_t);

#endif /* XC_HEADER_TEMPLATE_H */

