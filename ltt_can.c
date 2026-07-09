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
#include "statemachine.h"
#include "config.h"

#include "canlib.h"
#include "timer.h"

#include <xc.h>
#include <string.h>

#define CAN_MESSAGE_PERIOD_MS 500
#define OVER_CURRENT_THRESHOLD 200

// memory pools for the CAN buffer
static uint8_t tx_pool[sizeof(can_msg_t)*128];
static uint8_t rx_pool[sizeof(can_msg_t)*128];

// last transmit time
static uint32_t last_transmit;

// store the last message sent so it's not forwarded back to air
static can_msg_t last_can_message_sent;

// for blinking heartbeat LED
static int blinky = 0;

static void can_msg_handler(const can_msg_t *msg) {
    can_msg_type_t msg_type = get_message_type(msg);

    if(msg_type == MSG_TELEMETRY_STATE_SWITCH) return; // this should be internal only

    if(memcmp(msg, &last_can_message_sent, msg->data_len + 5)) {
        // message is not what we just sent
        rcvb_push_message(msg);
    }

    switch(msg_type) {
        case MSG_LEDS_ON:
            LED_set_Green(1);
            LED_set_Red(1);
            LED_set_Blue(1);
            break;

        case MSG_LEDS_OFF:
            LED_set_Green(0);
            LED_set_Red(0);
            LED_set_Blue(0);
            break;

        case MSG_ACTUATOR_CMD: {
            if(!channel_is_rocket()) break;

            can_actuator_id_t actuator_id = ACTUATOR_ENUM_MAX;
            can_actuator_state_t actuator_state = ACT_STATE_ILLEGAL;

            get_actuator_id(msg, &actuator_id);
            get_cmd_actuator_state(msg, &actuator_state);

            if(actuator_id == ACTUATOR_TELEMETRY) {
                switch(actuator_state) {
                    case ACT_STATE_ON:
                        SM_LTT_Stop_TX(false);
                        break;
                    case ACT_STATE_OFF:
                        SM_LTT_Stop_TX(true);
                        break;
                    default:
                        break;
                }
            }
            break;
        }

        case MSG_RESET_CMD: {
            bool need_reset = false;
            check_board_need_reset(msg, &need_reset);
            if(need_reset) RESET();
            break;
        }

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
    uint32_t now = millis();
    if(now - last_transmit > CAN_MESSAGE_PERIOD_MS) {
        can_msg_t msg;
        uint32_t error_bitfield = 0;
        uint16_t current_sense_val = ADC_read_curr_filter();

        // Send overcurrent warning if current over threshold
        if (current_sense_val >= OVER_CURRENT_THRESHOLD) {
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
        LED_set_Blue(blinky++ % 2);
    }

    txb_heartbeat();
}

// LTT internal can send that sends to both CAN and over the air
void CAN_enqueue(const can_msg_t *msg) {
    txb_enqueue(msg);
    rcvb_push_message(msg);
}
