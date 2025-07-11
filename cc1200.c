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

// SPI commands to access data buffers
#define CC1200_ENQUEUE_TX_FIFO 0x3F
#define CC1200_DEQUEUE_RX_FIFO 0xBF

// R/W bits
#define CC1200_READ (1 << 7)
#define CC1200_WRITE 0
#define CC1200_BURST (1 << 6) // indicate burst access

// SPI command to access FIFO memory (or several other areas depending on mode)
#define CC1200_MEM_ACCESS 0x3E

// RX and TX FIFOs are 128 bytes
#define MAX_PACKET_LEN 128
#define PACKET_LEN 0x15 // 21 bytes

#define _XTAL_FREQ 12000000

// Call sign MUST be transmitted at start of every message
const uint64_t CALLSIGN = 0x564133555750; // ASCII "VAEUWP"/Manav

// Register assignments, use MARTRFTM-STUDIO to configure and copy and paste in
// "TrxEB RF Settings Value Line" format
// https://www.ti.com/tool/SMARTRFTM-STUDIO
static const registerSetting_t preferredSettings[] = {
//    {CC1200_SYNC_CFG1, 0x28}, // 11-bit Sync Word
//    {CC1200_SYNC_CFG0, 0x13},    {CC1200_DEVIATION_M, 0x99}, // 124.8MHz deviation
//    {CC1200_MODCFG_DEV_E, 0x05}, // 2-FSK
//    {CC1200_DCFILT_CFG, 0x26},   {CC1200_PREAMBLE_CFG0, 0x8A}, // 3 preamble bytes: 0xAA
//    {CC1200_IQIC, 0x00}, // IQIC disabled
//    {CC1200_CHAN_BW, 0x02}, // RX Filter Bandwidth: 833.3kHz
//    {CC1200_MDMCFG2, 0x00},      {CC1200_MDMCFG1, 0x42},
//    {CC1200_MDMCFG0, 0x05},      {CC1200_SYMBOL_RATE2, 0xC9}, // Symbol rate: 500 ksp
//    {CC1200_SYMBOL_RATE1, 0x99}, {CC1200_SYMBOL_RATE0, 0x99},
//    {CC1200_AGC_REF, 0x2F},      {CC1200_AGC_CS_THR, 0xF7},
//    {CC1200_AGC_CFG1, 0x00},     {CC1200_AGC_CFG0, 0x80},
//    {CC1200_FIFO_CFG, 0x00},     {CC1200_SETTLING_CFG, 0x03},
//    {CC1200_FS_CFG, 0x12}, // 820.0 - 960.0 MHz band (LO divider = 4)
//    {CC1200_WOR_CFG0, 0x20},     {CC1200_WOR_EVENT0_LSB, 0x14},
//
//    {CC1200_IF_MIX_CFG, 0x18},
//
//    {CC1200_TOC_CFG, 0x03},
//
//    {CC1200_FS_DIG0, 0x55}, // loop bandwidth 300kHz in RX and TX
//    {CC1200_FS_CAL0, 0x0E},
//
//    {CC1200_IFAMP, 0x0D},
      {CC1200_IOCFG2,0x06},        //GPIO2 IO Pin Configuration
      {CC1200_SYNC_CFG1,0xA9},     //Sync Word Detection Configuration Reg. 1
      {CC1200_MODCFG_DEV_E,0x0B},  //Modulation Format and Frequency Deviation Configur..
      {CC1200_PREAMBLE_CFG0,0x8A}, //Preamble Detection Configuration Reg. 0
      {CC1200_IQIC,0xC8},          //Digital Image Channel Compensation Configuration
      {CC1200_CHAN_BW,0x10},       //Channel Filter Configuration
      {CC1200_MDMCFG1,0x42},       //General Modem Parameter Configuration Reg. 1
      {CC1200_MDMCFG0,0x05},       //General Modem Parameter Configuration Reg. 0
      {CC1200_SYMBOL_RATE2,0x8F},  //Symbol Rate Configuration Exponent and Mantissa [1..
      {CC1200_SYMBOL_RATE1,0x75},  //Symbol Rate Configuration Mantissa [15:8]
      {CC1200_SYMBOL_RATE0,0x10},  //Symbol Rate Configuration Mantissa [7:0]
      {CC1200_AGC_REF,0x27},       //AGC Reference Level Configuration
      {CC1200_AGC_CS_THR,0x01},    //Carrier Sense Threshold Configuration
      {CC1200_AGC_CFG1,0x11},      //Automatic Gain Control Configuration Reg. 1
      {CC1200_AGC_CFG0,0x94},      //Automatic Gain Control Configuration Reg. 0
      {CC1200_FIFO_CFG,0x00},      //FIFO Configuration
      {CC1200_FS_CFG,0x12},        //Frequency Synthesizer Configuration
      {CC1200_PKT_CFG2,0x00},      //Packet Configuration Reg. 2
      {CC1200_PKT_CFG0,0x20},      //Packet Configuration Reg. 0
      {CC1200_PA_CFG1,0x76},       //Power Amplifier Configuration Reg. 1
      {CC1200_PKT_LEN,0xFF},       //Packet Length Configuration
      {CC1200_IF_MIX_CFG,0x1C},    //IF Mix Configuration
      {CC1200_TOC_CFG,0x03},       //Timing Offset Correction Configuration
      {CC1200_MDMCFG2,0x02},       //General Modem Parameter Configuration Reg. 2
      {CC1200_FREQ2,0x5C},         //Frequency Configuration [23:16]
      {CC1200_FREQ1,0x0F},         //Frequency Configuration [15:8]
      {CC1200_FREQ0,0x5C},         //Frequency Configuration [7:0]
      {CC1200_IF_ADC1,0xEE},       //Analog to Digital Converter Configuration Reg. 1
      {CC1200_IF_ADC0,0x10},       //Analog to Digital Converter Configuration Reg. 0
      {CC1200_FS_DIG1,0x04},       //Frequency Synthesizer Digital Reg. 1
      {CC1200_FS_DIG0,0x55},       //Frequency Synthesizer Digital Reg. 0
      {CC1200_FS_CAL1,0x40},       //Frequency Synthesizer Calibration Reg. 1
      {CC1200_FS_CAL0,0x0E},       //Frequency Synthesizer Calibration Reg. 0
      {CC1200_FS_DIVTWO,0x03},     //Frequency Synthesizer Divide by 2
      {CC1200_FS_DSM0,0x33},       //FS Digital Synthesizer Module Configuration Reg. 0
      {CC1200_FS_DVC0,0x17},       //Frequency Synthesizer Divider Chain Configuration ..
      {CC1200_FS_PFD,0x00},        //Frequency Synthesizer Phase Frequency Detector Con..
      {CC1200_FS_PRE,0x6E},        //Frequency Synthesizer Prescaler Configuration
      {CC1200_FS_REG_DIV_CML,0x1C},//Frequency Synthesizer Divider Regulator Configurat..
      {CC1200_FS_SPARE,0xAC},      //Frequency Synthesizer Spare
      {CC1200_FS_VCO0,0xB5},       //FS Voltage Controlled Oscillator Configuration Reg..
      {CC1200_IFAMP,0x09},         //Intermediate Frequency Amplifier Configuration
      {CC1200_XOSC5,0x0E},         //Crystal Oscillator Configuration Reg. 5
      {CC1200_XOSC1,0x03},         //Crystal Oscillator Configuration Reg. 1
};

