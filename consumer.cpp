#include "consumer.h"

Consumer::Consumer()
{
}

// void consumer()
// {
//     ItemType item;
//     while (true)
//     {
//         // Block until something to consume
//         unconsumed.wait(); // down

//         // Access buffer exclusively
//         mutex.wait(); // down
//         Item = buffer.remove();
//         mutex.signal(); // up

//         AvailableSlots.signal(); // up
//         consume(item);           // consume or use Item }
//     }
// }