/* 
 * File:   channel_info.h
 * Author: Pranav Mahabal
 *
 * Created on June 27, 2026, 12:06 PM
 */

#ifndef CHANNEL_INFO_H
#define	CHANNEL_INFO_H

void channel_info_get(uint8_t channel_index, int8_t *rssi, uint8_t *lqi);
void channel_info_start(void);
void channel_info_add(int8_t rssi, uint8_t lqi);
bool channel_info_started(void);
void channel_info_end(uint8_t channel_index);

#endif	/* STATUS_TRACKER_H */
