/**
 * Cody Crawford
 * RedId: 824167663
 * CS 480 - 1001
 * Assignment 4: Real Time Crypto Brokerage
 */

#define BADFLAG 1
#define NORMALEXIT 0

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>

#include <string>
#include <fstream>
#include <iostream>

#include "cryptoexchange.h"
#include "shared.h"
#include "producer.h"
#include "consumer.h"

using namespace std;

// Note: use only sem_init, sem_wait, and sem_pos POSIX semaphores

int main(int argc, char **argv)
{
    // Argument handling
    // set default values
    int r = 100;
    int x = 0;
    int y = 0;
    int b = 0;
    int e = 0;

    // Parse command line arguments using getopt
    int opt;
    while ((opt = getopt(argc, argv, "r:x:y:b:e:")) != -1)
    {
        switch (opt)
        {
        case 'r': /**
                   * @brief case -r N
                   * Total number of trade requests (production limit). Default is 100 if
                   * not specified.
                   */
            r = atoi(optarg);
            break;
        case 'x': /**
                   * @brief case -x N
                   * Specifies the number of milliseconds N that the consumer using Blockchain
                   * X requires for processing a trade request and
                   * completing its transaction. You would simulate this time to consume a request
                   * by putting the consumer thread to sleep for N milliseconds. Other consumer
                   * and producer threads (consuming over Blockchain Y, producing Bitcoin request,
                   * and producing Ethereum request) are handled similarly.
                   */
            x = atoi(optarg);
            break;
        case 'y': /**
                   * @brief case -y N Similar argument for consuming over Blockchain Y.
                   */
            y = atoi(optarg);
            break;
        case 'b': /**
                   * @brief case -b N Specifies the number of milliseconds required to
                   * produce and publish a Bitcoin request.
                   */
            b = atoi(optarg);
            break;
        case 'e': /**
                   * @brief case -e N Specifies the number of milliseconds required to
                   * produce and publish an Ethereum request.
                   */
            e = atoi(optarg);
            break;
        default:
            cerr << "Usage: " << argv[0] << " [-r N] [-x N] [-y N] [-b N] [-e N]" << endl;
            exit(BADFLAG);
        }
    }

    // // Print the values of the arguments
    // cout << "Total number of trade requests: " << r << endl;
    // cout << "Milliseconds for Blockchain X processing: " << x << endl;
    // cout << "Milliseconds for Blockchain Y processing: " << y << endl;
    // cout << "Milliseconds for Bitcoin request production: " << b << endl;
    // cout << "Milliseconds for Ethereum request production: " << e << endl;

    // DEFINE AND INITIALIZE SHARED DATA
    SHARED_DATA sharedData;

    // Initialize semaphores with default values
    //  shared semaphore mutex = 1
    sem_init(&(sharedData.mutexBuffer), 1, 1);
    // shared semaphore unconsumed = 0
    sem_init(&(sharedData.unconsumed), 1, 0);
    // shared semaphore availableSlots = BufferSize
    sem_init(&(sharedData.availableslotsBitcoin), 1, BITCOIN_MAX);
    sem_init(&(sharedData.availableslotsTotal), 1, BUFFER_SIZE);

    sem_init(&(sharedData.requestsComplete), 1, 0);

    // Initialize other sharedData values
    for (unsigned type = 0; type < RequestTypeN; type++)
    {
        sharedData.inRequestQueue[type] = 0;
        sharedData.produced[type] = 0;
        for (unsigned consumer = 0; consumer < ConsumerTypeN; consumer++)
        {
            sharedData.consumed[consumer][type] = 0;
        }
    }

    // THREAD DEFINITIONS AND HANDLING
    // Producers
    pthread_t producerBitcoinThread;  // Bitcoin
    pthread_t producerEtheriumThread; // Ethereum
    // Consumers
    pthread_t consumerXbchainThread; // Blockchain X
    pthread_t consumerYbchainThread; // Blockchain Y

    // Create producers and consumers
    Producer pBitcoin = {Bitcoin, milliseconds(b), &sharedData};
    Producer pEtherium = {Ethereum, milliseconds(e), &sharedData};
    Consumer cBlockchainX = {BlockchainX, milliseconds(x), &sharedData};
    Consumer cBlockchainY = {BlockchainY, milliseconds(y), &sharedData};

    // Start threads
    if (pthread_create(&producerBitcoinThread, NULL,
                       &produce, &pBitcoin))
    {
        // Error handling
        cout << endl
             << "Unable to create producerBitcoinThread." << endl;
        exit(BADFLAG); // BADFLAG is an error # defined in a header
    }
    if (pthread_create(&producerEtheriumThread, NULL,
                       &produce, &pEtherium))
    {
        // Error handling
        cout << endl
             << "Unable to create producerEtheriumThread." << endl;
        exit(BADFLAG); // BADFLAG is an error # defined in a header
    }
    if (pthread_create(&consumerXbchainThread, NULL,
                       &consume, &cBlockchainX))
    {
        // Error handling
        cout << endl
             << "Unable to create consumerXbchainThread." << endl;
        exit(BADFLAG); // BADFLAG is an error # defined in a header
    }
    if (pthread_create(&consumerYbchainThread, NULL,
                       &consume, &cBlockchainY))
    {
        // Error handling
        cout << endl
             << "Unable to create consumerYbchainThread." << endl;
        exit(BADFLAG); // BADFLAG is an error # defined in a header
    }

    // Wait until enough requests have been produced
    for (int i = 0; i < r; i++)
    {
        sem_wait(&(sharedData.requestsComplete));
    }
    // Kill producer threads
    pthread_cancel(producerBitcoinThread);
    pthread_cancel(producerEtheriumThread);

    // Wait until all requests have been consumed from the buffer
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        sem_wait(&(sharedData.availableslotsTotal));
    }

    // Kill consumer threads
    pthread_cancel(consumerXbchainThread);
    pthread_cancel(consumerYbchainThread);

    // Log entire production history
    log_production_history(sharedData.produced, sharedData.consumed);

    // Clean up semaphores
    sem_destroy(&sharedData.mutexBuffer);
    sem_destroy(&sharedData.unconsumed);
    sem_destroy(&sharedData.availableslotsBitcoin);
    sem_destroy(&sharedData.availableslotsTotal);

    return 0;
}