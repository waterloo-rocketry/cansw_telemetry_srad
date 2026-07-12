#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "ltt_can.h"
#include <stdbool.h>

typedef enum {
    LTT_STATE_INIT,
    LTT_STATE_TX,
    LTT_STATE_TX_END,
    LTT_STATE_RX,
} LTT_State;

void SM_Init(void);
void SM_LTT_State_Machine(void);
void SM_LTT_Stop_TX(bool);
void SM_LTT_Wake_Remote(const can_msg_t*);

#endif
