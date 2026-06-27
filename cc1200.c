/*
 * File:   cc1200.c
 * Author: Manav
 *
 * Created on February 26, 2025, 10:34 AM
 *
 * Note a lot of register values are set using the TI SmartRF Studio application:
 * https://www.ti.com/tool/SMARTRFTM-STUDIO and are subject to change as the board
 * is tested and characterized
 */


#include "cc1200.h"
#include "canlib/can.h"
#include "canlib/message/msg_telemetry.h"
#include "config.h"

#include <string.h>

#define MAX_PACKET_LEN 12

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
    {CC1200_IOCFG3,         0x57}, // GPIO3 to LNA_PA_REG_PD, inverted
    {CC1200_IOCFG2,         0x07}, // GPIO2 to CRC_OK
    {CC1200_IOCFG0,         0x73}, // GPIO0 to 1 TODO antenna diversity
    {CC1200_FREQOFF1,       0x02}, // Frequency Offset MSB
    {CC1200_FREQOFF0,       0xB6}, // Frequency Offset LSB
    {CC1200_RFEND_CFG1,     0x0E}, // RXOFF_MODE = IDLE, RX_TIME = 0
    {CC1200_RFEND_CFG0,     0x00}, // TXOFF_MODE = IDLE, TERM_ON_BAD_PACKET_EN = 1 TODO antenna diversity
    {CC1200_WOR_EVENT0_MSB, 0x01}, // EVENT0 = 256
    {CC1200_WOR_EVENT0_LSB, 0x00}, // EVENT0 / 2^(RX_TIME+3) * 1250 / 40MHz = 1ms
    {CC1200_FIFO_CFG,       0x80}, // CRC_AUTOFLUSH = 1

    // automatic configs
    {CC1200_SYNC_CFG1,      0xA8},
    {CC1200_SYNC_CFG0,      0x13},
    {CC1200_DEVIATION_M,    0x99},
    {CC1200_MODCFG_DEV_E,   0x85},
    {CC1200_DCFILT_CFG,     0x26},
    {CC1200_PREAMBLE_CFG0,  0x8A},
    {CC1200_IQIC,           0x00},
    {CC1200_CHAN_BW,        0x02},
    {CC1200_MDMCFG1,        0xC2},
    {CC1200_MDMCFG0,        0x05},
    {CC1200_SYMBOL_RATE2,   0xC9},
    {CC1200_SYMBOL_RATE1,   0x99},
    {CC1200_SYMBOL_RATE0,   0x99},
    {CC1200_AGC_REF,        0x2F},
    {CC1200_AGC_CS_THR,     0x01},
    {CC1200_AGC_CFG1,       0x16},
    {CC1200_AGC_CFG0,       0x84},
    {CC1200_FS_CFG,         0x12},
    {CC1200_PKT_CFG2,       0x00},
    {CC1200_PKT_CFG1,       0x43},
    {CC1200_PKT_CFG0,       0x20},
    {CC1200_PA_CFG1,        0x5F},
    {CC1200_PKT_LEN,        0xFF},
    {CC1200_IF_MIX_CFG,     0x18},
    {CC1200_FREQOFF_CFG,    0x30},
    {CC1200_TOC_CFG,        0xC0},
    {CC1200_MDMCFG2,        0x00},
    {CC1200_FREQ2,          0x5B},
    {CC1200_FREQ1,          0x80},
    {CC1200_IF_ADC1,        0xEE},
    {CC1200_IF_ADC0,        0x10},
    {CC1200_FS_DIG1,        0x04},
    {CC1200_FS_DIG0,        0x55},
    {CC1200_FS_CAL1,        0x40},
    {CC1200_FS_CAL0,        0x0E},
    {CC1200_FS_DIVTWO,      0x03},
    {CC1200_FS_DSM0,        0x33},
    {CC1200_FS_DVC0,        0x17},
    {CC1200_FS_PFD,         0x00},
    {CC1200_FS_PRE,         0x6E},
    {CC1200_FS_REG_DIV_CML, 0x1C},
    {CC1200_FS_SPARE,       0xAC},
    {CC1200_FS_VCO0,        0xB5},
    {CC1200_IFAMP,          0x0D},
    {CC1200_XOSC5,          0x0E},
    {CC1200_XOSC1,          0x03},
};

