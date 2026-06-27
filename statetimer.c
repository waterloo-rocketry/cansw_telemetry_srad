#include "statetimer.h"
#include "timer.h"

void state_timer_init(state_timer_s *state_timer, uint32_t duration) {
    state_timer->duration = duration;
}

void state_timer_reset(state_timer_s *state_timer) {
    state_timer->start = millis();
}

bool state_timer_expired(state_timer_s *state_timer) {
    return (int32_t) (millis() - state_timer->duration - state_timer->start) > 0;
}
