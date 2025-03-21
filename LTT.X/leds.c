/*
 * File:   LEDs.c
 * Author: Manav
 *
 * Created on February 22, 2025, 11:54 AM
 */

#include "leds.h"

void LEDs_Init() {
    // Set LEDs as output
    TRISAbits.TRISA2 = 0; // green
    TRISAbits.TRISA3 = 0; // blue
    TRISAbits.TRISA4 = 0; // red
}

void toggle_LED_Green (bool LED_On) {
    LATA2 = LED_On;
}

void toggle_LED_Blue (bool LED_On) {
    LATA3 = LED_On;
}

void toggle_LED_Red (bool LED_On) {
    LATA4 = LED_On;
}
