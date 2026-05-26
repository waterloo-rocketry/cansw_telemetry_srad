/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H
#include "priority_queue.h"
extern CAN_PriorityQueue tx_queue;

#include <xc.h> // include processor files - each processor file is guarded.  

void CAN_Init();

void send_board_status(uint8_t status);

void send_current_reading(uint8_t *board_status);

#endif	/* XC_HEADER_TEMPLATE_H */

