#ifndef __random_h__
#define __random_h__

#include <math.h>
#include <stdlib.h>
#include <time.h>

bool random_ready;

/* Sets a random seed. */
void random_init() {
    if(!random_ready) {
        random_ready = true;
        srand(time(NULL));
    }
}

/* Simple function to return a random double value
 * between 0 and 1. */
double next_random() {
    return (double)rand() / RAND_MAX;
}

/* Implementation of Box-Muller transform to generate
 * a normally distributed random number. */
double normal_random() {
    while(true) {
        double u1 = next_random();
        double u2 = next_random();
        double mag = sqrt(-2 * log(u1));
        double z0 = mag * cos(2l * M_PI * u2) + 2;
        if(z0 >= 0) {
            return z0;
        }
    }
}

#endif
