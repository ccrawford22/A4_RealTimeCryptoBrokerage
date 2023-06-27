## Real-time Crypto Brokerage

This README file provides an overview of the programming assignment for implementing a real-time crypto brokerage system. The system connects 
traders of cryptocurrencies and facilitates transactions through a producer-consumer communication scheme. The assignment involves simulating 
the producer and consumer problem using POSIX unnamed semaphores and POSIX threads.

## Functionality

The program aims to implement a real-time brokerage system with the following specifications:

1. **Thread Creation**: Upon starting, the program creates two producer threads and two consumer threads using pthreads. The producers are
   responsible for accepting and publishing trade requests, while the consumers consume requests from the broker.

3. **Trade Request Publication**: The producer threads accept crypto trade requests and publish them to the broker. The following specifications apply:
   - There are two types of trade request services: one for Bitcoin requests and one for Ethereum requests.
   - Each producer thread should continue accepting and publishing requests until it reaches the production limit specified as a command line argument
     (-r). The default production limit is 100 if not specified.
   - The producers simulate request production by sleeping for a certain amount of time.

4. **Trade Request Consumption**: The consumer threads consume trade requests from the broker and complete the transactions. The following specifications apply:
   - Two types of request transaction services are available, one using Blockchain X and the other using Blockchain Y.
   - The consumers simulate request consumption by sleeping for a certain amount of time.

5. **Bounded Buffer**: The trade request queue serves as a bounded buffer, streaming requests from producers to consumers. The following specifications apply:
   - The broker can hold a maximum of 16 crypto requests in its request queue.
   - When the trade request queue is full, producers must wait for consumers to consume a request before they can publish another request.
   - When the trade request queue is empty, consumers must wait for producers to add a new request to the broker before they can consume another request.

6. **Mutex Lock**: To maintain mutual exclusion, a mutex is used to control access to the trade request queue. Producers and consumers must acquire the mutex
   before adding or removing requests from the queue.

8. **Main Thread**: The main thread waits for the consumer threads to complete consuming the last request before exiting the program. A barrier is used to
   ensure proper synchronization.

10. **Command Line Arguments**: The program accepts optional command line arguments to customize the behavior:
   - `-rN`: Specifies the total number of trade requests (production limit). The default is 100 if not specified.
   - `-xN`: Specifies the number of milliseconds (N) required for consuming a trade request and completing the transaction using Blockchain X.
   - `-yN`: Specifies the number of milliseconds (N) required for consuming a trade request and completing the transaction using Blockchain Y.
   - `-bN`: Specifies the number of milliseconds (N) required to produce and publish a Bitcoin request.
   - `-eN`: Specifies the number of milliseconds (N) required to produce and publish an Ethereum request.

Note: If an argument is not given for any thread type, the default delay for that thread is 0.

## Conclusion

This programming assignment focuses on implementing a real-time crypto brokerage system using POSIX unnamed semaphores and POSIX threads. The program simulates 
the producer-consumer problem, where trade requests are produced by the producers, added to a trade request queue (broker), and consumed by the consumers for 
transaction completion. The program ensures proper synchronization and mutual exclusion using mutex locks and implements a bounded buffer for streaming requests. 
Optional command line arguments can be used to customize the behavior of the system.
