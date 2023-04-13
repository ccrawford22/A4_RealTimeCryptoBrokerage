#include "producer.h"

Producer::Producer()
{
}

// void producer()
// {
//     ItemType Item;
//     while (true)
//     {
//         item = new ItemType(); // Producing an item
//         /* make sure we have room */
//         availableSlots.wait(); // down

//         /* Access buffer exclusively */
//         mutex.wait(); // down
//         buffer.insert(item);
//         mutex.signal(); // up

//         unconsumed.signal(); // up, inform consumer }
//     }
// }