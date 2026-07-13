/*
 * File:   main.c
 * Author: Manav
 *
 * Created on February 22, 2025, 11:40 AM
 */

#include "timer.h"
#include "adc.h" // interface with ADC
#include "cc1200.h" // interface with CC1200
#include "leds.h" // interface with LEDs
#include "statemachine.h"
#include "spi.h"
#include "channels.h"
#include "config.h"

#include <xc.h>
#include <string.h>

static void OSC_Init(void) {
    OSCCON1 = 0x70; // Fosc with division of 1
    while (OSCCON3bits.ORDY == 0) {}
}

static void board_Init(void) {
    timer0_init();
    LED_Init();
    OSC_Init();
    ADC_Init();
    SPI_Init();
    CAN_Init();
    CC1200_Init();
    SM_Init();
}

void main(void) {
    board_Init();

    CC1200_Set_Frequency(904000);
    CC1200_Set_Power(14);
    CC1200_Set_Ant_Diversity(!channel_is_rocket());

    LED_set_Green(0);
    LED_set_Blue(0);
    LED_set_Red(0);

    while (1) {
        CLRWDT();
        ADC_run_filter();
        SM_LTT_State_Machine();
        CAN_send_messages();
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
