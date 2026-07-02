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

#define ROLLING_AVG_ALPHA 0.9

typedef struct {
    uint8_t rssi;
    uint8_t lqi;
} telem_channel_info;

static telem_channel_info channel_info[8]; // assume we'll never have more than 8 nodes
static telem_channel_info current_channel;
static uint16_t current_sample_count;

// channel ordered by index in array
void channel_info_get(uint8_t channel_index, uint8_t *rssi, uint8_t *lqi) {
    *rssi = channel_info[channel_index].rssi;
    *lqi  = channel_info[channel_index].lqi;
}

void channel_info_start(void) {
    current_sample_count = current_channel.rssi = current_channel.lqi = 0;
}

void channel_info_add(uint8_t rssi, uint8_t lqi) {
    current_channel.rssi += rssi;
    current_channel.lqi += lqi;
    current_sample_count++;
}

void channel_info_end(uint8_t channel_index) {
    channel_info[channel_index].rssi =
        (uint8_t) (ROLLING_AVG_ALPHA * channel_info[channel_index].rssi) +
        (uint8_t) ((1-ROLLING_AVG_ALPHA) * current_channel.rssi / current_sample_count);
    channel_info[channel_index].lqi =
        (uint8_t) (ROLLING_AVG_ALPHA * channel_info[channel_index].lqi) +
        (uint8_t) ((1-ROLLING_AVG_ALPHA) * current_channel.lqi / current_sample_count);
}
