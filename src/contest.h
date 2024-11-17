#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdio.h>
#include "random.h"
#include "queue.h"

#define NWORKERS 5
#define WORK_TIME_MULTIPLIER 100

pthread_t worker[NWORKERS];
size_t work_ammount[NWORKERS];
pthread_mutex_t start_mutex, queue_mutex;
pthread_cond_t start_condition;
bool start;

int contest_compare(const void *va, const void *vb) {
    const size_t a = *(size_t*)va;
    const size_t b = *(size_t*)vb;
    return work_ammount[a] < work_ammount[b] 
        || work_ammount[a] == work_ammount[b] && a > b;
}

void *contest_work(void *vid) {
    size_t id = (size_t)vid;
    size_t done = 0;
    bool to_continue;
    double item;
    
    pthread_mutex_lock(&start_mutex);
    while(!start) pthread_cond_wait(&start_condition, &start_mutex);
    pthread_mutex_unlock(&start_mutex);
    
    while(true) {
        pthread_mutex_lock(&queue_mutex);
        to_continue = queue_pop(&item);
        pthread_mutex_unlock(&queue_mutex);
        if(!to_continue) {
            break;
        }
        usleep(item * WORK_TIME_MULTIPLIER);
        ++done;
    }

    work_ammount[id] = done;
}

void contest_prepare() {
    queue_init();
    pthread_mutex_init(&start_mutex, NULL);
    pthread_mutex_init(&queue_mutex, NULL);
    pthread_cond_init(&start_condition, NULL);
    for(size_t i = 0; i < NWORKERS; ++i) {
        pthread_create(&worker[i], NULL, contest_work, (void*)i);
    }
}

void contest_start() {
    pthread_mutex_lock(&start_mutex);
    start = true;
    pthread_cond_broadcast(&start_condition);
    pthread_mutex_unlock(&start_mutex);
}

void contest_finish() {
    size_t classification[NWORKERS];
    for(size_t i = 0; i < NWORKERS; ++i) {
        pthread_join(worker[i], NULL);
        classification[i] = i;
    }
    qsort(classification, NWORKERS, sizeof(size_t), contest_compare);
    for(size_t i = 0; i < NWORKERS; ++i) {
        size_t contestant = classification[i];
        printf("%ld : %ld\n", contestant, work_ammount[contestant]);
    }
}
