#include "adc.h"
#include <xc.h>

void ADC_Init(void) {
    TRISAbits.TRISA0 = 1;   // Set RA0 to input
    ANSELAbits.ANSELA0 = 1; // Set RA0 to analog

    // Configure FVR module
    // b[7] enables FVR
    // b[1:0] sets the reference to 2.048
    FVRCON = 0b10000010;

    // Configure ADC module
    ADCON0bits.FM = 1;     // Right justify
    ADCON0bits.CONT = 1;   // Continuous conversion
    ADCON0bits.CS = 0;     // Drive from Fosc/2
    ADCLKbits.CS = 36;     // Tad = 36 / (12MHz/2) = 6us
    ADCON2bits.MD = 0b100; // Low-pass filter mode
    ADCON2bits.CRS = 1;    // 0.72 wT filter cut-off (~13 kHz)
    ADREFbits.NREF = 0;    // Negative reference is GND
    ADREFbits.PREF = 0b11; // Positive reference is FVR
    ADPCH = 0;             // ADC channel to A0
    ADCON0bits.ADON = 1;   // Turn ADC on
}

uint16_t ADC_read_raw(void) {
    return (uint16_t) (ADFLTRH << 8) | ADFLTRL;
}

uint16_t ADC_read_curr_ma(void) {
    return ADC_read_raw() * 5 / 16; // 2048 mV ref / 12 bit / 100 V/V / 16mR
}
