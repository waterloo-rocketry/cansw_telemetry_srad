/*
 * File:   main.c
 * Author: Manav
 *
 * Created on February 22, 2025, 11:40 AM
 */

#include "adc.h"
#include "clockInit.h"
#include "leds.h"
#include "spi.h"
#include "cc1200.h"
#include <xc.h>

#include "canlib.h"

#pragma config WDTE = OFF // Watchdog Timer disabled

#define _XTAL_FREQ 12000000 // 12 MHz

static void can_msg_handler(const can_msg_t *msg);
static void send_status_ok(void);
// memory pool for the CAN tx buffer
uint8_t tx_pool[200];

static void can_msg_handler(const can_msg_t *msg) {
    uint16_t msg_type = get_message_type(msg);

    switch (msg_type) {
        case MSG_LEDS_ON:
            toggle_LED_Green(1);
            break;

        case MSG_LEDS_OFF:
            toggle_LED_Green(1);
            break;

        default:
            break;
    }
}

void CAN_Init() {
    // Set up CAN TX
    TRISC1 = 0;
    RC1PPS = 0x33;

    // Set up CAN RX
    TRISC0 = 1;
    ANSELC0 = 0;
    CANRXPPS = 0x10;

    // set up CAN module
    can_timing_t can_setup;
    can_generate_timing_params(_XTAL_FREQ, &can_setup);
    can_init(&can_setup, can_msg_handler);

    // set up CAN tx buffer
    txb_init(tx_pool, sizeof(tx_pool), can_send, can_send_rdy);
}

void Board_Init() {
    LEDs_Init();
    Osc_Init();
    ADC_Init();
    SPI_Init();
    CAN_Init();
}

void Send_Current_Reading() {
    uint16_t current_sense_val;
    current_sense_val = read_ADC();
    current_sense_val = current_sense_val / 0.025; // Shunt resistor is 25mR

    uint16_t time = 0; // CHANGE LATER
    can_msg_prio_t current_reading_CAN_priority = PRIO_LOW;
    can_analog_sensor_id_t current_reading_CAN_msgid = SENSOR_12V_CURR;
    can_msg_t current_reading_msg;

    build_analog_data_msg(current_reading_CAN_priority, time, current_reading_CAN_msgid, current_sense_val, &current_reading_msg);
    can_send(&current_reading_msg);
}

void main() {
    Board_Init();

    toggle_LED_Green(0);
    toggle_LED_Blue(0);
    toggle_LED_Red(0);
    
    while (1) {
        CLRWDT();

        __delay_ms(1000);

        Send_Current_Reading();

        // Confirm SPI Works
        // Read part number register 0x8F
        // Should be 0x20
        CC1200ReadResult cc12_read;
        cc12_read = Read_CC1200(0x8F);
        uint8_t read_val = cc12_read.value; 
        
        if (read_val == 0x20) {
            toggle_LED_Green(1);
            toggle_LED_Blue(1);
            toggle_LED_Red(1);
        }
        
        uint16_t time = 0;
        can_msg_prio_t prio = PRIO_HIGH;
        can_msg_t debugMsg;

        build_debug_raw_msg(prio, time, &read_val, &debugMsg);
        can_send(&debugMsg);
    }
}
