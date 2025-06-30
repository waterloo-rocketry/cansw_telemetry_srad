#include <stdbool.h>
#include <xc.h>

#include "spi.h"

void SPI_Init(void) {
    RA5PPS = 0x20;
    RC3PPS = 0x1E;
    RC4PPS = 0x1F; // Set RC4 to MOSI
    SPI1SDIPPS = 0b10101; // Set RC5 to MISO

    TRISA5 = 0;
    TRISC3 = 0;
    TRISC4 = 0;
    TRISC5 = 1;

    SPI1CLKbits.CLKSEL = 0;
    SPI1BAUD = 0x22;

    SPI1CON0bits.MST = 1; // set mode to master
    SPI1CON0bits.BMODE = 1; // sets bit mode to constant width

    SPI1CON1bits.CKP = 0; // idle state for clk is low
    SPI1CON1bits.CKE = 1; // change data on falling edge
    SPI1CON1bits.SSP = 1; // make cs active low
    SPI1CON1bits.SDOP = 0;
    SPI1CON1bits.SDIP = 0;

    SPI1CON2bits.SSET = 0; // cs controlled by transfer counter
    SPI1CON2bits.TXR = 1; // transmit required for transfer
    SPI1CON2bits.RXR = 1; // receive data in FIFO
    SPI1TWIDTH = 0; // 8 bits

    // Enable SPI
    SPI1CON0bits.EN = 1; // enable SPI
}

/**/
uint8_t SPI_Transfer(uint8_t data) {
    SPI1TXB = data;
    while (SPI1CON2bits.BUSY) { }
    data = SPI1RXB;
    return data; // reading should remove top most byte
}

void SPI_Select(uint8_t byte_count) {
    SPI1TCNT = byte_count;
}
