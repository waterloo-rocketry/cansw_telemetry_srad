/*
 * File:   cc1200.c
 * Author: Manav
 *
 * Created on February 26, 2025, 10:34 AM
 */

#include "cc1200.h"

// CC1200 register addresses 
// This only includes registers which are configured through SmartRF Studio
#define CC1200_IOCFG2 0x0001 // GPIO2 IO Pin Configuration
#define CC1200_IOCFG0 0x0003 // GPIO0 IO Pin Configuration
#define CC1200_SYNC_CFG1 0x0008 // Sync Word Detection Configuration Reg. 1
#define CC1200_SYNC_CFG0 0x0009 // Sync Word Detection Configuration Reg. 0
#define CC1200_DEVIATION_M 0x000A // Frequency Deviation Configuration
#define CC1200_MODCFG_DEV_E 0x000B // Modulation Format and Frequency Deviation Configur..
#define CC1200_DCFILT_CFG 0x000C // Digital DC Removal Configuration
#define CC1200_PREAMBLE_CFG1 0x000D // Preamble Length Configuration Reg. 1
#define CC1200_PREAMBLE_CFG0 0x000E // Preamble Detection Configuration Reg. 0
#define CC1200_IQIC 0x000F // Digital Image Channel Compensation Configuration
#define CC1200_CHAN_BW 0x0010 // Channel Filter Configuration
#define CC1200_MDMCFG1 0x0011 // General Modem Parameter Configuration Reg. 1
#define CC1200_MDMCFG0 0x0012 // General Modem Parameter Configuration Reg. 0
#define CC1200_SYMBOL_RATE2 0x0013 // Symbol Rate Configuration Exponent and Mantissa [1..
#define CC1200_SYMBOL_RATE1 0x0014 // Symbol Rate Configuration Mantissa [15:8]
#define CC1200_SYMBOL_RATE0 0x0015 // Symbol Rate Configuration Mantissa [7:0]
#define CC1200_AGC_REF 0x0016 // AGC Reference Level Configuration
#define CC1200_AGC_CS_THR 0x0017 // Carrier Sense Threshold Configuration
#define CC1200_AGC_CFG1 0x001B // Automatic Gain Control Configuration Reg. 1
#define CC1200_AGC_CFG0 0x001C // Automatic Gain Control Configuration Reg. 0
#define CC1200_FIFO_CFG 0x001D // FIFO Configuration
#define CC1200_FS_CFG 0x0020 // Frequency Synthesizer Configuration
#define CC1200_PKT_CFG2 0x0026 // Packet Configuration Reg. 2
#define CC1200_PKT_CFG1 0x0027 // Packet Configuration Reg. 1
#define CC1200_PKT_CFG0 0x0028 // Packet Configuration Reg. 0
#define CC1200_PA_CFG1 0x002B // Power Amplifier Configuration Reg. 1
#define CC1200_PKT_LEN 0x002E // Packet Length Configuration
#define CC1200_IF_MIX_CFG 0x2F00 // IF Mix Configuration
#define CC1200_TOC_CFG 0x2F02 // Timing Offset Correction Configuration
#define CC1200_MDMCFG2 0x2F05 // General Modem Parameter Configuration Reg. 2
#define CC1200_FREQ2 0x2F0C // Frequency Configuration [23:16]
#define CC1200_FREQ1 0x2F0D // Frequency Configuration [15:8]
#define CC1200_FREQ0 0x2F0E // Frequency Configuration [7:0]
#define CC1200_IF_ADC1 0x2F10 // Analog to Digital Converter Configuration Reg. 1
#define CC1200_IF_ADC0 0x2F11 // Analog to Digital Converter Configuration Reg. 0
#define CC1200_FS_DIG1 0x2F12 // Frequency Synthesizer Digital Reg. 1
#define CC1200_FS_DIG0 0x2F13 // Frequency Synthesizer Digital Reg. 0
#define CC1200_FS_CAL1 0x2F16 // Frequency Synthesizer Calibration Reg. 1
#define CC1200_FS_CAL0 0x2F17 // Frequency Synthesizer Calibration Reg. 0
#define CC1200_FS_DIVTWO 0x2F19 // Frequency Synthesizer Divide by 2
#define CC1200_FS_DSM0 0x2F1B // FS Digital Synthesizer Module Configuration Reg. 0
#define CC1200_FS_DVC0 0x2F1D // Frequency Synthesizer Divider Chain Configuration ..
#define CC1200_FS_PFD 0x2F1F // Frequency Synthesizer Phase Frequency Detector Con..
#define CC1200_FS_PRE 0x2F20 // Frequency Synthesizer Prescaler Configuration
#define CC1200_FS_REG_DIV_CML 0x2F21 // Frequency Synthesizer Divider Regulator Configurat..
#define CC1200_FS_SPARE 0x2F22 // Frequency Synthesizer Spare
#define CC1200_FS_VCO0 0x2F27 // FS Voltage Controlled Oscillator Configuration Reg..
#define CC1200_IFAMP 0x2F2F // Intermediate Frequency Amplifier Configuration
#define CC1200_XOSC5 0x2F32 // Crystal Oscillator Configuration Reg. 5
#define CC1200_XOSC1 0x2F36 // Crystal Oscillator Configuration Reg. 1
#define CC1200_SERIAL_STATUS 0x2F91 // Serial Status

