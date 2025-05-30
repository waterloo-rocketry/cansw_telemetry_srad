/*
 * File:   spi.h
 * Author: Manav
 *
 * Created on February 22, 2025, 12:50 PM
 */

#ifndef SPI_H
#define SPI_H

#include <xc.h>

#include "stdint.h"

void SPI_Init(void);

uint8_t SPI_Transfer(uint8_t data);

void SPI_Select(void);
        
void SPI_Deselect(void);

#endif