static CC1200ReadResult Read_CC1200(uint16_t reg) {
    CC1200ReadResult result;

    SPI_Select();

    // If accessing extended registers
    if (reg >= 0x2F00) {
        // extended register read command
        SPI_Transfer(0x80 | CC1200_EXTENDED_REGISTER);
        result.status = SPI_Transfer(reg & 0xFF);
        result.value = SPI_Transfer(0x00);
    } else {
        result.status = SPI_Transfer(0x80 | reg); // R/W=1 | 0 | register address
        result.value = SPI_Transfer(0x00);
    }

    SPI_Deselect();
    return result;
};

static uint8_t Write_CC1200(uint16_t reg, uint8_t val) {
    uint8_t status;

    SPI_Select();

    // If accessing extended registers
    if (reg >= 0x2F00) {
        // extended register write command
        SPI_Transfer(CC1200_EXTENDED_REGISTER);
        SPI_Transfer(reg & 0xFF);
        status = SPI_Transfer(val);
    } else {
        SPI_Transfer(0x00 | reg);
        status = SPI_Transfer(val);
    }

    SPI_Deselect();
    return status;
};

void CC1200_Frequency(uint32_t freq) {
    // Refer to Section 9.12 (Eqn 27/28, Table 34)
    uint24_t reg_value = (freq * 4 * 65536) / 40000000;
    
    SPI_Select();
    SPI_Transfer(CC1200_EXTENDED_REGISTER | CC1200_BURST);
    SPI_Transfer(CC1200_FREQOFF1);
    SPI_Transfer(0x00); // FREQOFF1 - MSB
    SPI_Transfer(0x00); // FREQOFF0 - LSB
    SPI_Transfer((reg_value >> 16) & 0xFF); // FREQ2 - MSB
    SPI_Transfer((reg_value >> 8) & 0xFF);  // FREQ1 - middle byte
    SPI_Transfer(reg_value & 0xFF);         // FREQ0 - LSB
    SPI_Deselect();
}