// Other registers
#define CC1200_IOCFG3 0x00 // GPIO3 Pin 3 Configuration
#define CC1200_RFEND_CFG0 0x2A // RFEND Configuration Rg. 0

// Register assignments, use MARTRFTM-STUDIO to configure and copy and paste in
// "TrxEB RF Settings Value Line" format
// https://www.ti.com/tool/SMARTRFTM-STUDIO
static const registerSetting_t preferredSettings[]= 
{
  {CC1200_IOCFG2,            0x08},
  {CC1200_IOCFG0,            0x09},
  {CC1200_SYNC_CFG1,         0xAB},
  {CC1200_SYNC_CFG0,         0x13},
  {CC1200_DEVIATION_M,       0x99},
  {CC1200_MODCFG_DEV_E,      0x05},
  {CC1200_DCFILT_CFG,        0x26},
  {CC1200_PREAMBLE_CFG1,     0x00},
  {CC1200_PREAMBLE_CFG0,     0x8A},
  {CC1200_IQIC,              0x00},
  {CC1200_CHAN_BW,           0x02},
  {CC1200_MDMCFG1,           0x06},
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
  {CC1200_PKT_CFG2,          0x01},
  {CC1200_PKT_CFG1,          0x40},
  {CC1200_PKT_CFG0,          0x20},
  {CC1200_PA_CFG1,           0x3F},
  {CC1200_PKT_LEN,           0xFF},
  {CC1200_IF_MIX_CFG,        0x18},
  {CC1200_TOC_CFG,           0x03},
  {CC1200_MDMCFG2,           0x01},
  {CC1200_FREQ2,             0x5C},
  {CC1200_FREQ1,             0x0F},
  {CC1200_FREQ0,             0x5C},
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
  {CC1200_SERIAL_STATUS,     0x08},
};

// Communicating to CC1200 via SPI
// PIC pulls CC1200 LOW
// PIC wait for MISO to go LOW (takes time to go low if chip was reset or in sleep mode))
// PIC sens a [Command then Address] OR PIC sens an [Address]]
// CC1200 has burst bits (for writing to buffers with consecutive addresses), it is not used)
// Refer to CC1200 data sheet for more information:
// https://www.ti.com/lit/ug/swru346b/swru346b.pdf?ts=1746854018299&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FCC1200


CC1200ReadResult Read_CC1200(uint8_t reg) {
    CC1200ReadResult result;

    LATA5 = 0; // CS Low
    while (PORTCbits.RC4) {} // Wait for MISO to go low
    
    // If accessing extended registers
    if (reg > 0x2F) {
        // extended register access command
        SPI_Transfer(0xAF); // R/W=1 | 2F
        result.status = SPI_Transfer(reg);
        result.value = SPI_Transfer(0x00);
    } else {
        result.status = SPI_Transfer(0x80 | reg); // R/W=1 | 0 | register address
        result.value = SPI_Transfer(0x00);
    }
    LATA5 = 1; // CS High
    return result;
};

CC1200ReadResult Write_CC1200(uint8_t reg, uint8_t val) {
    CC1200ReadResult result;
    result.value = 0x00;
    
    LATA5 = 0; // CS Low
    while(PORTCbits.RC4) {} // Wait for MISO to go low
    
    // If accessing extended registers
    if (reg > 0x2F) {
        // extended register access command
        SPI_Transfer(0x2F);
        SPI_Transfer(reg);
        result.status = SPI_Transfer(val);
    } else {
        SPI_Transfer(0x00 | reg);
        result.status = SPI_Transfer(val);
    }
    LATA5 = 1; // CS High
    return result;
};

// Configure CC1200 Registers
void CC1200_Init(void) {
    size_t numSettings = sizeof(preferredSettings) / sizeof(preferredSettings[0]);

    for (size_t i = 0; i < numSettings; i++) {
        Write_CC1200(preferredSettings[i].addr, preferredSettings[i].value);
    }
    
    // Overwrite GPIO pin configurations
    Write_CC1200(CC1200_IOCFG0, 0b00100100); // 0=Digital | 0=Invert output disabled | 100100=Antenna_Select
    Write_CC1200(CC1200_IOCFG3, 0b01011001); // 0=Digital | 1=Invert output disabled | 011000=PA_PD (Although we're using for external TRX switch))
    
    // Setup Antenna Switching 
    Write_CC1200(CC1200_RFEND_CFG0, 0b00000011); // X | 0=CAL_END_WAKE_UP_EN | 00=TXOFF_MODE | 0=TERM_ON_BAD_PACKET_EN | 011=ANT_DIV_RX_TERM_CFG 
}
