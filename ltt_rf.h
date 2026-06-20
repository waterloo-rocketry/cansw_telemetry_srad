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

#include <xc.h> // include processor files - each processor file is guarded.
#include <stdint.h>

void CC1200_Set_Power(int8_t power);

void CC1200_Frequency(uint32_t freq);

void RF_Init(void);

void RF_Receive(void);
#endif	/* XC_HEADER_TEMPLATE_H */

