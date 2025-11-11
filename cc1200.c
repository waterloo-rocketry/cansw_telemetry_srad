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

// R/W bits
#define CC1200_READ (1 << 7)
#define CC1200_WRITE 0
#define CC1200_BURST (1 << 6) // indicate burst access

// SPI command to access FIFO memory (or several other areas depending on mode)
#define CC1200_MEM_ACCESS 0x3E

// RX and TX FIFOs are 128 bytes
#define MAX_PACKET_LEN 128

#define _XTAL_FREQ 12000000

// Call sign MUST be transmitted at start of every message
const uint64_t CALLSIGN = 0x564133555750; // ASCII "VAEUWP"/Manav

// Register assignments, use MARTRFTM-STUDIO to configure and copy and paste in
// "TrxEB RF Settings Value Line" format
// https://www.ti.com/tool/SMARTRFTM-STUDIO

static const registerSetting_t preferredSettings[] = {
    {CC1200_IOCFG3,            0x57}, // GPIO3 IO Pin Configuration
    {CC1200_IOCFG0,            0x73}, // GPIO0 IO Pin Configuration
    {CC1200_SYNC_CFG1,         0xA8}, // Sync Word Detection Configuration Reg. 1
    {CC1200_SYNC_CFG0,         0x23}, // Sync Word Detection Configuration Reg. 0
    {CC1200_DEVIATION_M,       0x47}, // Frequency Deviation Configuration
    {CC1200_MODCFG_DEV_E,      0x0C}, // Modulation Format and Frequency Deviation Configur..
    {CC1200_DCFILT_CFG,        0x4B}, // Digital DC Removal Configuration
    {CC1200_PREAMBLE_CFG0,     0x8A}, // Preamble Detection Configuration Reg. 0
    {CC1200_IQIC,              0xD8}, // Digital Image Channel Compensation Configuration
    {CC1200_CHAN_BW,           0x08}, // Channel Filter Configuration
    {CC1200_MDMCFG1,           0x42}, // General Modem Parameter Configuration Reg. 1
    {CC1200_MDMCFG0,           0x05}, // General Modem Parameter Configuration Reg. 0
    {CC1200_SYMBOL_RATE2,      0xA4}, // Symbol Rate Configuration Exponent and Mantissa [1..
    {CC1200_SYMBOL_RATE1,      0x7A}, // Symbol Rate Configuration Mantissa [15:8]
    {CC1200_SYMBOL_RATE0,      0xE1}, // Symbol Rate Configuration Mantissa [7:0]
    {CC1200_AGC_REF,           0x2A}, // AGC Reference Level Configuration
    {CC1200_AGC_CS_THR,        0x01}, // Carrier Sense Threshold Configuration
    {CC1200_AGC_CFG1,          0x12}, // Automatic Gain Control Configuration Reg. 1
    {CC1200_AGC_CFG0,          0x80}, // Automatic Gain Control Configuration Reg. 0
    {CC1200_FIFO_CFG,          0x00}, // FIFO Configuration
    {CC1200_FS_CFG,            0x12}, // Frequency Synthesizer Configuration
    {CC1200_PKT_CFG2,          0x00}, // Packet Configuration Reg. 2
    {CC1200_PKT_CFG0,          0x20}, // Packet Configuration Reg. 0
    {CC1200_RFEND_CFG1,        0x3F}, // FEND Configuration Reg. 1
    {CC1200_RFEND_CFG0,        0x30}, // FEND Configuration Reg. 0
    {CC1200_PA_CFG1,           0x5F}, // Power Amplifier Configuration Reg. 1
    {CC1200_PKT_LEN,           0xFF}, // Packet Length Configuration
    {CC1200_IF_MIX_CFG,        0x1C}, // IF Mix Configuration
    {CC1200_TOC_CFG,           0x03}, // Timing Offset Correction Configuration
    {CC1200_MDMCFG2,           0x02}, // General Modem Parameter Configuration Reg. 2
    {CC1200_FREQOFF1,          0x02}, // Frequency Offset MSB
    {CC1200_FREQOFF0,          0xB6}, // Frequency Offset LSB
    {CC1200_FREQ2,             0x5B}, // Frequency Configuration [23:16]
    {CC1200_FREQ1,             0x80}, // Frequency Configuration [15:8]
    {CC1200_IF_ADC1,           0xEE}, // Analog to Digital Converter Configuration Reg. 1
    {CC1200_IF_ADC0,           0x10}, // Analog to Digital Converter Configuration Reg. 0
    {CC1200_FS_DIG1,           0x04}, // Frequency Synthesizer Digital Reg. 1
    {CC1200_FS_DIG0,           0x55}, // Frequency Synthesizer Digital Reg. 0
    {CC1200_FS_CAL1,           0x40}, // Frequency Synthesizer Calibration Reg. 1
    {CC1200_FS_CAL0,           0x0E}, // Frequency Synthesizer Calibration Reg. 0
    {CC1200_FS_DIVTWO,         0x03}, // Frequency Synthesizer Divide by 2
    {CC1200_FS_DSM0,           0x33}, // FS Digital Synthesizer Module Configuration Reg. 0
    {CC1200_FS_DVC0,           0x17}, // Frequency Synthesizer Divider Chain Configuration ..
    {CC1200_FS_PFD,            0x00}, // Frequency Synthesizer Phase Frequency Detector Con..
    {CC1200_FS_PRE,            0x6E}, // Frequency Synthesizer Prescaler Configuration
    {CC1200_FS_REG_DIV_CML,    0x1C}, // Frequency Synthesizer Divider Regulator Configurat..
    {CC1200_FS_SPARE,          0xAC}, // Frequency Synthesizer Spare
    {CC1200_FS_VCO0,           0xB5}, // FS Voltage Controlled Oscillator Configuration Reg..
    {CC1200_IFAMP,             0x09}, // Intermediate Frequency Amplifier Configuration
    {CC1200_XOSC5,             0x0E}, // Crystal Oscillator Configuration Reg. 5
    {CC1200_XOSC1,             0x03}, // Crystal Oscillator Configuration Reg. 1
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

void CC1200_Frequency(uint32_t freq) {
    // freq in kHz
    // Refer to Section 9.12 (Eqn 27/28, Table 34)
    uint32_t reg_value = freq * 4096 / 625; // 4096 / 625 = 1000 * 4 * 65536 / 40000;

    SPI_Select();
    SPI_Transfer(CC1200_EXTENDED_REGISTER | CC1200_BURST);
    SPI_Transfer(CC1200_FREQ2 & 0xFF);
    SPI_Transfer((reg_value >> 16) & 0xFF); // FREQ2 - MSB
    SPI_Transfer((reg_value >> 8) & 0xFF);  // FREQ1 - middle byte
    SPI_Transfer(reg_value & 0xFF);         // FREQ0 - LSB
    SPI_Deselect();
}

// Configure CC1200 Registers
void CC1200_Init(void) {
    // configure RESET_n pin
    TRISC7 = 0;
    LATC7 = 0;
    __delay_ms(100);
    LATC7 = 1;

    size_t numSettings = sizeof(preferredSettings) / sizeof(preferredSettings[0]);
    for (size_t i = 0; i < numSettings; i++) {
        Write_CC1200(preferredSettings[i].addr, preferredSettings[i].value);
    }

    CC1200_Frequency(915000);
    CC1200_Set_Power(0);
}

void CC1200_Transmit(uint8_t *data, uint8_t len) {
    // TODO check space in FIFO
    SPI_Select();
    uint8_t status = SPI_Transfer(CC1200_FIFO | CC1200_BURST); // 3.2.4 FIFO access with burst
    SPI_Transfer(len);
    for (int i = 0; i < len; i++) {
        SPI_Transfer(data[i]);
    }
    SPI_Deselect();
    Command_CC1200(COMMAND_STX);
}

uint8_t CC1200_Receive(uint8_t *data, uint8_t len) {
    uint8_t fifo_len = Read_CC1200(CC1200_NUM_RXBYTES).value;
    if (len > fifo_len) {
        len = fifo_len;
    }

    SPI_Select();
    SPI_Transfer(CC1200_FIFO | CC1200_READ | CC1200_BURST);
    for (int i = 0; i < len; i++) {
        // read the whole packet even if buffer isn't big enough
        data[i] = SPI_Transfer(0);
    }
    SPI_Deselect();

    return len;
}

uint8_t CC1200_State_Transition(void){
    uint8_t state = (Command_CC1200(COMMAND_SNOP) >> 4) & 0x7;
    switch (state) {
        case STATE_IDLE:
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

void CC1200_Set_Power(int8_t power) {
    if (power > 14) {
        power = 14;
    }

    uint8_t reg_value = 0;

    // special low power modes https://e2e.ti.com/support/wireless-connectivity/sub-1-ghz-group/sub-1-ghz/f/sub-1-ghz-forum/448235/cc1200--38dbm-tx-power-output
    if (power <= -70) {
        reg_value = 0x00;
    } else if (power <= -32) {
        reg_value = 0x01;
    } else if (power <= -26) {
        reg_value = 0x02;

    } else { // User Guide 7.1 Equation 21
        // Pout = (reg + 1) / 2 - 18 [dBm]
        reg_value = (uint8_t) (2 * (power + 18)) - 1;
    }

    // Preserve bits 7:6, update bits 5:0 with new reg_value
    CC1200ReadResult cur_reg_val = Read_CC1200(CC1200_PA_CFG1);
    uint8_t new_reg_value = (cur_reg_val.value & 0xC0) | (reg_value & 0x3F);
    Write_CC1200(CC1200_PA_CFG1, new_reg_value);
}
