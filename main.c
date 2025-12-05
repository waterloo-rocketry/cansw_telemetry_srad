/*
 * File:   main.c
 * Author: Manav
 *
 * Created on February 22, 2025, 11:40 AM
 */
#include "config.h"
#include <xc.h>
#include <string.h>

#include "adc.h" // interface with ADC
#include "cc1200.h" // interface with CC1200
#include "clockInit.h" // initialize XTAL
#include "leds.h" // interface with LEDs

// rocketlib and canlib
#include "canlib.h" // interface with RocketCAN
#include "timer.h" // import custom millis() function

#define TEST_SIZE 32

uint8_t board_status = 0; // board status flag

// memory pool for the CAN tx buffer
uint8_t tx_pool[200];

// All messages received shall be transmitted
static void can_msg_handler(const can_msg_t *msg) {
    // For transmitting, we don't care what the message is or where it's from
    uint32_t msg_SID = msg->sid;
    uint8_t msg_len = msg->data_len;
    uint64_t msg_data;
    for (int i = 0; i < 8; i++) {
        msg_data = (msg_data << 8) | msg->data[i];
    }

    //CC1200_Transmit(msg_SID, msg_len, msg_data);

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

        // DEBUG RAW Message will be used for power control
        case MSG_DEBUG_RAW: {
            uint8_t debug_data[6];
            get_debug_raw_data(msg, debug_data);
            //CC1200_Set_Power(((int8_t) debug_data[0]) - 16);
            //CC1200_Frequency(debug_data[0]);
            break;
        }

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
    for (int i = 0; i < 8; i++) {
        msg->data[i] = msg_data[i];
    }

    pic18f26k83_can_send(msg);
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

    // set up CAN tx buffer
    txb_init(tx_pool, sizeof(tx_pool), pic18f26k83_can_send, pic18f26k83_can_send_rdy);
}

void Board_Init() {
    timer0_init();
    LEDs_Init();
    Osc_Init();
    ADC_Init();
    SPI_Init();
    CAN_Init();
    CC1200_Init();
}

void send_board_status(uint8_t status) {
    can_msg_prio_t prio;
    can_msg_type_t msg_type = MSG_GENERAL_BOARD_STATUS;
    can_msg_t status_msg;
    uint16_t error = (status != 0 ? 0xff : 0);
    uint32_t error_bitfield;

    // no error, set bit fields to 0
    if (status == 0x00) {
        prio = PRIO_LOW;
        error_bitfield = 0x00;
    } // Over current
    else if (status == 0x01) {
        prio = PRIO_HIGH;
        error_bitfield = 0x03; // 12V_OVER_CURRENT
    }

    // error bit fields are 0 for healthy message
    build_general_board_status_msg(prio, millis(), error_bitfield, error, &status_msg);
    pic18f26k83_can_send(&status_msg);
}

void send_current_reading() {
    uint16_t current_sense_val;
    current_sense_val = read_ADC();
    current_sense_val = (uint8_t)(1000 * (current_sense_val / 0.025)); // Shunt resistor is 25mR

    can_msg_prio_t prio = PRIO_LOW;
    can_analog_sensor_id_t current_reading_CAN_msgid = SENSOR_12V_CURR;
    can_msg_t current_reading_msg;

    build_analog_data_msg(
        prio, millis(), current_reading_CAN_msgid, current_sense_val, &current_reading_msg
    );
    pic18f26k83_can_send(&current_reading_msg);

    // Send overcurrent warning if current over 0.8A
    if (current_sense_val >= 8000) {
        board_status = 0x01;
    }
}

