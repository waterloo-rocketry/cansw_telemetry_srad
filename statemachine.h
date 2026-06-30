#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "ltt_can.h"

#define BOARD_MODE_ROCKET (BOARD_INST_UNIQUE_ID == BOARD_INST_ID_ROCKET ? 1 : 0)

typedef enum {
    LTT_STATE_INIT,
    LTT_STATE_TX,
    LTT_STATE_TX_END,
    LTT_STATE_RX,
} LTT_State;

void SM_Init(void);
void SM_LTT_State_Machine(void);

#endif
