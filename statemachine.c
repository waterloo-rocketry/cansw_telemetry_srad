#include "statemachine.c"
#include "cc1200.h"
#include "priority_queue.h"

static PriorityQueue tx_queue;

void SM_Init(void) {
    pq_init(&tx_queue);
}

uint8_t SM_CC1200_State_Transition(LTT_State ltt_state) {
    uint8_t state = (Command_CC1200(COMMAND_SNOP) >> 4) & 0x7;

    switch (state) {
        case STATE_IDLE:
            switch(LTT_State) {
                case LTT_State_TX:
                    if(!pq_empty(&tx_queue)) {
                        can_msg_t message = {0};
                        pq_pop(&tx_queue, &message);
                        CC1200_Transmit_Packet(&message);
                    }
                    break;

                case LTT_State_RX:
                    CC1200_Receive_Packet();
                    Command_CC1200(COMMAND_SRX);
                    break;
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
    uint8_t cc1200_state = CC1200_State_Transition();
}
