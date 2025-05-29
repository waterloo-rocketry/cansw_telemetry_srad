/*
 * File:   cc1200.h
 * Author: Manav
 *
 * Created on February 26, 2025, 10:31 AM
 */

#ifndef CC1200_H
#define CC1200_H

#include "spi.h"
#include "stdint.h"
#include "stdbool.h"
#include <xc.h>

typedef struct {
    uint8_t addr;
    uint8_t value;
} registerSetting_t;

typedef struct {
    uint8_t status;
    uint8_t value;
} CC1200ReadResult;

CC1200ReadResult Write_CC1200(uint8_t, uint8_t);

CC1200ReadResult Read_CC1200(uint8_t);

bool CC1200_Init(void);

void CC1200_Reset(void);

void CC1200_RX(void);

void CC1200_Transmit(void);

CC1200ReadResult CC1200_Status(void);

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

// Other registers: See User manual Table 4/5
#define CC1200_IOCFG3 0x00 // GPIO3 Pin 3 Configuration
#define CC1200_RFEND_CFG0 0x2A // RFEND Configuration Rg. 0
#define CC1200_PARTNUMBER 0x8F // Part number

// Command Strobes: See user guide section 3.2.2
#define COMMAND_SOFT_RESET = 0x30
#define COMMAND_FAST_TX_ON = 0x31
#define COMMAND_OSC_OFF = 0x32
#define COMMAND_CAL_FREQ_SYNTH = 0x33
#define COMMAND_RX = 0x34
#define COMMAND_TX = 0x35
#define COMMAND_IDLE 0x36
#define COMMAND_AUTO_FREQ_COMP = 0x37
#define COMMAND_WAKE_ON_RADIO = 0x38
#define COMMAND_SLEEP = 0x39
#define COMMAND_FLUSH_RX = 0x3A
#define COMMAND_FLUSH_TX = 0x3B
#define COMMAND_WOR_RESET = 0x3C
#define COMMAND_NOP = 0x3D

// State of chip: See user guide Figure 2
#define STATE_IDLE 0x00
#define STATE_RX 0x01
#define STATE_TX 0x02
#define STATE_FAST_ON 0x03
#define STATE_CALIBRATE 0x04
#define STATE_SETTLING 0x05
#define STATE_RX_FIFO_ERROR 0x06
#define STATE_TX_FIFO_ERROR 0x07

#endif
