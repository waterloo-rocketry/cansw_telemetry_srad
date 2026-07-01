#include "channels.h"
#include "message_types.h"

bool channel_is_rocket(void) {
    return BOARD_INST_UNIQUE_ID == BOARD_INST_ID_ROCKET;
}

uint8_t channel_remote_from_index(int i) {
    if(channel_is_rocket()) {
        switch(i) {
            case 0:
            default:
                return BOARD_INST_ID_TELEMETRY_GROUND_1;
            case 1:
                return BOARD_INST_ID_TELEMETRY_GROUND_2;
        }
    } else {
        return BOARD_INST_ID_ROCKET;
    }
}

int channel_remote_count(void) {
    if(channel_is_rocket()) {
        return 2;
    } else {
        return 1;
    }
}
