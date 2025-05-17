/*
 * File:   clock_init.c
 * Author: Manav
 *
 * Created on March 21, 2025, 4:34 PM
 */

#include "clockInit.h"

void Osc_Init(void) {
#pragma config FEXTOSC = HS // External Oscillator Selection (HS (crystal oscillator) above 8 MHz;
                            // PFM set to high power)

    // Select external oscillator with PLL of ??
    OSCCON1 = 0x70;
    // wait until the clock switch has happened
    while (OSCCON3bits.ORDY == 0) {}
    // if the currently active clock (CON2) isn't the selected clock (CON1)
    if (OSCCON2 != 0x20) {
        // try to wait for the oscillator to come back
        for (uint16_t i = 0; i < 60000; i++) {}
    }
}
