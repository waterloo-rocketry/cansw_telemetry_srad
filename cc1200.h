/*
 * File:   cc1200.h
 * Author: Manav
 *
 * Created on February 26, 2025, 10:31 AM
 */

#ifndef CC1200_H
#define CC1200_H

#include "spi.h"
#include "stdint.h"
#include <xc.h>

typedef struct {
    uint8_t addr;
    uint8_t value;
} registerSetting_t;

typedef struct {
    uint8_t status;
    uint8_t value;
} CC1200ReadResult;

CC1200ReadResult Write_CC1200(uint8_t, uint8_t);

CC1200ReadResult Read_CC1200(uint8_t);

void CC1200_Init(void);

#endif
