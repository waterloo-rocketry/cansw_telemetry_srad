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

uint16_t timestamp = 0; // Figure out how to sync clocks

static void can_msg_handler(const can_msg_t *msg);
static void send_status_ok(void);
// memory pool for the CAN tx buffer
uint8_t tx_pool[200];

// All messages received shall be transmitted
static void can_msg_handler(const can_msg_t *msg) {
    // For transmitting, we don't care what the message is or where it's from
    uint32_t msg_SID = msg->sid;
    uint8_t msg_len = msg->data_len;
    uint64_t msg_data;
    for (int i=0; i < 8; i++) {
        msg_data = (msg_data << 8) | msg->data[i];
    }

    CC1200_Transmit(msg_SID, msg_len, msg_data);
    
    // For parsing commands to LTT board
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

static void rf_msg_handler(uint64_t *callsign, uint32_t *sid, uint8_t *len, uint64_t *data) {
    can_msg_t *msg;
    msg->sid = *sid;
    msg->data_len = *len;
    uint8_t msg_data[8];
    for (int i = 7; i >= 0; i--) {
        msg_data[i] = *data & 0xff;
        *data >>= 8;
    }
    for (int i=0; i < 8; i++) {
        msg->data[i] = msg_data[i];
    }
    
    can_send(msg);
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
    //CC1200_Init();
}

void send_board_status() {
    can_msg_prio_t prio = PRIO_LOW;
    can_msg_type_t msg_type = MSG_GENERAL_BOARD_STATUS;
    can_msg_t healthy_msg;
    
    // error bit fields are 0 for healthy message
    build_general_board_status_msg(prio, timestamp, 0, 0, &healthy_msg);
    can_send(&healthy_msg);
}

void send_current_reading() {
    uint16_t current_sense_val;
    current_sense_val = read_ADC();
    current_sense_val = (uint8_t)(1000 * (current_sense_val / 0.025)); // Shunt resistor is 25mR

    can_msg_prio_t prio = PRIO_LOW;
    can_analog_sensor_id_t current_reading_CAN_msgid = SENSOR_12V_CURR;
    can_msg_t current_reading_msg;

    build_analog_data_msg(prio, timestamp, current_reading_CAN_msgid, current_sense_val, &current_reading_msg);
    can_send(&current_reading_msg);
    
    // Send overcurrent warning if current over 0.8A
    if (current_sense_val >= 8000) {
        can_msg_t over_curr_msg;
        prio = PRIO_HIGH;
        // Bit field for 12V_OVER_CUR = 0x03
        build_general_board_status_msg(prio, timestamp, 0x03, 0xff, &over_curr_msg);
        can_send(&over_curr_msg);
    }
}

void main() {
    Board_Init();

    toggle_LED_Green(0);
    toggle_LED_Blue(1);
    toggle_LED_Red(0);
    
    while (1) {
        CLRWDT();

        
        toggle_LED_Green(0);
        toggle_LED_Red(0);
        __delay_ms(1000);

        //send_current_reading();
        
        // This coe is to test SPI
        uint16_t time = 0; // CHANGE LATER
        can_msg_prio_t priority = PRIO_HIGH;
        can_analog_sensor_id_t msgid = SENSOR_12V_CURR;
        can_msg_t msg;
            
        bool is_SPI_working = false;
        
        is_SPI_working = is_CC1200();
        
        if (is_SPI_working == true) {
            toggle_LED_Green(1);
            toggle_LED_Red(1);
            
            build_analog_data_msg(priority, time, msgid, 0x20, &msg);
            can_send(&msg);
        }
        
        __delay_ms(500);
        build_analog_data_msg(priority, time, msgid, 0x00, &msg);
        can_send(&msg);      
        
    }
}
