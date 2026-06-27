/*
 * File:   status_tracker.c
 * Author: Pranav Mahabal
 *
 * Created on June 27th, 2026
 * 
 * Tracking of LQQI and RSSI done here
 */

#include "statemachine.h"
#include "status_tracker.h"
#include "canlib/message_types.h"

uint8_t channels;
#if BOARD_MODE == BOARD_MODE_ROCKET
    channels=BOARD_INST_ID_TELEMETRY_ENUM_MAX-BOARD_INST_ID_TELEMETRY_GROUND_1;
#elif BOARD_MODE == BOARD_MODE_GROUND
    channels=1;
#endif

telem_channel_info channel_info[channels]; //only rocket to track

int8_t channel_to_index(uint8_t channel_id)
{
    return (int8_t)(channel_id)-INST_ID_TELEMETRY_GROUND_1;
}

//use average approximation for rolling average
//avg += (sample - avg) / num samples;
//rio suggested a*old + (1-a)*new;
//channel ordered by index in array
uint8_t get_telemetry_channel_lqi(uint8_t channel_id){
    int8_t index=channel_to_index(channel_id);
    if(channels==1 && channel_id == BOARD_INST_ID_ROCKET){
        return channel_info[0].channel_lqi_avg;
    }
    else if(index >=0 && index<INST_ID_TELEMETRY_ENUM_MAX)
    {
        return channel_info[index].channel_lqi_avg;
    }
    //return some out of channel error
    return;
}
uint8_t get_telemetry_channel_rssi(uint8_t channel_id){
    int8_t index=channel_to_index(channel_id);
    if(channels==1 && channel_id == BOARD_INST_ID_ROCKET){
        return channel_info[0].channel_rssi_avg;
    }
    else if(index >=0 && index<INST_ID_TELEMETRY_ENUM_MAX)
    {
        return channel_info[index].channel_rssi_avg;
    }
    //return some out of channel error
    return 0;
}

void add_new_telemetry_channel_info(uint8_t channel_id, uint8_t rssi, uint8_t lqi)
{
    int8_t index;

    if (channels == 1 && channel_id == BOARD_INST_ID_ROCKET) {
        index = 0;
    } else {
        index = (int8_t)channel_id - INST_ID_TELEMETRY_GROUND_1;

        if (index<0 || index >= channels) {
            // Invalid channel
            return 0;
        }
    }

    if (channel_info[index].channel_rssi_avg == 0) {
        channel_info[index].channel_rssi_avg = rssi;
    } else {
        channel_info[index].channel_rssi_avg =
            channel_info[index].channel_rssi_avg * ROLLING_AVG_ALPHA +
            (1 - ROLLING_AVG_ALPHA) * rssi;
    }

    if (channel_info[index].channel_lqi_avg == 0) {
        channel_info[index].channel_lqi_avg = lqi;
    } else {
        channel_info[index].channel_lqi_avg =
            channel_info[index].channel_lqi_avg * ROLLING_AVG_ALPHA +
            (1 - ROLLING_AVG_ALPHA) * lqi;
    }
}

