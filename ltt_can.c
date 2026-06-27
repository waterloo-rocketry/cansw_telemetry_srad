/*
 * File:   ltt_can.c
 * Author: Manav
 *
 * Created on April 16, 2026, 9:20 AM
 * 
 * All interfacing with RocketCAN done here
 */

#include "config.h"
#include "ltt_can.h"
#include "adc.h"
#include "osc.h"
#include "leds.h"
#include "status_tracker.h"

#include <xc.h>
#include "canlib.h" // interface with RocketCAN
#include "timer.h" // import custom millis() function

// memory pool for the CAN tx buffer
uint8_t tx_pool[200];
// memory pool for CAN rx buffer
PriorityQueue ltt_can_queue;

static void can_msg_handler(const can_msg_t *msg) {
    if(msg_type != MSG_TELEMETRY_STATE_SWITCH) {
        pq_push(&ltt_can_queue,msg);
    }

    // For parsing commands to LTT board
    uint16_t msg_type = get_message_type(msg);

    switch (msg_type) {
        case MSG_LEDS_ON:
            toggle_LED_Green(1);
            toggle_LED_Red(1);
            toggle_LED_Blue(1);
            break;

        case MSG_LEDS_OFF:
            toggle_LED_Green(0);
            toggle_LED_Red(0);
            toggle_LED_Blue(0);
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
    pic18f26k83_can_init(&can_setup, can_msg_handler);
    pq_init(&ltt_can_queue);

    // set up CAN tx buffer
    txb_init(tx_pool, sizeof(tx_pool), pic18f26k83_can_send, pic18f26k83_can_send_rdy);
}

void send_board_status(uint8_t status) {
    can_msg_prio_t prio;
    can_msg_type_t msg_type = MSG_GENERAL_BOARD_STATUS;
    uint32_t error_bitfield;
    can_msg_t status_msg;

    // no error, set bit fields to 0
    if (status == 0x00) {
        prio = PRIO_LOW;
        error_bitfield = 0x00;
    } // Over current
    else if (status == 0x01) {
        prio = PRIO_HIGH;
        error_bitfield = E_12V_OVER_CURRENT_OFFSET; // 12V_OVER_CURRENT
    }
    
    build_general_board_status_msg(prio, millis(), error_bitfield, &status_msg);
    pic18f26k83_can_send(&status_msg);
}

void send_current_reading(uint8_t *board_status) {
    uint16_t current_sense_val;
    current_sense_val = read_ADC();
    current_sense_val = (uint16_t)((current_sense_val / 100)/ 0.025f); // fixed cast, 25mR shunt

    can_msg_prio_t prio = PRIO_LOW;
    can_analog_sensor_id_t current_reading_CAN_msgid = SENSOR_12V_CURR;
    can_msg_t current_reading_msg;

    build_analog_sensor_16bit_msg(
        prio, millis(), current_reading_CAN_msgid, current_sense_val, &current_reading_msg
    );
    pic18f26k83_can_send(&current_reading_msg);

    // Send overcurrent warning if current over 0.8A
    if (current_sense_val >= 8000) {
        *board_status = 0x01;
    }
}

//function user must guarantee channel_id is valid
void send_telemetry_info(can_msg_prio_t prio, uint16_t timestamp, uint8_t channel_id)
{
    can_msg_t telemetry_info_msg;
    
    uint8_t rssi=get_telemetry_channel_rssi(channel_id);
    uint8_t lqi=get_telemetry_channel_lqi(channel_id);
    
    build_telemetry_info_msg(prio,timestamp,channel_id,lqi,rssi,&telemetry_info_msg);
    
    pic18f26k83_can_send(&telemetry_info_msg);
}