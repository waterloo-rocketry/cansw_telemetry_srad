#include "eeprom.h"
#include <xc.h>

#define EEPROM_DEFAULT_FREQ 904000
#define EEPROM_DEFAULT_POWER 8 // 8dBm is already 33dBm after PA, any more is jsut saturating the PA

#define EEPROM_ADDR_FREQ 0
#define EEPROM_ADDR_POWER 4

__EEPROM_DATA((EEPROM_DEFAULT_FREQ >> 0) & 0xFF, (EEPROM_DEFAULT_FREQ >> 8) & 0xFF,
			  (EEPROM_DEFAULT_FREQ >> 16) & 0xFF, (EEPROM_DEFAULT_FREQ >> 24) & 0xFF,
			  EEPROM_DEFAULT_POWER, 0, 0, 0);

// eeprom_read and eeprom_write from xc.h doesn't work anymore, write our own
// From datasheet EXAMPLE 13-5 and 13-6
uint8_t eeprom_read2(uint8_t addr) {
	NVMCON1 = 0;
	NVMADRL = addr;
	NVMCON1bits.RD = 1;
	return NVMDAT;
}

void eeprom_write2(uint8_t addr, uint8_t data) {
	NVMCON1 = 0;
	NVMADRL = addr;
	NVMDAT = data;
	NVMCON1bits.WREN = 1;

	// disable interrupt
	uint8_t GIE = INTCON0bits.GIE;
	INTCON0bits.GIE = 0;

	// unlock sequence
	NVMCON2 = 0x55;
	NVMCON2 = 0xAA;

	NVMCON1bits.WR = 1;
	while (NVMCON1bits.WR)
		;
	NVMCON1bits.WREN = 0;

	// restore interrupt
	INTCON0bits.GIE = GIE;
}

bool eeprom_set_frequency(uint32_t frequency) {
	if (frequency >= 902000 && frequency <= 928000 && eeprom_get_frequency() != frequency) {
		eeprom_write2(EEPROM_ADDR_FREQ + 0, (frequency >> 0) & 0XFF);
		eeprom_write2(EEPROM_ADDR_FREQ + 1, (frequency >> 8) & 0XFF);
		eeprom_write2(EEPROM_ADDR_FREQ + 2, (frequency >> 16) & 0XFF);
		eeprom_write2(EEPROM_ADDR_FREQ + 3, (frequency >> 24) & 0XFF);
		return true;
	}
	return false;
}

bool eeprom_set_power(int8_t power) {
	if (eeprom_get_power() != power) {
		// let CC1200_Set_Power clip the power
		eeprom_write2(EEPROM_ADDR_POWER, (uint8_t)power);
		return true;
	}
	return false;
}

uint32_t eeprom_get_frequency(void) {
	uint32_t eeprom_frequency = ((uint32_t)eeprom_read2(EEPROM_ADDR_FREQ + 0) << 0) |
								((uint32_t)eeprom_read2(EEPROM_ADDR_FREQ + 1) << 8) |
								((uint32_t)eeprom_read2(EEPROM_ADDR_FREQ + 2) << 16) |
								((uint32_t)eeprom_read2(EEPROM_ADDR_FREQ + 3) << 24);
	return eeprom_frequency;
}

int8_t eeprom_get_power(void) {
	int8_t eeprom_power = (int8_t)eeprom_read2(EEPROM_ADDR_POWER);
	return eeprom_power;
}
