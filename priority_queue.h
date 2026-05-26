/* 
 * File:   priority_queue.h
 * Author: Pranav Mahabal
 *
 * Created on May 25, 2026, 3:15 PM
 */

#ifndef PRIORITY_QUEUE_H
#define	PRIORITY_QUEUE_H
#define QUEUE_SIZE 32
#include "canlib/can.h"

#ifdef	__cplusplus
extern "C" {
#endif
    typedef struct
    {
        can_msg_t rx_msgs[QUEUE_SIZE];
        uint8_t size;
    }CAN_PriorityQueue;
    
    // Initialize queue
    void pq_init(CAN_PriorityQueue *q);

    // Status checks
    uint8_t pq_empty(CAN_PriorityQueue *q);
    uint8_t pq_full(CAN_PriorityQueue *q);

    // Queue operations
    uint8_t pq_push(CAN_PriorityQueue *q, can_msg_t *msg);
    uint8_t pq_pop(CAN_PriorityQueue *q, can_msg_t *out);
    uint8_t pq_peek(CAN_PriorityQueue *q, can_msg_t *out);
    
#ifdef	__cplusplus
}
#endif

#endif	/* PRIORITY_QUEUE_H */

