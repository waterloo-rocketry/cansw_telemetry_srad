/*
 * File:   cc1200.c
 * Author: Manav
 *
 * Created on February 26, 2025, 10:34 AM
 *
 * This code is very crude, a lot of functions provide write hard coded values
 * to registers that cannot be changed without modifying this file, in future
 * versions I would like to change that, but for now, I just want to write the
 * minimum viable code to have the board reliably function
 *
 * Note a lot of register values are set using the TI SmartRF Studio application:
 * https://www.ti.com/tool/SMARTRFTM-STUDIO and are subject to change as the board
 * is tested and characterized
 */


#include "cc1200.h"
#include <string.h>
#include "canlib/can.h"
#include "canlib/pic18f26k83/pic18f26k83_can.h"
#include "canlib/message/msg_telemetry.h"
#include "config.h"

#define MAX_PACKET_LEN 14


// Call sign MUST be transmitted at start of every message
const uint64_t CALLSIGN = 0x564133555750; // ASCII "VAEUWP"/Manav

/* RX flow:
 *
 * In RX state, CC1200 transitions to IDLE when a packet is received
 * (configured by RFEND_CFGx). CC1200_State_Transition would attempt to read a
 * packet from fifo when CC1200 is in idle, flush the FIFO, then go back to RX
 * state.
 *
 * Packet is prepended with length, appeneded with RSSI, CRC, and LQI. See user
 * guide section 8.7.3
 */

// Register assignments, use MARTRFTM-STUDIO to configure and copy and paste in
// "TrxEB RF Settings Value Line" format
// https://www.ti.com/tool/SMARTRFTM-STUDIO
// frequency and power has helper function for runtime configuration
static const registerSetting_t preferredSettings[] = {
    // manual configs
    {CC1200_IOCFG3, 0x57}, // GPIO3 IO Pin Configuration
    {CC1200_IOCFG0, 0x73}, // GPIO0 IO Pin Configuration
    {CC1200_FREQOFF1, 0x02}, // Frequency Offset MSB
    {CC1200_FREQOFF0, 0xB6}, // Frequency Offset LSB
    {CC1200_RFEND_CFG1, 0x3F}, // FEND Configuration Reg. 1 //keep in RX if good packet recieved.RX timeout change using 3:1
    {CC1200_RFEND_CFG0, 0x38}, // FEND Configuration Reg. 0

    // automatic configs
    {CC1200_SYNC_CFG1, 0xA8},
    {CC1200_SYNC_CFG0, 0x13},
    {CC1200_DEVIATION_M, 0x99},
    {CC1200_MODCFG_DEV_E, 0x85},
    {CC1200_DCFILT_CFG, 0x26},
    {CC1200_PREAMBLE_CFG0, 0x8A},
    {CC1200_IQIC, 0x00},
    {CC1200_CHAN_BW, 0x02},
    {CC1200_MDMCFG1, 0xC2},
    {CC1200_MDMCFG0, 0x05},
    {CC1200_SYMBOL_RATE2, 0xC9},
    {CC1200_SYMBOL_RATE1, 0x99},
    {CC1200_SYMBOL_RATE0, 0x99},
    {CC1200_AGC_REF, 0x2F},
    {CC1200_AGC_CS_THR, 0x01},
    {CC1200_AGC_CFG1, 0x16},
    {CC1200_AGC_CFG0, 0x84},
    {CC1200_FIFO_CFG, 0x00},
    {CC1200_FS_CFG, 0x12},
    {CC1200_PKT_CFG2, 0x00},
    {CC1200_PKT_CFG1, 0x43},
    {CC1200_PKT_CFG0, 0x20},
    {CC1200_PA_CFG1, 0x5F},
    {CC1200_PKT_LEN, 0xFF},
    {CC1200_IF_MIX_CFG, 0x18},
    {CC1200_FREQOFF_CFG, 0x30},
    {CC1200_TOC_CFG, 0xC0},
    {CC1200_MDMCFG2, 0x00},
    {CC1200_FREQ2, 0x5B},
    {CC1200_FREQ1, 0x80},
    {CC1200_IF_ADC1, 0xEE},
    {CC1200_IF_ADC0, 0x10},
    {CC1200_FS_DIG1, 0x04},
    {CC1200_FS_DIG0, 0x55},
    {CC1200_FS_CAL1, 0x40},
    {CC1200_FS_CAL0, 0x0E},
    {CC1200_FS_DIVTWO, 0x03},
    {CC1200_FS_DSM0, 0x33},
    {CC1200_FS_DVC0, 0x17},
    {CC1200_FS_PFD, 0x00},
    {CC1200_FS_PRE, 0x6E},
    {CC1200_FS_REG_DIV_CML, 0x1C},
    {CC1200_FS_SPARE, 0xAC},
    {CC1200_FS_VCO0, 0xB5},
    {CC1200_IFAMP, 0x0D},
    {CC1200_XOSC5, 0x0E},
    {CC1200_XOSC1, 0x03},
};

