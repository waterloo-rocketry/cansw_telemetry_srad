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
        can_msg_t msgs[QUEUE_SIZE];
        uint8_t size;
    }PriorityQueue;
    
    // Initialize queue
    void pq_init(PriorityQueue *q);

    // Status checks
    uint8_t pq_empty(PriorityQueue *q);
    uint8_t pq_full(PriorityQueue *q);

    // Queue operations
    uint8_t pq_push(PriorityQueue *q, can_msg_t *msg);
    uint8_t pq_pop(PriorityQueue *q, can_msg_t *out);
    uint8_t pq_peek(PriorityQueue *q, can_msg_t *out);
    
#ifdef	__cplusplus
}
#endif

#endif	/* PRIORITY_QUEUE_H */

