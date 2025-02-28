/*
 * File:   cc1200.c
 * Author: Manav
 *
 * Created on February 26, 2025, 10:34 AM
 */


#include "cc1200.h"

void Write_CC1200(uint8_t reg, uint8_t val){
    LATA5 = 0; // CS Low
    while (PORTCbits.RC4); // Wait for MISO to go low
    if (reg > 0x2F) {
        // extended registers
        Write_SPI(0b00101111);
        Write_SPI(reg);
    } else {
        // If less than 2F, first two bits will be 0
        // these set mode to W and no burst bit
        Write_SPI(reg);
    }
    
    Write_SPI(val);
    LATA5 = 1; // CS High
};

uint8_t Read_CC1200(uint8_t reg){
    uint8_t val;
    LATA5 = 0; // CS Low
    while (PORTCbits.RC4); // Wait for MISO to go low
    if (reg > 0x2F) {
        // extended registers
        Write_SPI(0b10101111);
        Write_SPI(reg);
    } else {
        // If less than 2F, first two bits will be 0
        // set to 1 to enable Read 
        reg |= (1 << 7);
        Write_SPI(reg);
    }
    val = Read_SPI();
    LATA5 = 1; // CS High
    return val;
};
