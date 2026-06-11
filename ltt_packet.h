/* 
 * File:   ltt_packet.h
 * Author: Pranav Mahabal
 *
 * Created on June 6, 2026, 6:02 PM
 */

#ifndef LTT_PACKET_H
#define	LTT_PACKET_H
#define ENDFRAME 247;

#include "canlib/can.h"
#include "canlib/message_types.h"
#include "config.h"

#ifdef	__cplusplus
extern "C" {
#endif


    typedef struct {
        can_msg_t msg;
        uint8_t pkt_type;
    } ltt_packet;

    void ltt_packet_build(const ltt_packet* pckt, can_msg_t* msg, uint8_t packet_type);
    void ltt_endframe_build(ltt_packet* pckt, enum Ground_Transceiver_Sel tran_sel);
    uint8_t check_ltt_endframe(ltt_packet* pckt);
    


#ifdef	__cplusplus
}
#endif

#endif	/* LTT_PACKET_H */

