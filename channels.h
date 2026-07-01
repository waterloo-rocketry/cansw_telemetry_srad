/*
 * Describes the list of remote board IDs that can talk to this board
 * channel_is_rocket additional determines the master node
 */
#ifndef CHANNELS_H
#define CHANNELS_H

#include <stdbool.h>
#include <stdint.h>

bool channel_is_rocket(void);
uint8_t channel_remote_from_index(int);
int channel_remote_count(void);

#endif