void CC1200_XOSC_Config() {
    Write_CC1200(CC1200_XOSC2, 0x05); // XOSC_CORE_PD_OVERRIDE=1
    Write_CC1200(CC1200_XOSC1, 0x03); // XOSC_BUF_SEL=1
}

void CC1200_Packet_Config() {
    SPI_Select();
    SPI_Transfer(CC1200_PKT_CFG2 | CC1200_BURST);
    // Standard packets in normal/FIFO mode
    SPI_Transfer(0x00); // PKT_CFG2
    // Whitening enabled, CRC_CFG=1
    SPI_Transfer(0x43); // PKT_CFG1
    // Fixed length packets,
    SPI_Transfer(0x04); // PKT_CFG0
    // Packet length
    Write_CC1200(CC1200_PKT_LEN, PACKET_LEN);
    SPI_Deselect();
}

void CC1200_RF_Config(void) {
    SPI_Select();
    SPI_Transfer(CC1200_RFEND_CFG1 | CC1200_BURST);
    // After receiving good packet enter RX
    SPI_Transfer(0x3F); // RFEND_CFG1
    // Terminate on bad packets, Antenna diversity, after transmit set to RX mode
    //SPI_Transfer(0x6B); // RFEND_CFG0
    SPI_Transfer(0x18); // RFEND_CFG0 without antenna diversity
    SPI_Deselect();
}

// Configure CC1200 Registers
bool CC1200_Init(void) {
    // RESET_n pin
    TRISC7 = 0;
    LATC7 = 1;

    size_t numSettings = sizeof(preferredSettings) / sizeof(preferredSettings[0]);

    for (size_t i = 0; i < numSettings; i++) {
        Write_CC1200(preferredSettings[i].addr, preferredSettings[i].value);
    }

    // Overwrite GPIO pin configurations
    Write_CC1200(
        CC1200_IOCFG0, 0b00100100
    ); // 0=Digital | 0=Invert output disabled | 100100=Antenna_Select
    Write_CC1200(CC1200_IOCFG3, 0b01011001); // 0=Digital | 1=Invert output disabled | 011001=PA_PD
                                             // (Although we're using for external TRX switch))

    CC1200_Frequency(915000000);
    CC1200_Set_Power(-16);
    CC1200_XOSC_Config();
    CC1200_Packet_Config();
    CC1200_RF_Config();

    CC1200ReadResult part_number = Read_CC1200(CC1200_PARTNUMBER);
    if (part_number.value == 0x20) {
        return true;
    } else {
        return false;
    }
}

void CC1200_Reset(void) {
    Read_CC1200(0x80 | COMMAND_SRES);
}

