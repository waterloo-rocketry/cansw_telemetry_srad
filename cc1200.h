/*
 * File:   cc1200.h
 * Author: Manav
 *
 * Created on February 26, 2025, 10:31 AM
 */


#ifndef CC1200_H
#define CC1200_H

#include <xc.h>
#include "stdint.h"
#include "spi.h"

void Write_CC1200(uint8_t, uint8_t);

uint8_t Read_CC1200(uint8_t);

#endif
