/*
 * File:   main.c
 * Author: Manav
 *
 * Created on February 22, 2025, 11:40 AM
 */

#include <xc.h>
#include "leds.h"
#include "adc.h"
#include "spi.h"
#include "cc1200.h"
#include "canlib/can.h"
#include "canlib/message_types.h"

#define _XTAL_FREQ 12000000 //12MHz

void Board_Init() {
    LEDs_Init();
    ADC_Init();
    SPI_Init();
}

void main(void) {
    Board_Init();
    
    Read_CC1200(0x3D);
    
    
    return;
}
