/*
 * File:   adc.h
 * Author: Manav
 *
 * Created on February 22, 2025, 12:46 PM
 */

#ifndef ADC_H
#define ADC_H

#include <xc.h>

void ADC_Init(void);
uint16_t ADC_read_raw(void);
uint16_t ADC_read_curr_ma(void);
uint16_t ADC_read_curr_filter(void);
void ADC_run_filter(void);

#endif
