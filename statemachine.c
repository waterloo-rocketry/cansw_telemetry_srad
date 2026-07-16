#include "statemachine.h"
#include "cc1200.h"
#include "ltt_can.h"
#include "leds.h"
#include "channels.h"
#include "channel_info.h"
#include "eeprom.h"

#include "canlib.h"
#include "timer.h"

/*
 * LTT communication is managed by two finite state machines. The top FSM in
 * SM_LTT_State_Machine manages when to transmit and when to receive. And a low
 * level FSM partially in CC1200_State_Transition, partially inside CC1200's
 * own firmware, manages the TX and RX FIFO of CC1200.
 *
 * For the top level FSM, in TX state, if there is no CAN message in queue for
 * TX_TIMEOUT_MS, or the state has been in TX for more than TX_TIME_MAX_MS, it
 * transitions into TX_END. TX_END state sends an end-of-transmission (EOT) frame and
 * waits until CC1200 goes into IDLE mode, then the FSM transitions into RX.
 *
 * In RX state, the FSM waits for an EOT frame containing the instance ID of
 * the current instance, then goes back to TX state. Additionally, if the
 * current instance ID is ROCKET, the FSM also transitions to TX when there is
 * no message received for RX_TIMEOUT_MS. This is to make sure it continues to
 * transmit telemetry even if the receivers may be too weak.
 *
 * The ROCKET instance distributes time slot between all other instances with
 * this EOT frame in a round-robin fashion. Since each instance only enters TX
 * state when a matching EOT frame is received, this ensures only one of them is
 * transmitting at the same time. For all non-ROCKET instances, the EOT should
 * only contain the instance id ROCKET.
 *
 * For the low level FSM, the CC1200 is configured to transition from RX->RX
 * when a packet is received, and from TX->IDLE when a packet is transmitted.
 * TX FIFO writes can happens in both IDLE or RX state, then STX command is
 * sent to go into TX state. In RX state, IDLE->RX transition is done using SRX
 * command. During RX TMR3 is incremented with CC1200's PKT_CRC_OK to keep
 * track of number of packets in the RX FIFO.
 *
 * The low level FSM additional clears the FIFOs when they over/under flows.
 */

#define TX_TIMEOUT_MS 50
#define RX_TIMEOUT_MS 60
#define TX_TIME_MAX_MS 200

typedef struct {
    uint32_t start;
    uint32_t duration;
    uint32_t last;
} StateTimer;

static LTT_State ltt_state;

static StateTimer tx_timer     = { .duration = TX_TIMEOUT_MS };
static StateTimer rx_timer     = { .duration = RX_TIMEOUT_MS };
static StateTimer tx_max_timer = { .duration = TX_TIME_MAX_MS };

static bool stop_tx;
static bool reload_config;

// index of the remote that is transmitting / should transmit next based on channels.h
static uint8_t remote_index;
static can_msg_t remote_on_msg;

void SM_Init(void) {
    remote_index = 0;
    stop_tx = false;
    ltt_state = LTT_STATE_INIT;
}

static bool timer_expired(StateTimer *timer, uint32_t now) {
    return now - timer->last > timer->duration;
}

