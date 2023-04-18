/**
 * Cody Crawford
 * RedId: 824167663
 * CS 480 - 1001
 * Assignment 4: Real Time Crypto Brokerage
 */

#ifndef PRODUCER_H
#define PRODUCER_H

#include <chrono>
#include <thread>
#include <fstream>

#include "cryptoexchange.h"
#include "shared.h"
#include "log.h"

using namespace std::this_thread;
using namespace std::chrono;

typedef struct
{
    RequestType type; // Bitcoin or Etherium
    milliseconds ms;  // Time it takes to produce a request
    SHARED_DATA *sd;  // Shared data for all threads
} Producer;

// NOTE: only use sem_init, sem_wait, and sem_pos POSIX semaphores

void *produce(void *v)
{
    // ItemType Item;
    Producer *p = (Producer *)v;

    while (true)
    {
        // Producing an item
        sleep_for(p->ms);
        // make sure we have room
        // down
        if (p->type == Bitcoin)
        {
            sem_wait(&(p->sd->availableslotsBitcoin));
        }
        sem_wait(&(p->sd->availableslotsTotal));
        // Access buffer exclusively
        // dowm
        sem_wait(&(p->sd->mutexBuffer));

        /* CRITICAL AREA */
        p->sd->buffer.push(p->type);
        p->sd->produced[p->type]++;
        p->sd->inRequestQueue[p->type]++;
        log_request_added(p->type, p->sd->produced, p->sd->inRequestQueue);
        /** END CRITICAL AREA */

        // up
        sem_post(&(p->sd->mutexBuffer));
        // up
        sem_post(&(p->sd->unconsumed));
        // spin request complete semaphore
        sem_post(&(p->sd->requestsComplete));
    }
}

#endif