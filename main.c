/*
 * File:   main.c
 * Author: Manav
 *
 * Created on February 22, 2025, 11:40 AM
 */

#include <xc.h>
#include <string.h>

#include "ltt_rf.h"
#include "adc.h" // interface with ADC
#include "cc1200.h" // interface with CC1200
#include "osc.h" // initialize XTAL
#include "leds.h" // interface with LEDs
#include "statemachine.h"
#include "config.h"

// rocketlib and canlib
#include "canlib.h" // interface with RocketCAN

#define TEST_SIZE 32

void Board_Init() {
    timer0_init();
    LEDs_Init();
    Osc_Init();
    ADC_Init();
    SPI_Init();
    CAN_Init();
    RF_Init();
    SM_Init();
}

void main() {
    Board_Init();

    toggle_LED_Green(0);
    toggle_LED_Blue(0);
    toggle_LED_Red(1);

    while (1) {
        CLRWDT();

        SM_LTT_State_Machine();

        uint8_t board_status = 0; // board status flag
        send_current_reading(&board_status);
        send_board_status(board_status);
    }
}

static void __interrupt() interrupt_handler(void) {
    if (PIR5) {
        pic18f26k83_can_handle_interrupt();
    }

    // Timer0 has overflowed - update millis() function
    // This happens approximately every 500us
    if (PIE3bits.TMR0IE == 1 && PIR3bits.TMR0IF == 1) {
        timer0_handle_interrupt();
        PIR3bits.TMR0IF = 0;
    }
}
