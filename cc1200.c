/*
 * File:   cc1200.c
 * Author: Manav
 *
 * Created on February 26, 2025, 10:34 AM
 */

#include "cc1200.h"

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
bool CC1200_Init(void) {
    size_t numSettings = sizeof(preferredSettings) / sizeof(preferredSettings[0]);

    for (size_t i = 0; i < numSettings; i++) {
        Write_CC1200(preferredSettings[i].addr, preferredSettings[i].value);
    }
    
    // Overwrite GPIO pin configurations
    Write_CC1200(CC1200_IOCFG0, 0b00100100); // 0=Digital | 0=Invert output disabled | 100100=Antenna_Select
    Write_CC1200(CC1200_IOCFG3, 0b01011001); // 0=Digital | 1=Invert output disabled | 011000=PA_PD (Although we're using for external TRX switch))
    
    // Setup Antenna Switching 
    Write_CC1200(CC1200_RFEND_CFG0, 0b00000011); // X | 0=CAL_END_WAKE_UP_EN | 00=TXOFF_MODE | 0=TERM_ON_BAD_PACKET_EN | 011=ANT_DIV_RX_TERM_CFG 

    CC1200ReadResult part_number = Read_CC1200(CC1200_PARTNUMBER);
    if (part_number.value == 0x20) {
        return true;
    } else {
        return false;
    }
}

void CC1200_Reset(void) {
    Read_CC1200(0x80 | 0x30);
}

void CC1200_RX(void) {
    return;
}

void CC1200_Transmit(void) {
    CC1200ReadResult result;
    LATA5 = 0; // CS Low
    SPI_Transfer(0x80 | 0x3E);
    
    LATA5 = 1; // CS High
}

CC1200ReadResult CC1200_Status(void) {
    CC1200ReadResult result;
    result.value = 0x00;
    LATA5 = 0; // CS Low
    result.status = SPI_Transfer(0x80 | 0x3D);
    LATA5 = 1; // CS High
    return result;
}
