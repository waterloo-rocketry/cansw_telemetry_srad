/*
 * File:   channel_info.c
 * Author: Pranav Mahabal
 *
 * Created on June 27th, 2026
 * 
 * Tracking of LQI and RSSI done here
 */

#include "statemachine.h"
#include "channel_info.h"
#include "channels.h"
#include "timer.h"

#define ROLLING_AVG_ALPHA 0.7
#define CHANNEL_INFO_TIMEOUT 3000

typedef struct {
    int8_t rssi;
    uint8_t lqi;
} telem_channel_info;

static telem_channel_info channel_info[8]; // assume we'll never have more than 8 nodes

static int32_t current_rssi;
static uint32_t current_lqi;

static uint16_t current_sample_count;
static uint32_t last_update;

// channel ordered by index in array
void channel_info_get(uint8_t channel_index, int8_t *rssi, uint8_t *lqi) {
    if(millis() - last_update > CHANNEL_INFO_TIMEOUT) {
        *rssi = -128;
        *lqi = 0;
    } else {
        *rssi = channel_info[channel_index].rssi;
        *lqi  = channel_info[channel_index].lqi;
    }
}

void channel_info_start(void) {
    current_sample_count = current_rssi = current_lqi = 0;
}

void channel_info_add(int8_t rssi, uint8_t lqi) {
    current_rssi += rssi;
    current_lqi += lqi;
    current_sample_count++;
}

void channel_info_end(uint8_t channel_index) {
    if(current_sample_count == 0) return;
    channel_info[channel_index].rssi = (int8_t)
        ((ROLLING_AVG_ALPHA * channel_info[channel_index].rssi) +
        ((1-ROLLING_AVG_ALPHA) * current_rssi / current_sample_count));
    channel_info[channel_index].lqi = (uint8_t)
        ((ROLLING_AVG_ALPHA * channel_info[channel_index].lqi) +
        ((1-ROLLING_AVG_ALPHA) * current_lqi / current_sample_count));
    last_update = millis();
}