// read one packet from fifo
static uint8_t packet[MAX_PACKET_LEN] = {0};
static uint8_t packet_len = 0;

//static void Receive_Packet(void) {
//    uint8_t len = Read_CC1200(CC1200_NUM_RXBYTES).value;
//    if (!len) {
//        return;
//    }
//
//    SPI_Select();
//    SPI_Transfer(CC1200_FIFO | CC1200_READ | CC1200_BURST);
//    for (int i = 0; i < len; i++) {
//        // read the whole packet even if buffer isn't big enough
//        uint8_t data = SPI_Transfer(0);
//        if (i < MAX_PACKET_LEN) {
//            packet[i] = data;
//        }
//    }
//    SPI_Deselect();
//    Command_CC1200(COMMAND_SFRX);
//
//    packet_len = len;
//}

CC1200ReadResult Read_CC1200(uint16_t reg) {
    CC1200ReadResult result;

    SPI_Select();

    // If accessing extended registers
    if (reg >= 0x2F00) {
        // extended register read command
        SPI_Transfer(0x80 | CC1200_EXTENDED_REGISTER);
        result.status = SPI_Transfer(reg & 0xFF);
        result.value = SPI_Transfer(0x00);
    } else {
        result.status = SPI_Transfer(0x80 | (reg & 0xFF)); // R/W=1 | 0 | register address
        result.value = SPI_Transfer(0x00);
    }

    SPI_Deselect();
    return result;
};

uint8_t Write_CC1200(uint16_t reg, uint8_t val) {
    uint8_t status;

    SPI_Select();

    // If accessing extended registers
    if (reg >= 0x2F00) {
        // extended register write command
        SPI_Transfer(CC1200_EXTENDED_REGISTER);
        SPI_Transfer(reg & 0xFF);
        status = SPI_Transfer(val);
    } else {
        SPI_Transfer(reg & 0xFF);
        status = SPI_Transfer(val);
    }

    SPI_Deselect();
    return status;
};

uint8_t Command_CC1200(uint8_t command) {
    SPI_Select();
    uint8_t status = SPI_Transfer(command);
    SPI_Deselect();
    return status;
};

// Configure CC1200 Registers

void CC1200_Init(void) {
    // configure RESET_n pin
    TRISC7 = 0;
    LATC7 = 0;
    __delay_ms(100);
    LATC7 = 1;

    // variables
    packet_len = 0;

    // registers
    size_t numSettings = sizeof (preferredSettings) / sizeof (preferredSettings[0]);
    for (size_t i = 0; i < numSettings; i++) {
        Write_CC1200(preferredSettings[i].addr, preferredSettings[i].value);
    }
}

void CC1200_Transmit(uint8_t *data, uint8_t len) {
    SPI_Select();
    uint8_t status = SPI_Transfer(CC1200_FIFO | CC1200_BURST); // 3.2.4 FIFO access with burst
    SPI_Transfer(len);
    for (int i = 0; i < len; i++) {
        SPI_Transfer(data[i]);
    }
    SPI_Deselect();
    Command_CC1200(COMMAND_STX);
}

cc1200_transmit_status CC1200_Load_TX_FIFO(const can_msg_t *msg) {
    if (msg->data_len + 6 <= CC1200_TX_Buffer_Bytes()) { //1 for start byte, 4 bytes for sid, 1 for data len 
        uint8_t buffer[MAX_PACKET_LEN]; //max size of can message + start byte
        buffer[0] = MSG_START_BYTE;
        for (int i = 0; i < 4; i++) {
            buffer[i + 1] = (msg->sid >> (3 - i) << 3) & 0xFF;
        }

        buffer[5] = msg->data_len;

        // Data
        for (int i = 0; i < msg->data_len; i++) {
            buffer[6 + i] = msg->data[i];
        }
        CC1200_Transmit(buffer, msg->data_len + 6);
        return MSG_LOADED;
    }
    return MSG_NOT_LOADED;
}

