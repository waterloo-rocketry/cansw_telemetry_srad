/*
 * File:   spi.c
 * Author: Manav
 *
 * Created on February 22, 2025, 12:52 PM
 */

#include "spi.h"

void SPI_Init() {
    // Set Pins as outputs
    TRISCbits.TRISC3 = 0; // SCLK
    // NOTE VERY IMPORTANT
    // I am a silly goose and accidently swapped the MISO and MOSI lines 
    // on the PCB, this configuration matches that
    SPI1SDIPPS = 0b10101; // Set RC5 to MISO
    RC4PPS = 0x1F; // Set RC4 to MOSI
    TRISCbits.TRISC4 = 0; // Output
    TRISCbits.TRISC5 = 1; // Input
    
    TRISAbits.TRISA5 = 0; // CS

    LATAbits.LATA5 = 1; // Set CS high

    SPI1CON0bits.EN = 0; // Disable SPI to configure settings

    SPI1CON0bits.MST = 1; // Operate as Master
    SPI1CON0bits.LSBF = 0; // MSB First

    SPI1CON1bits.CKP = 0; // SCLK idles low
    SPI1CON1bits.CKE = 0; // Change data on falling edge

    // Full Duplex Mode
    SPI1CON2bits.TXR = 1;
    SPI1CON2bits.RXR = 1;

    // Clock Speed
    SPI1BAUD = 0; // (12 MHz / 2*(0+1)) = 6 MHz

    SPI1CON0bits.EN = 1; // Enable SPI
}

uint8_t SPI_Transfer(uint8_t data) {
    // Wait for the transmit buffer to be empty
    while (!SPI1STATUSbits.TXBE) {}

    // Write data to transmit buffer
    SPI1TXB = data;

    // Wait until data is received
    while (!SPI1STATUSbits.RXBF) {}

    // Read and return the received data
    return SPI1RXB;
}