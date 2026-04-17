/*
 * File:   ltt_rf.c
 * Author: Manav
 *
 * Created on April 16, 2026, 11:38 AM
 */

#include "ltt_rf.h"
#include <xc.h>

#include "cc1200.h"
#include "spi.h"

void CC1200_Set_Power(int8_t power) {
    if (power > 14) {
        power = 14;
    }

    uint8_t reg_value = 0;

    // special low power modes https://e2e.ti.com/support/wireless-connectivity/sub-1-ghz-group/sub-1-ghz/f/sub-1-ghz-forum/448235/cc1200--38dbm-tx-power-output
    if (power <= -70) {
        reg_value = 0x00;
    } else if (power <= -32) {
        reg_value = 0x01;
    } else if (power <= -26) {
        reg_value = 0x02;
    } else { // User Guide 7.1 Equation 21
        // Pout = (reg + 1) / 2 - 18 [dBm]
        int16_t tmp = 2 * (power + 18) - 1;
        reg_value = (uint8_t)tmp;
    }

    // Preserve bits 7:6, update bits 5:0 with new reg_value
    CC1200ReadResult cur_reg_val = Read_CC1200(CC1200_PA_CFG1);
    uint8_t new_reg_value = (cur_reg_val.value & 0xC0) | (reg_value & 0x3F);
    Write_CC1200(CC1200_PA_CFG1, new_reg_value);
}

void CC1200_Frequency(uint32_t freq) {
    // freq in kHz
    // Refer to Section 9.12 (Eqn 27/28, Table 34)
    uint32_t reg_value = ((uint64_t)freq * 4096) / 625; 

    SPI_Select();
    SPI_Transfer(CC1200_EXTENDED_REGISTER | CC1200_BURST);
    SPI_Transfer(CC1200_FREQ2 & 0xFF);
    SPI_Transfer((reg_value >> 16) & 0xFF); // FREQ2 - MSB
    SPI_Transfer((reg_value >> 8) & 0xFF);  // FREQ1 - middle byte
    SPI_Transfer(reg_value & 0xFF);         // FREQ0 - LSB
    SPI_Deselect();
}

RF_Init() {
    CC1200_Init();
    
    CC1200_Frequency(915000);
    CC1200_Set_Power(14);
}

RF_Receive(void) {
    uint8_t data[64] = {0};
    uint8_t rx_len = CC1200_Receive(data, sizeof(data));
    
    if (rx_len) {
        // counter increments by one and crc check passes
        if (!(data[rx_len-1] & 0x80)) {

        }
    }
}