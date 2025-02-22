/*
 * File:   LEDs.h
 * Author: Manav
 *
 * Created on February 22, 2025, 12:22 PM
 */

#ifndef LEDS_H
#define LEDS_H

#include <xc.h>

#include "stdbool.h"

void LEDs_Init(void);

void toggle_LED_Green(bool);

void toggle_LED_Blue(bool);

void toggle_LED_Red(bool);

#endif