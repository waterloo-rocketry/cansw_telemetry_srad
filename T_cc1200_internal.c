/*
 * File:   T_cc1200_internal.c
 * Author: Manav
 *
 * Created on April 14, 2026, 4:35 PM
 * 
 * Private functions for communicating with the cc1200 that should not be called
 * from main.c
 */


#include "T_cc1200_internal.h"
#include "T_cc1200.h"

// Register assignments, use MARTRFTM-STUDIO to configure and copy and paste in
// "TrxEB RF Settings Value Line" format
// https://www.ti.com/tool/SMARTRFTM-STUDIO
// frequency and power has helper function for runtime configuration
static const registerSetting_t preferredSettings[] = {
    // manual configs
    {CC1200_IOCFG3,            0x57}, // GPIO3 IO Pin Configuration
    {CC1200_IOCFG0,            0x73}, // GPIO0 IO Pin Configuration
    {CC1200_FREQOFF1,          0x02}, // Frequency Offset MSB
    {CC1200_FREQOFF0,          0xB6}, // Frequency Offset LSB
    {CC1200_RFEND_CFG1,        0x0F}, // FEND Configuration Reg. 1
    {CC1200_RFEND_CFG0,        0x38}, // FEND Configuration Reg. 0

    // automatic configs
    {CC1200_SYNC_CFG1,         0xA8},
    {CC1200_SYNC_CFG0,         0x13},
    {CC1200_DEVIATION_M,       0x99},
    {CC1200_MODCFG_DEV_E,      0x85},
    {CC1200_DCFILT_CFG,        0x26},
    {CC1200_PREAMBLE_CFG0,     0x8A},
    {CC1200_IQIC,              0x00},
    {CC1200_CHAN_BW,           0x02},
    {CC1200_MDMCFG1,           0xC2},
    {CC1200_MDMCFG0,           0x05},
    {CC1200_SYMBOL_RATE2,      0xC9},
    {CC1200_SYMBOL_RATE1,      0x99},
    {CC1200_SYMBOL_RATE0,      0x99},
    {CC1200_AGC_REF,           0x2F},
    {CC1200_AGC_CS_THR,        0x01},
    {CC1200_AGC_CFG1,          0x16},
    {CC1200_AGC_CFG0,          0x84},
    {CC1200_FIFO_CFG,          0x00},
    {CC1200_FS_CFG,            0x12},
    {CC1200_PKT_CFG2,          0x00},
    {CC1200_PKT_CFG1,          0x43},
    {CC1200_PKT_CFG0,          0x20},
    {CC1200_PA_CFG1,           0x5F},
    {CC1200_PKT_LEN,           0xFF},
    {CC1200_IF_MIX_CFG,        0x18},
    {CC1200_FREQOFF_CFG,       0x30},
    {CC1200_TOC_CFG,           0xC0},
    {CC1200_MDMCFG2,           0x00},
    {CC1200_FREQ2,             0x5B},
    {CC1200_FREQ1,             0x80},
    {CC1200_IF_ADC1,           0xEE},
    {CC1200_IF_ADC0,           0x10},
    {CC1200_FS_DIG1,           0x04},
    {CC1200_FS_DIG0,           0x55},
    {CC1200_FS_CAL1,           0x40},
    {CC1200_FS_CAL0,           0x0E},
    {CC1200_FS_DIVTWO,         0x03},
    {CC1200_FS_DSM0,           0x33},
    {CC1200_FS_DVC0,           0x17},
    {CC1200_FS_PFD,            0x00},
    {CC1200_FS_PRE,            0x6E},
    {CC1200_FS_REG_DIV_CML,    0x1C},
    {CC1200_FS_SPARE,          0xAC},
    {CC1200_FS_VCO0,           0xB5},
    {CC1200_IFAMP,             0x0D},
    {CC1200_XOSC5,             0x0E},
    {CC1200_XOSC1,             0x03},
};

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

// read one packet from fifo
static uint8_t packet[MAX_PACKET_LEN] = {0};
static uint8_t packet_len = 0;
static void Receive_Packet(void) {
    uint8_t len = Read_CC1200(CC1200_NUM_RXBYTES).value;
    if (!len) {
        return;
    }

    SPI_Select();
    SPI_Transfer(CC1200_FIFO | CC1200_READ | CC1200_BURST);
    for (int i = 0; i < len; i++) {
        // read the whole packet even if buffer isn't big enough
        uint8_t data = SPI_Transfer(0);
        if (i < MAX_PACKET_LEN) {
            packet[i] = data;
        }
    }
    SPI_Deselect();
    Command_CC1200(COMMAND_SFRX);

    packet_len = len;
}
