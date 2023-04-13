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

#include <queue>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> //TODO: Do we need this?

#include "cryptoexchange.h"

using namespace std;

// Note: use only sem_init, sem_wait, and sem_pos POSIX semaphores

/* For implementing the critical region */
sem_t mutexQueue;

/* Items in buffer */
sem_t unconsumed;

/* Space in buffer */
sem_t shared;

// TODO: We need a semaphore for precedence constraint?
/* number of trade requests for Bitcoin */
/* Set initial value to max allowed for Bitcoin = 5 */
sem_t numTradeRequestsOfBitcoin;

/* Number of trade requests for Ethereum */
/* Set initial value to max allowed for Ethereum = 16 */
sem_t numTradeRequestsOfEthereum;

/* Max capacity of queue */
sem_t maxCapacity;

/* Shared BufferADT buffer: queue, tree, etc */
queue<int> buffer;

int main(int argc, char **argv)
{
    // TODO: implement argument handling based on previous assignments
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

    // Print the values of the arguments
    cout << "Total number of trade requests: " << r << endl;
    cout << "Milliseconds for Blockchain X processing: " << x << endl;
    cout << "Milliseconds for Blockchain Y processing: " << y << endl;
    cout << "Milliseconds for Bitcoin request production: " << b << endl;
    cout << "Milliseconds for Ethereum request production: " << e << endl;

    // TODO: implement thread and other element creations here
    // sem_init(&mutexQueue, 1, 1);

    return 0;
}