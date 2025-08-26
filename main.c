/*
 * File:   main.c
 * Author: Manav
 *
 * Created on February 22, 2025, 11:40 AM
 */

#include "config.h"
#include <xc.h>

#include "adc.h" // interface with ADC
#include "cc1200.h" // interface with CC1200
#include "clockInit.h" // initialize XTAL
#include "leds.h" // interface with LEDs

// rocketlib and canlib
#include "canlib.h" // interface with RocketCAN
#include "timer.h" // import custom millis() function

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
    can_send(&status_msg);
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
    can_send(&current_reading_msg);

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

    while (1) {
        CLRWDT();

        // send_board_status();
        // send_current_reading();

        // This code is to test SPI

        can_msg_prio_t priority = PRIO_HIGH;
        can_analog_sensor_id_t msgid = SENSOR_12V_CURR;
        can_msg_t msg;

        uint8_t status[6] = {0};

        CC1200ReadResult result = Read_CC1200(CC1200_PARTNUMBER);
        status[0] = result.status;
        if (result.value == 0x20) {
            toggle_LED_Red(1);
        } else {
            toggle_LED_Red(0);
        }

        result = Read_CC1200(CC1200_MODEM_STATUS0);
        status[1] = result.value;

        result = Read_CC1200(CC1200_MODEM_STATUS1);
        status[2] = result.value;

        status[3] = CC1200_get_RX_FIFO_len();

        if (status[0] >> 4 == STATE_RX) {
            toggle_LED_Green(1);
        } else {
            toggle_LED_Green(0);
        }

        if (millis() - last_millis > 100) {
            Command_CC1200(COMMAND_SFRX);
            last_millis = millis();
            build_debug_raw_msg(priority, millis(), status, &msg);
            can_send(&msg);

            static int i = 0;
            if(i++ % 10 == 0) {
                uint8_t data[129] = "According to all known laws of aviation, there is no way a bee should be able to fly. Its wings are too small to get its fat little bo";
                //CC1200_Transmit(data, 128);
            }
        }
    }
}

static void __interrupt() interrupt_handler(void) {
    if (PIR5) {
        can_handle_interrupt();
    }

    // Timer0 has overflowed - update millis() function
    // This happens approximately every 500us
    if (PIE3bits.TMR0IE == 1 && PIR3bits.TMR0IF == 1) {
        timer0_handle_interrupt();
        PIR3bits.TMR0IF = 0;
    }
}
