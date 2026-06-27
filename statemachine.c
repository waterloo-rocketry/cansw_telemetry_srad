#include "statemachine.h"
#include "cc1200.h"
#include "ltt_can.h"

#include "canlib/can.h"
#include "canlib/pic18f26k83/pic18f26k83_can.h"

static LTT_State ltt_state;

void SM_Init(void) {
    ltt_state = LTT_State_RX;
}

uint8_t SM_CC1200_State_Transition(LTT_State ltt_state) {
    uint8_t state = (Command_CC1200(COMMAND_SNOP) >> 4) & 0x7;

    switch (state) {
        case STATE_IDLE:
            switch(ltt_state) {
                case LTT_State_TX:
                    if(!pq_empty(&ltt_can_queue)) {
                        can_msg_t message = {0};
                        pq_pop(&ltt_can_queue, &message);
                        CC1200_Transmit_Packet(&message);
                    }
                    break;

                case LTT_State_RX: {
                    can_msg_t *message = {0};
                    CC1200_Receive_Packet();
                    Command_CC1200(COMMAND_SRX);
                    if(msg.data_len > 0) {
                        pic18f26k83_can_send(&message);
                    }
                    break;
                }
            }
            break;

        case STATE_RX_FIFO_ERROR:
            Command_CC1200(COMMAND_SFRX);
            break;

        case STATE_TX_FIFO_ERROR:
            Command_CC1200(COMMAND_SFTX);
            break;
    }

    return state;
}

void SM_LTT_State_Machine(void) {
    uint8_t cc1200_state = SM_CC1200_State_Transition(ltt_state);
}
