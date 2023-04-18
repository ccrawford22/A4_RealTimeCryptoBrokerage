/**
 * Cody Crawford
 * RedId: 824167663
 * CS 480 - 1001
 * Assignment 4: Real Time Crypto Brokerage
 */

#ifndef SHARED_DATA_H
#define SHARED_DATA_H

/* Shared constants and data structures among threads */

#define BUFFER_SIZE 16
#define BITCOIN_MAX 5

// Include necessary modules
#include <queue>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#include "cryptoexchange.h"

using namespace std;

/* Common data shared between threads */
typedef struct
{
    /* for implementing the critical region */
    sem_t mutexBuffer;

    /* items in buffer */
    sem_t unconsumed;

    /* space in buffer */
    sem_t availableslotsBitcoin;
    sem_t availableslotsTotal;

    /* Production ender */
    sem_t requestsComplete;

    /* shaerd bufferADT buffer; queue, tree, etc */
    queue<RequestType> buffer;

    /**
     * @brief Array of number of requests of each type that are
     * in the request queue and have not yet been consumed.
     * (inRequestQueue[Bitcoin] and inRequestQueue[Etherium])
     */
    unsigned int inRequestQueue[RequestTypeN];

    /**
     * @brief count for each RequestType produced
     * e.g. produced[Bitcoin]
     */
    unsigned int produced[RequestTypeN];

    /**
     * @brief Array of pointers to consumed arrays for each consumer
     * e.g. consumed[BlockchainX] is an array that is indexed by request type
     * (it is a 2-D array, cionsumed[BlockchainX][Bitcoin] is the number for
     * bitcoin trade requests that were transacted on BlockchainX)
     */
    unsigned int consumed[ConsumerTypeN][RequestTypeN];

} SHARED_DATA;

#endif