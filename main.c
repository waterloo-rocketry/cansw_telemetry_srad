/*
 * File:   main.c
 * Author: Manav
 *
 * Created on February 22, 2025, 11:40 AM
 */

#include <xc.h>
#include "leds.h"
#include "clockInit.h"
#include "adc.h"

#include "canlib/canlib.h"
#include "canlib/message_types.h"

#pragma config WDTE = OFF // Watchdog Timer disabled

#define _XTAL_FREQ 12000000  // 12 MHz 

static void can_msg_handler(const can_msg_t *msg);
static void send_status_ok(void);
// memory pool for the CAN tx buffer
uint8_t tx_pool[200];

static void can_msg_handler(const can_msg_t *msg) {
    uint16_t msg_type = get_message_type(msg);

    // ignore messages that were sent from this board
    if (get_board_unique_id(msg) == BOARD_TYPE_UNIQUE_ID) {
        return;
    }
    
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
    //SPI_Init();
    CAN_Init();
}

void SendCurrentReading() {
    uint16_t val;
    val = read_ADC();
    val = val / 0.025; // Shunt resistor is 25mR
    
    uint16_t time = 0; // CHANGE LATER
    can_msg_prio_t valPrio = PRIO_LOW;
    can_analog_sensor_id_t msgid = SENSOR_12V_CURR;
    can_msg_t currMsg;
    
    build_analog_data_msg(valPrio, time, msgid, val, &currMsg);
    can_send(&currMsg)
}

void main() {
    Board_Init();

    while (1) {
        CLRWDT();
        
        toggle_LED_Green(1);
        toggle_LED_Blue(1);
        toggle_LED_Red(1);

        __delay_ms(1000);

        toggle_LED_Green(0);
        toggle_LED_Blue(0);
        toggle_LED_Red(0);

        __delay_ms(1000);
        
        //SendCurrentReading();
        
        uint8_t ccRead;
        ccRead = Read_CC1200(0x3D);
        uint16_t time = 0;
        can_msg_prio_t prio = PRIO_HIGH;
        can_msg_t debugMsg;
        
        build_debug_raw_msg(prio, time, ccRead, &debugMsg);
        can_send(&debugMsg);
        
    }
}
