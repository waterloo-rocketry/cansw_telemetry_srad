#include "statemachine.h"
#include "cc1200.h"
#include "ltt_can.h"
#include "leds.h"

#include "canlib/can.h"
#include "canlib/pic18f26k83/pic18f26k83_can.h"

#define TX_TIMEOUT_MS 10
#define RX_TIMEOUT_MS 10
#define TX_TIME_MAX_MS 100

typedef struct {
    uint32_t start;
    uint32_t duration;
    uint32_t last;
} StateTimer;

#if BOARD_MODE_ROCKET
static const uint8_t remote_list[] = {
    BOARD_INST_ID_TELEMETRY_GROUND_1,
    BOARD_INST_ID_TELEMETRY_GROUND_2,
};
#else
static const uint8_t remote_list[] = { BOARD_INST_ID_ROCKET };
#endif
static int remote_index;

static LTT_State ltt_state;

static StateTimer tx_timer     = { .duration = TX_TIMEOUT_MS };
static StateTimer rx_timer     = { .duration = RX_TIMEOUT_MS };
static StateTimer tx_max_timer = { .duration = TX_TIME_MAX_MS };

void SM_Init(void) {
    ltt_state = LTT_STATE_INIT;
}

static bool timer_expired(StateTimer *timer, uint32_t now) {
    return (int32_t) (now - timer->duration - timer->last) > 0;
}

static uint8_t CC1200_State_Transition(LTT_State, can_msg_t *tx_msg, can_msg_t *rx_msg, uint8_t *rssi, uint8_t *lqi) {
    uint8_t state = (Command_CC1200(COMMAND_SNOP) >> 4) & 0x7;

    switch (state) {
        case CC1200_STATE_IDLE:
            switch(ltt_state) {
                case LTT_STATE_TX:
                    if(!pq_empty(&ltt_can_queue)) {
                        pq_pop(&ltt_can_queue, tx_msg);
                        CC1200_Transmit_Packet(tx_msg);
                    }
                    break;

                case LTT_STATE_TX_END:
                    CC1200_Transmit_End(remote_list[remote_index]);
                    break;

                case LTT_STATE_RX: {
                    CC1200_Receive_Packet(rx_msg, rssi, lqi);
                    break;
                }
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
    uint8_t rssi = 0, lqi = 0;

    LTT_State next_state = ltt_state;

    bool tx_queue_empty = pq_empty(&ltt_can_queue);
    uint32_t now = millis();

    uint8_t cc1200_state = CC1200_State_Transition(ltt_state, &tx_msg, &rx_msg, &rssi, &lqi);

    switch(ltt_state) {
        case LTT_STATE_INIT:
            next_state = BOARD_MODE_ROCKET ? LTT_STATE_TX : LTT_STATE_RX;
            break;

        case LTT_STATE_TX:
            if(tx_msg.sid != 0) {
                tx_timer.last = now;
                toggle_LED_Green(1);
            }
            if(timer_expired(&tx_timer) || timer_expired(&tx_max_timer)) {
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
                    case MSG_TELEMETRY_STATE_SWITCH:
                        uint8_t channel_id = get_message_metadata(&rx_msg);
                        if(channel_id == BOARD_INST_UNIQUE_ID) {
                            next_state = LTT_STATE_TX;
                        }
                        break;
                    default:
                        pic18f26k83_can_send(&rcvd_msg);
                        break;
                }
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
                remote_index = (remote_index + 1) % (sizeof(remote_list) / sizeof(remote_list[0]));
                break;
            case LTT_STATE_RX:
                toggle_LED_Red(0);
                break;
        }
        switch(next_state) {
            case LTT_STATE_TX:
                tx_timer.last = now;
                tx_max_timer.last = now;
                break;

            case LTT_STATE_RX:
                rx_timer.last = now;
                break;
        }
        ltt_state = next_state;
    }
}
