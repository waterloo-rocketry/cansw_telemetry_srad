#ifndef STATETIMER_H
#define STATETIMER_H

typedef struct {
    uint32_t start;
    uint32_t duration;
} state_timer_s;

void state_timer_init(state_timer_s*, uint32_t duration);
void state_timer_reset(state_timer_s*);
bool state_timer_expired(state_timer_s*);

#endif
