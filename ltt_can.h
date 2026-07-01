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

void CAN_Init(void);
void CAN_send_messages(void);
void CAN_enqueue(const can_msg_t*);

#endif	/* XC_HEADER_TEMPLATE_H */

