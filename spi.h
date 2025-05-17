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

uint8_t Read_SPI(void);

void Write_SPI(uint8_t);

#endif

