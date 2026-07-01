#include "statemachine.h"
#include "cc1200.h"
#include "ltt_can.h"
#include "leds.h"
#include "channels.h"
#include "channel_info.h"

#include "canlib.h"
#include "timer.h"

#define TX_TIMEOUT_MS 10
#define RX_TIMEOUT_MS 10
#define TX_TIME_MAX_MS 100

typedef struct {
    uint32_t start;
    uint32_t duration;
    uint32_t last;
} StateTimer;

static LTT_State ltt_state;

static StateTimer tx_timer     = { .duration = TX_TIMEOUT_MS };
static StateTimer rx_timer     = { .duration = RX_TIMEOUT_MS };
static StateTimer tx_max_timer = { .duration = TX_TIME_MAX_MS };

// index of the remote that is transmitting / should transmit next based on channels.h
static uint8_t remote_index;

void SM_Init(void) {
    ltt_state = LTT_STATE_INIT;
}

static bool timer_expired(StateTimer *timer, uint32_t now) {
    return (int32_t) (now - timer->duration - timer->last) > 0;
}

static uint8_t CC1200_State_Transition(LTT_State ltt_state, can_msg_t *tx_msg, can_msg_t *rx_msg) {
    uint8_t state = (Command_CC1200(COMMAND_SNOP) >> 4) & 0x7;

    switch (state) {
        case CC1200_STATE_IDLE:
            switch(ltt_state) {
                case LTT_STATE_TX:
                    if(!rcvb_is_empty()) {
                        rcvb_pop_message(tx_msg);
                        CC1200_Transmit_Packet(tx_msg);
                    }
                    break;

                case LTT_STATE_TX_END:
                    CC1200_Transmit_End(channel_remote_list[remote_index]);
                    break;

                case LTT_STATE_RX: {
                    CC1200_Receive_Packet(rx_msg);
                    break;
                }

                default:
                    break;
            }
            break;

        case CC1200_STATE_RX_FIFO_ERROR:
            Command_CC1200(COMMAND_SFRX);
            break;

        case CC1200_STATE_TX_FIFO_ERROR:
            Command_CC1200(COMMAND_SFTX);
            break;
    }

    return state;
}

void SM_LTT_State_Machine(void) {
    can_msg_t tx_msg = { 0 };
    can_msg_t rx_msg = { 0 };

    LTT_State next_state = ltt_state;

    uint32_t now = millis();
    uint8_t cc1200_state = CC1200_State_Transition(ltt_state, &tx_msg, &rx_msg);

    switch(ltt_state) {
        case LTT_STATE_INIT:
            next_state = BOARD_MODE_ROCKET ? LTT_STATE_TX : LTT_STATE_RX;
            break;

        case LTT_STATE_TX:
            if(tx_msg.sid != 0) {
                tx_timer.last = now;
                toggle_LED_Green(1);
            }
            if(timer_expired(&tx_timer, now) || timer_expired(&tx_max_timer, now)) {
                next_state = LTT_STATE_TX_END;
            }
            break;

        case LTT_STATE_TX_END:
            if(cc1200_state == CC1200_STATE_IDLE) {
                next_state = LTT_STATE_RX;
            }
            break;

        case LTT_STATE_RX:
            if(rx_msg.sid != 0) {
                switch(get_message_type(&rx_msg)) {
                    // end frame gets packaged into can message in CC1200_Receive_Packet
                    case MSG_TELEMETRY_STATE_SWITCH: {
                        uint8_t channel_id = get_message_metadata(&rx_msg);
                        if(channel_id == BOARD_INST_UNIQUE_ID) {
                            next_state = LTT_STATE_TX;
                        }
                        break;
                    }
                    default:
                        pic18f26k83_can_send(&rx_msg);
                        break;
                }
                rx_timer.last = now;
                toggle_LED_Red(1);
            }
#if BOARD_MODE_ROCKET
            if(timer_expired(&rx_timer, now)) {
                next_state = LTT_STATE_TX;
            }
#endif
            break;
    }

    if(next_state != ltt_state) {
        switch(ltt_state) {
            case LTT_STATE_INIT:
                remote_index = 0;
                break;
            case LTT_STATE_TX:
                toggle_LED_Green(0);
                break;
            case LTT_STATE_TX_END:
                remote_index = (remote_index + 1) % CHANNEL_REMOTE_LEN;
                break;
            case LTT_STATE_RX:
                channel_info_end(remote_index);
                toggle_LED_Red(0);
                break;
            default:
                break;
        }
        switch(next_state) {
            case LTT_STATE_TX:
                tx_timer.last = now;
                tx_max_timer.last = now;
                break;
            case LTT_STATE_RX:
                channel_info_start();
                rx_timer.last = now;
                break;
            default:
                break;
        }
        ltt_state = next_state;
    }
}
