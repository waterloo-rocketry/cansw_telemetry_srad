#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#define TRANSMIT_TIME 100
#define TRANSMIT_TIME2 100
#define RECEIVE_TIME 10

#define BOARD_MODE_ROCKET 0
#define BOARD_MODE_GROUND 1
#define BOARD_MODE (BOARD_INST_UNIQUE_ID == BOARD_INST_ID_ROCKET ? BOARD_MODE_ROCKET : BOARD_MODE_GROUND)

typedef enum {
    LTT_State_TX,
    LTT_State_RX,
} LTT_State;

uint8_t SM_CC1200_State_Transition(void);
void SM_LTT_State_Machine(void);

#endif