CC1200ReadResult Read_CC1200(uint16_t reg) {
    CC1200ReadResult result;

    SPI_Select();

    // If accessing extended registers
    if (reg >= 0x2F00) {
        // extended register read command
        SPI_Transfer(0x80 | CC1200_EXTENDED_REGISTER);
        result.status = SPI_Transfer(reg & 0xFF);
    } else {
        result.status = SPI_Transfer((reg & 0xFF) | CC1200_READ);
    }

    result.value = SPI_Transfer(0x00);

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
    }

    SPI_Transfer(reg & 0xFF);
    status = SPI_Transfer(val);

    SPI_Deselect();
    return status;
};

uint8_t Command_CC1200(uint8_t command) {
    SPI_Select();
    uint8_t status = SPI_Transfer(command);
    SPI_Deselect();
    return status;
};

void CC1200_Init(void) {
    // configure RESET_n pin
    TRISC7 = 0;
    LATC7 = 0;
    __delay_ms(100);
    LATC7 = 1;

    // configure CC1200 GPIO2
    TRISB3 = 1;
    ANSELB3 = 0;

    // configure CC1200 Registers
    size_t numSettings = sizeof (preferredSettings) / sizeof (preferredSettings[0]);
    for (size_t i = 0; i < numSettings; i++) {
        Write_CC1200(preferredSettings[i].addr, preferredSettings[i].value);
    }
}

uint8_t CC1200_Transmit_Packet(can_msg_t *msg) {
    SPI_Select();
    SPI_Transfer(CC1200_FIFO | CC1200_BURST);

    // length (SID + Data)
    SPI_Transfer(msg->data_len + 4);

    // SID
    for (int i = 3; i >= 0; i--) {
        SPI_Transfer((msg->sid >> i*8) & 0xFF);
    }

    // data
    for (int i = 0; i < msg->data_len; i++) {
        SPI_Transfer(msg->data[i]);
    }

    SPI_Deselect();

    return Command_CC1200(COMMAND_STX);
}

/* 
 * CC1200 FIFO packet format:
 * 1 byte length
 * 4 bytes sid
 * 2-8 bytes data
 * 1 byte RSSI
 * 1bit CRC and 7 bits LQI
 */
uint8_t CC1200_Receive_Packet(can_msg_t *msg) {
    if(!RB3) return 0; // CRC_OK is not asserted from CC1200 GPIO2

    SPI_Select();
    SPI_Transfer(CC1200_FIFO_CFG | CC1200_READ | CC1200_BURST);

    uint8_t len = SPI_Transfer(0);

    if(len == 1) {
        // decode end of transmit frame into can message
        uint8_t channel_id = SPI_Transfer(0);
        build_telemetry_state_switch_msg(PRIO_LOW, 0, channel_id, msg);
        goto CC1200_Receive_Packet_end;
    }

    if(len <= 4 || len > MAX_PACKET_LEN) {
        goto CC1200_Receive_Packet_end;
    }

    msg.data_len = len - 4;

    for(int i = 0; i < 4; i++) {
        msg.sid = (msg.sid << 8) | SPI_Transfer(0);
    }

    for(int i = 0; i < len-4; i++) {
        msg.data[i] = SPI_Transfer(0);
    }

    // TODO do something with these
    uint8_t rssi = SPI_Transfer(0);
    uint8_t crc_lqi = SPI_Transfer(0);

CC1200_Receive_Packet_end:
    SPI_Deselect();
    return Command_CC1200(COMMAND_SFRX);
}

// transmit single byte indicating end of transmissio period + id of the board
// that should transmit next
uint8_t CC1200_Transmit_End(uint8_t next_channel) {
    SPI_Select();
    SPI_Transfer(CC1200_FIFO | CC1200_BURST);
    SPI_Transfer(1);
    SPI_Transfer(next_channel);
    SPI_Deselect();
    return Command_CC1200(COMMAND_STX);
}
