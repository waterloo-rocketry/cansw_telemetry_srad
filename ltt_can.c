/*
 * File:   ltt_can.c
 * Author: Manav
 *
 * Created on April 16, 2026, 9:20 AM
 * 
 * All interfacing with RocketCAN done here
 */

#include "ltt_can.h"
#include "adc.h"
#include "channel_info.h"
#include "channels.h"
#include "leds.h"
#include "config.h"

#include "canlib.h"
#include "timer.h"

#include <xc.h>
#include <string.h>

#define CAN_MESSAGE_PERIOD_MS 1000

// memory pools for the CAN buffer
static uint8_t tx_pool[sizeof(can_msg_t)*128];
static uint8_t rx_pool[sizeof(can_msg_t)*128];

// last transmit time
static uint32_t last_transmit;

// store the last message sent so it's not forwarded back to air
static can_msg_t last_can_message_sent;

static void can_msg_handler(const can_msg_t *msg) {
    uint16_t msg_type = get_message_type(msg);

    if(msg_type == MSG_TELEMETRY_STATE_SWITCH) return; // this should be internal only

    if(memcmp(msg, &last_can_message_sent, msg->data_len + 5)) {
        // message is not what we just sent
        rcvb_push_message(msg);
    }

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

static void can_send_wrapper(const can_msg_t *msg) {
    last_can_message_sent = *msg;
    pic18f26k83_can_send(msg);
}

void CAN_Init(void) {
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

    // set up CAN buffer
    txb_init(tx_pool, sizeof(tx_pool), can_send_wrapper, pic18f26k83_can_send_rdy);
    rcvb_init(rx_pool, sizeof (rx_pool));

    last_transmit = 0;
}

void CAN_send_messages(void) {
    static int blinky = 0;

    uint32_t now = millis();
    if(now - last_transmit > CAN_MESSAGE_PERIOD_MS) {
        can_msg_t msg;
        uint32_t error_bitfield = 0;
        uint16_t current_sense_val = ADC_read_curr_filter();

        // Send overcurrent warning if current over 0.8A
        if (current_sense_val >= 800) {
            error_bitfield |= E_12V_OVER_CURRENT_OFFSET;
        }

        build_analog_sensor_16bit_msg(PRIO_LOW, (uint16_t) now, SENSOR_12V_CURR, current_sense_val, &msg);
        CAN_enqueue(&msg);

        build_general_board_status_msg(error_bitfield ? PRIO_HIGH : PRIO_LOW, (uint16_t) now, error_bitfield, &msg);
        CAN_enqueue(&msg);

        for(uint8_t i = 0; i < channel_remote_count(); i++) {
            uint8_t rssi = 0, lqi = 0;
            channel_info_get(i, &rssi, &lqi);
            build_telemetry_info_msg(PRIO_MEDIUM, (uint16_t) now, channel_remote_from_index(i), lqi, rssi, &msg);
            CAN_enqueue(&msg);
        }

        last_transmit = now;
        toggle_LED_Blue(blinky++ % 2);
    }

    txb_heartbeat();
}

// LTT internal can send that sends to both CAN and over the air
void CAN_enqueue(const can_msg_t *msg) {
    txb_enqueue(msg);
    rcvb_push_message(msg);
}
