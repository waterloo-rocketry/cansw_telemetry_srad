#ifndef EEPROM_H
#define EEPROM_H

#include <stdint.h>
#include <stdbool.h>

bool eeprom_set_frequency(uint32_t);
bool eeprom_set_power(int8_t);
uint32_t eeprom_get_frequency(void);
int8_t eeprom_get_power(void);

#endif
