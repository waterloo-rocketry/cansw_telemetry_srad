#include "ltt_packet.h"
#include <stdio.h>
#include "config.h"
#include "canlib/message_types.h"

enum LTT_PACKET_TYPE {
    MSG,
    ENDING,
};

void ltt_packet_msg_build(ltt_packet* pckt, can_msg_t* msg) {
    pckt->msg = msg;
    pckt->pkt_type = MSG;
}

void ltt_endframe_build(ltt_packet* pckt, Ground_Transceiver_Sel tran_sel) {
    if (BOARD_MODE == BOARD_MODE_GROUND) {
        //send endframe mark
        uint8_t data[8];
        data[0] = ENDFRAME;
        build_debug_raw_msg(PRIO_LOW, millis(), &data, &(pckt->msg));
        pckt->pkt_type = ENDING;

    } else if (BOARD_MODE == BOARD_MODE_ROCKET) {
        //send selected transciever
        uint8_t data[8];
        data[0] = trans_sel;
        build_debug_raw_msg(PRIO_LOW, millis(), &data, &(pckt->msg));
        pckt->pkt_type = ENDING;
    }
}

uint8_t check_ltt_endframe(ltt_packet* pckt) {
    if (BOARD_MODE == BOARD_MODE_GROUND) {
        if (pckt->pkt_type == ENDING) {
            return pckt->msg->data[0];
        }
    } else if (BOARD_MODE == BOARD_MODE_ROCKET) {
        if (pckt->pkt_type == ENDING && pckt->msg->data[0] == ENDFRAME) {
            return ENDFRAME;
        }
    }
}