/**
 * @file monitor.cpp
 * @author Benjamin Carter and Trevor Pope
 * @brief Monitor Assignment. A monitor holds a queue that is shared between a producer and consumer.
 * @version 0.1
 * @date 2024-02-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include <mutex>
#include <vector>
#include <ctime>
#include <errno.h>
#include <iostream>

/**
 * @brief Delay X milliseconds
 * 
 * @param msec milliseconds to delay
 * @return int 
 */
int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

/**
 * @brief The Monitor Queue.
 * 
 */
class MonitorQueue
{
private:
    #define SIZE  10
    int buffer[SIZE];
    int start;
    int end;
    int length;
    std::mutex mainMutex;

public:
    MonitorQueue()
    {
        start = 0;
        end = 0;
        length = 0;
    }
    void put(int i)
    {
        std::unique_lock<std::mutex> lock(mainMutex);

        if (length <= SIZE) {
            buffer[end] = i;
            end += 1;
            end = end % SIZE;
            length += 1;
            printf("Added %d to queue\n", i);
        }
        else {
            printf("Buffer is full. Producer cannot continue\n");
        }
        // automatically unlocks on deconstruction
    }

    int get() 
    {
        printf("Running get\n");
        std::unique_lock<std::mutex> lock(mainMutex);

        if (length != 0) {
            int i;
            i = buffer[start];
            start += 1;
            start = start % SIZE;
            length -= 1;
            return i;
        }
        else {
            printf("Buffer is empty.\n");
            return 0;
        }
        // automatically unlocks on deconstruction
    }

    int getLength()
    {
        std::unique_lock<std::mutex> lock(mainMutex);
        return length;
        // automatically unlocks on deconstruction
    }
};

bool randomIF(float percent)
{
    if(percent >= 1.0f)
    {
        return true;
    }
    else if(percent <= 0.0f)
    {
        return false;
    }
    double randomVal = (double)std::rand() / (double)RAND_MAX;
    return randomVal <= percent; 
}

// int main()
// {
//     MonitorQueue mq = MonitorQueue();
//     mq.put(10);
//     std::cout << mq.get() << "\n";
// }

int main()
{
    unsigned int current_time = time(NULL);
    srand(current_time);
    // producer. 
    // consumer.

    MonitorQueue mq = MonitorQueue();
    while(true)
    {
        if(randomIF(0.5))
        {
            // PRODUCER
            int number = rand() % 100;
            mq.put(rand() % 100);

        }
        else
        {
            // CONSUMER
            if(mq.getLength() > 0)
            {
                int i =mq.get();
                printf("Consuming %d from queue\n", i);
            }
            else
            {
                printf("Buffer is empty, consumer cannot proceed\n");
            }
        }
        msleep(500);
    }

}