uint8_t CC1200_Receive(uint8_t *data, uint8_t len) {
    if (len > packet_len) {
        len = packet_len;
    }
    memcpy(data, packet, len);
    packet_len = 0;
    return len;
}

void CC1200_Resync(void) {
    uint8_t byte;
    uint8_t bytes_checked = 0;

    while ((Read_CC1200(CC1200_NUM_RXBYTES).value > 0) &&
            (bytes_checked < RESYNC_MAX_BYTES)) {
        SPI_Select();
        SPI_Transfer(CC1200_FIFO | CC1200_READ);
        byte = SPI_Transfer(0);
        SPI_Deselect();

        bytes_checked++;

        if (byte == MSG_START_BYTE) {
            uint8_t rx_first = Read_CC1200(CC1200_RXFIRST).value;
            Write_CC1200(CC1200_RXFIRST, (uint8_t) (rx_first - 1));
            return;
        }
    }
}

cc1200_receive_status CC1200_Receive_RX_FIFO() {
    uint8_t len = Read_CC1200(CC1200_NUM_RXBYTES).value;
    //no new message has arrived and all previous msgs have been removed from buffer
    if (len == 0) {
        return BUFFER_EMPTY;
    }
    if (len < 8) { //5 bytes is length of sid + len + 1 start byte + 2bytes of data is smallest possible can msg
        return MSG_PARTIAL_RCV;
    }

    SPI_Select();
    SPI_Transfer(CC1200_FIFO | CC1200_READ | CC1200_BURST);
    packet[0] = SPI_Transfer(0); //transfer out start byte
    //transfer sid and length bytes

    if (packet[0] != MSG_START_BYTE) {
        CC1200_Resync();
        return MSG_CORRUPTED;
    }
    for (int i = 0; i < 5; i++) {
        packet[i + 1] = SPI_Transfer(0);
    }
    SPI_Deselect();
    if (packet[5] > 8) {
        CC1200_Resync();
        return MSG_CORRUPTED;
    }
    //extract length of current can message in buffer
    len = Read_CC1200(CC1200_NUM_RXBYTES).value;
    //if the whole message has not arrived yet
    if (len < packet[5]) { //buffer length is less than datalen of msg
        uint8_t rx_first = Read_CC1200(CC1200_RXFIRST).value;
        Write_CC1200(CC1200_RXFIRST, (uint8_t) (rx_first - 6)); //move pointer back by 6 bytes in queue (sid + data)
        return MSG_PARTIAL_RCV;
    } else { //whole message has been received, extract it from buffer
        uint8_t msg_data[8]; //match definition in can_msg_t
        SPI_Select();
        SPI_Transfer(CC1200_FIFO | CC1200_READ | CC1200_BURST);
        //transfer over can msg data
        for (int i = 0; i < packet[5]; i++) {
            msg_data[i] = SPI_Transfer(0);
        }
        SPI_Deselect();
        can_msg_t msg;
        msg.sid = ((uint32_t) packet[1] << 24) + ((uint32_t) packet[2] << 16) + ((uint32_t) packet[3] << 8) + (uint32_t) packet[4];
        msg.data_len = packet[5];
        memcpy(msg.data, msg_data, msg.data_len);
        uint8_t channel_id;
        w_status_t state = get_telemetry_state_switch_msg(&msg, &channel_id);
        if (state == W_SUCCESS) {
            if (channel_id == BOARD_INST_UNIQUE_ID) // checks if the state received matches the board current id
            {
                return MSG_STATE_SW; //received state switch
            }
        }
        pic18f26k83_can_send(&msg);
        return MSG_RCV;
    }

}

uint8_t CC1200_TX_Buffer_Bytes() {
    CC1200ReadResult tx_bytes = Read_CC1200(CC1200_NUM_TXBYTES);
    return BUFFER_SIZE - tx_bytes.value;
}

uint8_t CC1200_RX_Buffer_Bytes() {
    CC1200ReadResult rx_bytes = Read_CC1200(CC1200_NUM_RXBYTES);
    return BUFFER_SIZE - rx_bytes.value;
}

uint8_t CC1200_State_Transition(void) {
    uint8_t state = (Command_CC1200(COMMAND_SNOP) >> 4) & 0x7;
    switch (state) {
        case STATE_IDLE:
            //Receive_Packet();
            Command_CC1200(COMMAND_SRX);
            break;
        case STATE_RX_FIFO_ERROR:
            Command_CC1200(COMMAND_SFRX);
            break;
        case STATE_TX_FIFO_ERROR:
            Command_CC1200(COMMAND_SFTX);
            break;
    }
    return state;
}

