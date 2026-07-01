/*
 * Describes the list of remote board IDs that can talk to this board
 * BOARD_MODE_ROCKET additional determines the master node
 */
#ifndef CHANNELS_H
#define CHANNELS_H

#include "message_types.h"

#define BOARD_MODE_ROCKET (BOARD_INST_UNIQUE_ID == BOARD_INST_ID_ROCKET ? 1 : 0)

#if BOARD_MODE_ROCKET
#define CHANNEL_REMOTE_LEN 2
static const uint8_t channel_remote_list[CHANNEL_REMOTE_LEN] = {
    BOARD_INST_ID_TELEMETRY_GROUND_1,
    BOARD_INST_ID_TELEMETRY_GROUND_2,
};
#else
#define CHANNEL_REMOTE_LEN 1
static const uint8_t channel_remote_list[CHANNEL_REMOTE_LEN] = {
    BOARD_INST_ID_ROCKET,
};
#endif

#endif
