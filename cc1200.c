/*
 * File:   cc1200.c
 * Author: Manav
 *
 * Created on February 26, 2025, 10:34 AM
 */

#include "cc1200.h"

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
