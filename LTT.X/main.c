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

void Board_Init() {
    LEDs_Init();
    ADC_Init();
    SPI_Init();
}

void main(void) {
    Board_Init();
    
    return;
}
