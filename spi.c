#include <stdbool.h>
#include <xc.h>

#include "spi.h"

void SPI_Init(void) {
    TRISA5 = 0;
    TRISC3 = 0;
    TRISC4 = 1;
    TRISC5 = 0;
    ANSELC4 = 0;

    //RA5PPS = 0x20; // RA5 to CS
    RC3PPS = 0x1E; // RC3 to SCLK
    RC5PPS = 0x1F; // RC5 to MOSI

    SPI1SDIPPS = 0b10100; // Set RC4 to MISO

    SPI1CLKbits.CLKSEL = 0; // Fosc as clock
    SPI1BAUD = 0x0; // 6MHz baud

    SPI1CON0bits.MST = 1; // set mode to master
    SPI1CON0bits.BMODE = 1; // sets bit mode to constant width

    SPI1CON1bits.CKP = 0; // idle state for clk is low
    SPI1CON1bits.CKE = 1; // change data on falling edge
    SPI1CON1bits.SSP = 1; // make cs active low
    SPI1CON1bits.SDOP = 0;
    SPI1CON1bits.SDIP = 0;

    SPI1CON2bits.TXR = 1; // transmit required for transfer
    SPI1CON2bits.RXR = 1; // receive data in FIFO
    SPI1TWIDTH = 0; // 8 bits

    // Enable SPI
    SPI1CON0bits.EN = 1; // enable SPI
}

uint8_t SPI_Transfer(uint8_t data) {
    while (!PIR2bits.SPI1TXIF);
    SPI1TXB = data;
    while (!PIR2bits.SPI1RXIF);
    data = SPI1RXB;
    return data;
}

void SPI_Select(void) {
    //SPI1CON2bits.SSET = 1; // manually set cs
    //SPI1TCNT = byte_count;
    LATA5 = 0;
    while (PORTCbits.RC4); // wait for MISO to go low, TODO add a timeout and return failure
}

void SPI_Deselect(void) {
    LATA5 = 1;
    //SPI1CON2bits.SSET = 0; // automatically set cs
}
