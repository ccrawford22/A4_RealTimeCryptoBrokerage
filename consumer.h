/**
 * Cody Crawford
 * RedId: 824167663
 * CS 480 - 1001
 * Assignment 4: Real Time Crypto Brokerage
 */

#ifndef CONSUMER_H
#define CONSUMER_H

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
    ConsumerType type; // Blockchain X or Y
    milliseconds ms;   // Time it takes to execute (consume) a request
    SHARED_DATA *sd;   // Shared data for all threads
} Consumer;

// NOTE: only use sem_init, sem_wait, and sem_pos POSIX semaphores

void *consume(void *v)
{
    // ItemType Item
    Consumer *c = (Consumer *)v;
    RequestType requestType;

    while (true)
    {
        // Block until something to consume
        // down
        sem_wait(&(c->sd->unconsumed));
        // Access buffer exclusively
        // down
        sem_wait(&(c->sd->mutexBuffer));

        /* CRITICAL AREA */
        requestType = c->sd->buffer.front();
        c->sd->buffer.pop();
        c->sd->consumed[c->type][requestType]++;
        c->sd->inRequestQueue[requestType]--;
        log_request_removed(c->type, requestType, c->sd->consumed[c->type], c->sd->inRequestQueue);
        /* END CRITICAL AREA */

        // up
        sem_post(&(c->sd->mutexBuffer));
        // up
        sem_post(&(c->sd->availableslotsTotal));
        if (requestType == Bitcoin)
        {
            sem_post(&(c->sd->availableslotsBitcoin));
        }
        // consume or use item
        sleep_for(c->ms);
    }
}

#endif