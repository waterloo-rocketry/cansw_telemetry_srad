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
        pckt->msg->data[0]=ENDFRAME;
        pckt->msg->data_len=1;
        pckt->msg->sid
        pckt->pkt_type=ENDING;
        
    } else if (BOARD_MODE == BOARD_MODE_ROCKET) {
        //send end frame + selected transciever
        pckt->msg->data[0]=ENDFRAME;
        pckt->msg->data[1]=tran_sel;
        pckt->msg->data_len=2;
        //pckt->msg->sid
        pckt->pkt_type=ENDING;

    }


}