static uint8_t CC1200_State_Transition(LTT_State ltt_state, can_msg_t *tx_msg, can_msg_t *rx_msg) {
    uint8_t state = (CC1200_Command(COMMAND_SNOP) >> 4) & 0x7;

    switch (state) {
        case CC1200_STATE_RX:
            CC1200_Receive_Packet(rx_msg);
            // fallthrough
        case CC1200_STATE_IDLE:
            switch(ltt_state) {
                case LTT_STATE_TX:
                    if(!rcvb_is_empty()) {
                        rcvb_pop_message(tx_msg);
                        CC1200_Transmit_Packet(tx_msg);
                    }
                    break;

                case LTT_STATE_TX_END:
                    CC1200_Transmit_End(channel_remote_from_index(remote_index));
                    break;

                case LTT_STATE_RX:
                    if(state != CC1200_STATE_RX) {
                        CC1200_Receive_Start();
                    }
                    break;

                default:
                    break;
            }
            break;

        case CC1200_STATE_RX_FIFO_ERROR:
            CC1200_Command(COMMAND_SFRX);
            break;

        case CC1200_STATE_TX_FIFO_ERROR:
            CC1200_Command(COMMAND_SFTX);
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
            if(cc1200_state == CC1200_STATE_IDLE) {
                next_state = channel_is_rocket() ? LTT_STATE_TX : LTT_STATE_RX;
            }
            break;

        case LTT_STATE_TX:
            if(tx_msg.sid != 0) {
                tx_timer.last = now;
                LED_set_Green(1);
            }
            if(timer_expired(&tx_timer, now) || timer_expired(&tx_max_timer, now)) {
                next_state = LTT_STATE_TX_END;
            }
            break;

        case LTT_STATE_TX_END:
            next_state = reload_config ? LTT_STATE_INIT : LTT_STATE_RX;
            break;

        case LTT_STATE_RX:
            if(rx_msg.sid != 0) {
                switch(get_message_type(&rx_msg)) {
                    // end frame gets packaged into can message in CC1200_Receive_Packet
                    case MSG_TELEMETRY_STATE_SWITCH: {
                        uint8_t channel_id = get_message_metadata(&rx_msg);
                        if(!stop_tx && channel_id == BOARD_INST_UNIQUE_ID) {
                            next_state = LTT_STATE_TX;
                        }
                        break;
                    }
                    default:
                        txb_enqueue(&rx_msg);
                        LED_set_Red(1);
                        break;
                }
                rx_timer.last = now;
            }
            if(timer_expired(&rx_timer, now)) {
                if(channel_is_rocket()) {
                    if(!stop_tx) {
                        next_state = LTT_STATE_TX;
                    }
                } else if(remote_on_msg.sid != 0) {
                    // special case: sends out telemetry on command when we haven't
                    // heard from rocket for a while
                    CC1200_Transmit_Packet(&remote_on_msg);
                    remote_on_msg.sid = 0;
                }
            }
            if(reload_config) {
                next_state = LTT_STATE_INIT;
            }
            break;
    }

    if(next_state != ltt_state) {
        switch(ltt_state) {
            case LTT_STATE_INIT: {
                reload_config = false;

                uint32_t freq = eeprom_get_frequency();
                int8_t power = eeprom_get_power();

                CC1200_Set_Frequency(freq);
                CC1200_Set_Power(power);
                CC1200_Set_Ant_Diversity(!channel_is_rocket());

                can_msg_t msg;
                build_config_status_msg(PRIO_MEDIUM, (uint16_t) now, CAN_CONFIG_ID_FREQ, (uint16_t) (freq-900000), &msg);
                CAN_enqueue(&msg);

                build_config_status_msg(PRIO_MEDIUM, (uint16_t) now, CAN_CONFIG_ID_POWER, (uint16_t) (power+100), &msg);
                CAN_enqueue(&msg);

                break;
            }
            case LTT_STATE_TX:
                LED_set_Green(0);
                break;
            case LTT_STATE_RX:
                channel_info_end(remote_index);
                remote_index = (remote_index + 1) % channel_remote_count();
                LED_set_Red(0);
                break;
            default:
                break;
        }
        switch(next_state) {
            case LTT_STATE_INIT:
                CC1200_Command(COMMAND_SIDLE);
                break;
            case LTT_STATE_TX:
                tx_timer.last = now;
                tx_max_timer.last = now;
                break;
            case LTT_STATE_RX:
                channel_info_start();
                rx_timer.last = now;
                remote_on_msg.sid = 0;
                break;
            default:
                break;
        }
        ltt_state = next_state;
    }
}

void SM_LTT_Stop_TX(bool stop) {
    if(stop) CC1200_PA_Off();
    else     CC1200_PA_On();
    stop_tx = stop;
}

// sends telemetry on command to potentially off rocket side
void SM_LTT_Wake_Remote(const can_msg_t *on_command) {
    remote_on_msg = *on_command;
}

void SM_LTT_Reload_Config(void) {
    reload_config = true;
}
