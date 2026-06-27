/* 
 * File:   status_tracker.h
 * Author: Pranav Mahabal
 *
 * Created on June 27, 2026, 12:06 PM
 */

#ifndef STATUS_TRACKER_H
#define	STATUS_TRACKER_H

#ifdef	__cplusplus
extern "C" {
#endif

#define ROLLING_AVG_ALPHA 0.9

    typedef struct {
        uint8_t channel_rssi_avg;
        uint8_t channel_lqi_avg;
    }telem_channel_info;

    uint8_t get_telemetry_channel_lqi(uint8_t channel_id);
    uint8_t get_telemetry_channel_rssi(uint8_t channel_id);
    void add_new_telemetry_channel_info(uint8_t channel_id, uint8_t rssi, uint8_t lqi);

#ifdef	__cplusplus
}
#endif

#endif	/* STATUS_TRACKER_H */