void main() {
    Board_Init();

    toggle_LED_Green(0);
    toggle_LED_Blue(1);
    toggle_LED_Red(0);

    uint32_t last_millis = millis();

    uint8_t status[6] = {0};
    uint8_t last_status[6] = {0};

    while (1) {
        CLRWDT();

        // send_board_status();
        // send_current_reading();

        // This code is to test SPI

        can_msg_prio_t priority = PRIO_HIGH;
        can_analog_sensor_id_t msgid = SENSOR_12V_CURR;
        can_msg_t msg;

        uint8_t state = CC1200_State_Transition();

        // Receive
        uint8_t data[64] = {0};
        uint8_t rx_len = CC1200_Receive(data, sizeof(data));
        static uint8_t test_count = 0;
        static uint16_t error_count = 0;
        static uint16_t total_count = 0;
        static uint16_t bit_count;

        if (rx_len) {
            // counter increments by one and crc check passes
            if (/*data[1] - test_count != 1 || */!(data[rx_len-1] & 0x80)) {
                error_count++;
            }
            total_count++;
            bit_count += (rx_len-3)*8;

            // overflow
            if (total_count == 0) {
                error_count = 0;
            }

            //build_debug_raw_msg(priority, millis(), data+rx_len-6, &msg);
            //pic18f26k83_can_send(&msg);
        }

#if 1
        // Transmit
        if (state != STATE_TX) {
            // xkcd.com/221 of PRBS11
            static const uint8_t test_sequence[] = { 0xFF, 0xE0, 0x0C, 0x07, 0x83, 0x31, 0xFE, 0xC0, 0xB8, 0x4B, 0x2C, 0xF3, 0xE7, 0x8F, 0x36, 0x7D, 0xF1, 0x46, 0x8B, 0x94, 0xB8, 0xCB, 0x7C, 0xD1, 0xF2, 0xC7, 0x3B, 0x7A, 0xD2, 0x33, 0x5F, 0xC4, 0x1A, 0x8E, 0x16, 0xC9, 0xBD, 0xE9, 0x49, 0x8D, 0xF7, 0x45, 0x4A, 0x0C, 0x47, 0xAB, 0x20, 0xF4, 0x64, 0xBE, 0xC8, 0xBD, 0x49, 0x0D, 0xA7, 0x67, 0x5F, 0x44, 0x4A, 0xAC, 0x03, 0x81, 0xB0, 0xEE, 0x6A, 0xF8, 0x23, 0x15, 0xE8, 0x49, 0x2D, 0xB3, 0x6F, 0xDA, 0x16, 0x49, 0xED, 0xCB, 0x5C, 0xC5, 0xFA, 0x42, 0x69, 0x79, 0x93, 0xFB, 0x82, 0xB1, 0x0E, 0xA6, 0x87, 0x93, 0x3B, 0xFA, 0x82, 0x11, 0x4A, 0x8C, 0x17, 0x89, 0x35, 0xBC, 0x69, 0xB9, 0xEB, 0xC8, 0x9D, 0x5D, 0x05, 0x22, 0x35, 0x5C, 0x05, 0x82, 0x71, 0x76, 0x95, 0x98, 0x7F, 0x30, 0x7E, 0x30, 0xDE, 0x74, 0xF4, 0xE4, 0xEE, 0xEA, 0xA8, 0x01, 0x00, 0xA0, 0x44, 0x2A, 0x90, 0x1A, 0x0E, 0x46, 0xEB, 0xA8, 0xA1, 0x44, 0x8A, 0xD4, 0x30, 0x9E, 0x5C, 0xE5, 0xEE, 0x4A, 0xEC, 0x2B, 0x90, 0xBA, 0x4A, 0x6C, 0x7B, 0xB2, 0xAF, 0x02, 0x61, 0x7C, 0x91, 0xDA, 0xD6, 0x31, 0xDE, 0xD4, 0xB0, 0xCE, 0x7E, 0xF0, 0xA6, 0x47, 0xEB, 0x08, 0xE5, 0x6E, 0x1A, 0xCE, 0x3E, 0xD8, 0xB7, 0x4D, 0x4F, 0x0E, 0x66, 0xFF, 0xA0, 0x24, 0x16, 0x89, 0x95, 0xF8, 0x43, 0x29, 0xF1, 0xC6, 0xDB, 0xB6, 0xAD, 0x83, 0x71, 0xD6, 0xD1, 0xB2, 0xEF, 0x2A, 0x70, 0x76, 0x35, 0xDC, 0x55, 0xA0, 0x64, 0x3E, 0x98, 0x9F, 0x5C, 0x45, 0xAA, 0x60, 0x7C, 0x31, 0x9E, 0xFC, 0xA1, 0xC4, 0xDA, 0xF6, 0x25, 0xD6, 0x51, 0xE2, 0xCD, 0x3F, 0x38, 0x7B, 0x32, 0xFF, 0x20, 0x74, 0x34, 0x9C, 0xDD, 0xF5, 0x44, 0x0A, 0x84, 0x12, 0x8B, 0x14, 0xE8, 0xE9, 0x69, 0x99, 0xFF, 0xE0, 0x0C, 0x07, 0x83, 0x31, 0xFE, 0xC0, 0xB8, 0x4B, 0x2C, 0xF3, 0xE7, 0x8F, 0x36, 0x7D, 0xF1, 0x46, 0x8B, 0x94, 0xB8, 0xCB, 0x7C, 0xD1, 0xF2, 0xC7, 0x3B, 0x7A, 0xD2, 0x33, 0x5F, 0xC4, 0x1A, 0x8E, 0x16, 0xC9, 0xBD, 0xE9, 0x49, 0x8D, 0xF7, 0x45, 0x4A, 0x0C, 0x47, 0xAB, 0x20, 0xF4, 0x64, 0xBE, 0xC8, 0xBD, 0x49, 0x0D, 0xA7, 0x67, 0x5F, 0x44, 0x4A, 0xAC, 0x03, 0x81, 0xB0, 0xEE, };
            static uint8_t test_count = 0;
            uint8_t data[TEST_SIZE] = {test_count};
            memcpy(data+1, test_sequence+test_count, sizeof(data)-1);
            CC1200_Transmit(data, sizeof(data));
            test_count++;
        }
#endif

        // Status report
        if (millis() - last_millis > 100) {
            build_analog_data_msg(priority, millis(), SENSOR_FPS, bit_count*10, &msg);
            pic18f26k83_can_send(&msg);
            bit_count = 0;

            if (total_count > 0) {
                build_analog_data_msg(priority, millis(), SENSOR_CANARD_ENCODER_1, 1000L * error_count / total_count, &msg);
                pic18f26k83_can_send(&msg);
            }

            last_millis = millis();
        }

        // led things
        if (rx_len) {
            toggle_LED_Green(1);
        } else {
            toggle_LED_Green(0);
        }
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
