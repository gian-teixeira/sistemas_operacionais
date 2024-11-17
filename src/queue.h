#ifndef __queue_h__
#define __queue_h__

#include <stddef.h>
#include <stdbool.h>
#include "random.h"

#define QUEUE_SIZE (size_t)100

double queue[QUEUE_SIZE];
size_t queue_head;

/* Initializes the queue with random and normally distributed
 * double values. */
void queue_init() {
    random_init();
    for(size_t i = 0; i < QUEUE_SIZE; ++i) {
        queue[i] = normal_random();
    }
}

/* Gets the next number on the queue. */
bool queue_pop(double *out) {
    if(queue_head == QUEUE_SIZE) {
        return false;
    }
    *out = queue[queue_head++];
    return true;
}

#endif
