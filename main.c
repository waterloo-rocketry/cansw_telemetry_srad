/*
 * File:   main.c
 * Author: Manav
 *
 * Created on February 22, 2025, 11:40 AM
 */

#include "config.h"
#include <xc.h>
#include <string.h>

#include "ltt_can.h"
#include "ltt_rf.h"
#include "adc.h" // interface with ADC
#include "cc1200.h" // interface with CC1200
#include "osc.h" // initialize XTAL
#include "leds.h" // interface with LEDs

// rocketlib and canlib
#include "canlib.h" // interface with RocketCAN
#include "timer.h" // import custom millis() function

#define TEST_SIZE 32

uint8_t board_status = 0; // board status flag
enum Transciever_State tstate = RX; // this also needs to vary based on rocket or ground

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 75; j++); // tune this inner value
}

void Board_Init() {
    timer0_init();
    LEDs_Init();
    Osc_Init();
    ADC_Init();
    SPI_Init();
    CAN_Init();
    RF_Init();
}

void main() {
    Board_Init();

    toggle_LED_Green(0);
    toggle_LED_Blue(0);
    toggle_LED_Red(1);

    uint32_t last_millis = millis();
    uint32_t time_to = millis(); //used to track
    uint32_t time_rx = millis();

    uint8_t state = CC1200_State_Transition();

    while (1) {
        CLRWDT();

        send_board_status(board_status);
        send_current_reading(&board_status);
        delay_ms(1000);

#if BOARD_MODE == BOARD_MODE_ROCKET
        if (tstate == TX) {
            //transmit messages into buffer if available
            
            //CC1200_Transmit();
            
            if (time_to >= 200) {
                tstate = RX;
                time_to = millis();
                time_rx = millis();
                //send ending frame to queue other side
            }
        } else if (tstate == RX) {
            uint8_t data[64] = {0};
            uint8_t length=CC1200_Receive(data, sizeof(data));
            // receive messages

            if (length>0)
            {
                time_rx=millis();
            }
            if (millis() - time_rx >= RECIEVE_TIME || millis() - time_to >= TRANSMIT_TIME) {
                tstate = TX;
                time_to = millis();
            }
        }

        toggle_LED_Green(0);

#elif BOARD_MODE == BOARD_MODE_GROUND
        if (tstate == TX) {
            //load messages into CC1200 if they are available
            //CC1200_Transmit();

            if (CC1200_TX_Buffer_Space() == 0 || millis() - time_to >= TRANSMIT_TIME) {
                tstate = RX;
            }
        } else if (tstate == RX) {
            uint8_t data[64] = {0};
            uint8_t length=CC1200_Receive(data, sizeof(data));
            if(data[length-1]==END_FRAME)
            {
                tstate=TX;
                time_to=millis();
            }

        }

#elif BOARD_MODE == BOARD_TEST_TX
        // Transmit
        if (state != STATE_TX) {
            // xkcd.com/221 of PRBS11
            static const uint8_t test_sequence[] = {0xFF, 0xE0, 0x0C, 0x07, 0x83, 0x31, 0xFE, 0xC0, 0xB8, 0x4B, 0x2C, 0xF3, 0xE7, 0x8F, 0x36, 0x7D, 0xF1, 0x46, 0x8B, 0x94, 0xB8, 0xCB, 0x7C, 0xD1, 0xF2, 0xC7, 0x3B, 0x7A, 0xD2, 0x33, 0x5F, 0xC4, 0x1A, 0x8E, 0x16, 0xC9, 0xBD, 0xE9, 0x49, 0x8D, 0xF7, 0x45, 0x4A, 0x0C, 0x47, 0xAB, 0x20, 0xF4, 0x64, 0xBE, 0xC8, 0xBD, 0x49, 0x0D, 0xA7, 0x67, 0x5F, 0x44, 0x4A, 0xAC, 0x03, 0x81, 0xB0, 0xEE, 0x6A, 0xF8, 0x23, 0x15, 0xE8, 0x49, 0x2D, 0xB3, 0x6F, 0xDA, 0x16, 0x49, 0xED, 0xCB, 0x5C, 0xC5, 0xFA, 0x42, 0x69, 0x79, 0x93, 0xFB, 0x82, 0xB1, 0x0E, 0xA6, 0x87, 0x93, 0x3B, 0xFA, 0x82, 0x11, 0x4A, 0x8C, 0x17, 0x89, 0x35, 0xBC, 0x69, 0xB9, 0xEB, 0xC8, 0x9D, 0x5D, 0x05, 0x22, 0x35, 0x5C, 0x05, 0x82, 0x71, 0x76, 0x95, 0x98, 0x7F, 0x30, 0x7E, 0x30, 0xDE, 0x74, 0xF4, 0xE4, 0xEE, 0xEA, 0xA8, 0x01, 0x00, 0xA0, 0x44, 0x2A, 0x90, 0x1A, 0x0E, 0x46, 0xEB, 0xA8, 0xA1, 0x44, 0x8A, 0xD4, 0x30, 0x9E, 0x5C, 0xE5, 0xEE, 0x4A, 0xEC, 0x2B, 0x90, 0xBA, 0x4A, 0x6C, 0x7B, 0xB2, 0xAF, 0x02, 0x61, 0x7C, 0x91, 0xDA, 0xD6, 0x31, 0xDE, 0xD4, 0xB0, 0xCE, 0x7E, 0xF0, 0xA6, 0x47, 0xEB, 0x08, 0xE5, 0x6E, 0x1A, 0xCE, 0x3E, 0xD8, 0xB7, 0x4D, 0x4F, 0x0E, 0x66, 0xFF, 0xA0, 0x24, 0x16, 0x89, 0x95, 0xF8, 0x43, 0x29, 0xF1, 0xC6, 0xDB, 0xB6, 0xAD, 0x83, 0x71, 0xD6, 0xD1, 0xB2, 0xEF, 0x2A, 0x70, 0x76, 0x35, 0xDC, 0x55, 0xA0, 0x64, 0x3E, 0x98, 0x9F, 0x5C, 0x45, 0xAA, 0x60, 0x7C, 0x31, 0x9E, 0xFC, 0xA1, 0xC4, 0xDA, 0xF6, 0x25, 0xD6, 0x51, 0xE2, 0xCD, 0x3F, 0x38, 0x7B, 0x32, 0xFF, 0x20, 0x74, 0x34, 0x9C, 0xDD, 0xF5, 0x44, 0x0A, 0x84, 0x12, 0x8B, 0x14, 0xE8, 0xE9, 0x69, 0x99, 0xFF, 0xE0, 0x0C, 0x07, 0x83, 0x31, 0xFE, 0xC0, 0xB8, 0x4B, 0x2C, 0xF3, 0xE7, 0x8F, 0x36, 0x7D, 0xF1, 0x46, 0x8B, 0x94, 0xB8, 0xCB, 0x7C, 0xD1, 0xF2, 0xC7, 0x3B, 0x7A, 0xD2, 0x33, 0x5F, 0xC4, 0x1A, 0x8E, 0x16, 0xC9, 0xBD, 0xE9, 0x49, 0x8D, 0xF7, 0x45, 0x4A, 0x0C, 0x47, 0xAB, 0x20, 0xF4, 0x64, 0xBE, 0xC8, 0xBD, 0x49, 0x0D, 0xA7, 0x67, 0x5F, 0x44, 0x4A, 0xAC, 0x03, 0x81, 0xB0, 0xEE,};
            static uint8_t test_count = 0;
            uint8_t data[TEST_SIZE] = {test_count};
            memcpy(data + 1, test_sequence + test_count, sizeof (data) - 1);
            CC1200_Transmit(data, sizeof (data));
            test_count++;
        }
#elif BOARD_MODE == BOARD_TEST_RX
        // Receive
        uint8_t data[64] = {0};
        uint8_t rx_len = CC1200_Receive(data, sizeof (data));
        static uint8_t test_count = 0;
        static uint16_t error_count = 0;
        static uint16_t drop_count = 0;
        static uint16_t total_count = 0;
        static uint16_t bit_count;
        static uint8_t rssi = 0;
        static uint8_t lqi = 0;

        if (rx_len) {
            // counter increments by one and crc check passes
            if (!(data[rx_len - 1] & 0x80)) {
                error_count++;
            }

            drop_count = data[1] - test_count;
            test_count = data[1];

            total_count++;
            bit_count += (rx_len - 3)*8;

            // overflow
            if (total_count == 0) {
                error_count = 0;
                drop_count = 0;
            }

            // other stats
            rssi = data[rx_len - 2];
            lqi = data[rx_len - 1] & 0x7F;
        }

        // Status report
        static uint8_t count = 0;
        if (millis() - last_millis > 20) {
            if (count % 5 == 0) {
                build_analog_data_msg(priority, millis(), SENSOR_FPS, bit_count * 10, &msg);
                pic18f26k83_can_send(&msg);
                bit_count = 0;
            } else if (count % 5 == 1) {
                build_analog_data_msg(priority, millis(), SENSOR_PT_CHANNEL_1, rssi, &msg);
                pic18f26k83_can_send(&msg);
            } else if (count % 5 == 2) {
                build_analog_data_msg(priority, millis(), SENSOR_PT_CHANNEL_2, lqi, &msg);
                pic18f26k83_can_send(&msg);
            } else if (count % 5 == 3) {
                build_analog_data_msg(priority, millis(), SENSOR_PT_CHANNEL_3, 1000L * drop_count / total_count, &msg);
                pic18f26k83_can_send(&msg);
            } else if (total_count > 0) {
                build_analog_data_msg(priority, millis(), SENSOR_PT_CHANNEL_4, 1000L * error_count / total_count, &msg);
                pic18f26k83_can_send(&msg);
            }

            last_millis = millis();
            count++;
        }

        // led things
        if (rx_len) {
            toggle_LED_Green(1);
        } else {
            toggle_LED_Green(0);
        }
    }
#endif
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
