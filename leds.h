/*
 * File:   LEDs.h
 * Author: Manav
 *
 * Created on February 22, 2025, 12:22 PM
 */

#ifndef LEDS_H
#define LEDS_H

#include <stdbool.h>
#include <xc.h>

void LED_Init(void);

void LED_set_Green(bool);

void LED_set_Blue(bool);

void LED_set_Red(bool);

#endif