bool is_CC1200(uint8_t *status) {
    CC1200ReadResult part_number = Read_CC1200(CC1200_PARTNUMBER);
    *status = part_number.status;
    if (part_number.value == 0x20) {
        return true;
    }
    return false;
}

bool CC1200_has_signal(void) {
    CC1200ReadResult modem_status = Read_CC1200(CC1200_MODEM_STATUS1);
    return modem_status.value == 0x8;
}

CC1200ReadResult CC1200_Status() {
    CC1200ReadResult result;
    result.value = 0x00;
    LATA5 = 0; // CS Low
    result.status = SPI_Transfer(0x80 | 0x3D);
    LATA5 = 1; // CS High
    return result;
}

void CC1200_Idle(void) {
    SPI_Select();
    SPI_Transfer(COMMAND_SIDLE);
    SPI_Deselect();
}

uint8_t CC1200_get_TX_FIFO_len(void) {
    CC1200ReadResult FIFO_len = Read_CC1200(CC1200_NUM_TXBYTES);
    return FIFO_len.value;
}

uint8_t CC1200_get_RX_FIFO_len(void) {
    CC1200ReadResult FIFO_len = Read_CC1200(CC1200_NUM_RXBYTES);
    return FIFO_len.value;
}

void CC1200_Transmit(uint32_t sid, uint8_t len, uint64_t data) {
    len = CC1200_get_TX_FIFO_len();
    SPI_Select();
    uint8_t status = SPI_Transfer(CC1200_ENQUEUE_TX_FIFO | CC1200_BURST); // 3.2.4 FIFO access with burst
    //for (int i = 7; i >= 0; i--) {
    //    uint8_t byte = (CALLSIGN >> (i * 8)) & 0xFF;
    //    SPI_Transfer(byte);
    //}
    //for (int i = 7; i >= 0; i--) {
    //    uint8_t byte = (sid >> (i * 8)) & 0xFF;
    //    SPI_Transfer(byte);
    //}
    status = SPI_Transfer(8);
    for (int i = 7; i >= 0; i--) {
        uint8_t byte = (data >> (i * 8)) & 0xFF;
        status = SPI_Transfer(byte);
    }
    SPI_Deselect();
    __delay_ms(100);
    len = CC1200_get_TX_FIFO_len();
    SPI_Select();
    SPI_Transfer(COMMAND_STX); // Enter TX mode
    SPI_Deselect();
}

bool CC1200_has_received_packet(void) {
    uint8_t bytesReceived = CC1200_get_RX_FIFO_len();
    if (bytesReceived < 1) {
        return false;
    }
    // is entire packet received
    return bytesReceived >= PACKET_LEN;
}

void CC1200_Receive(uint64_t *callsign, uint32_t *sid, uint8_t *len, uint64_t *data) {
    uint8_t buffer[PACKET_LEN];

    SPI_Select();
    SPI_Transfer(CC1200_DEQUEUE_RX_FIFO | CC1200_BURST);
    for (int i = 0; i < PACKET_LEN; i++) {
        buffer[i] = SPI_Transfer(0x00);
    }
    SPI_Deselect();

    *callsign = 0;
    for (int i = 0; i < 8; i++) {
        *callsign = (*callsign << 8) | buffer[i];
    }

    *sid = 0;
    for (int i = 8; i < 12; i++) {
        *sid = (*sid << 8) | buffer[i];
    }

    *len = buffer[12];

    *data = 0;
    for (int i = 13; i <= PACKET_LEN; i++) {
        *data = (*data << 8) | buffer[i];
    }
}

void CC1200_Set_Power(int8_t power) {
    // User Guide 7.1 Equation 21
    if (power < -16) power = -16;
    uint8_t reg_value = (uint8_t) (2 * (power + 18)) - 1;
    CC1200ReadResult cur_reg_val = Read_CC1200(CC1200_PA_CFG1);
    
    // Preserve bits 7:6, update bits 5:0 with new reg_value
    uint8_t new_reg_value = (cur_reg_val.value & 0xC0) | (reg_value & 0x3F);
    
    Write_CC1200(CC1200_PA_CFG1, new_reg_value);
}
