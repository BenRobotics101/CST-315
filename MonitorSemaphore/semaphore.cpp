/**
 * @file semaphore.cpp
 * @author Benjamin Carter and Trevor Pope
 * @brief Monitor/Semaphore Assignment. This program uses semaphores to do a producer/consumer program. It produces
 * random numbers placing them in a queue and consumes them and uses semaphores to have the producer or consumer
 * wait until there is objects in the queue/wait until the queue is no longer full.
 * @version 0.1
 * @date 2024-02-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <mutex>
#include <vector>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <semaphore.h>

// Reference for semaphore implementation
// http://www.csc.villanova.edu/~mdamian/threads/posixsem.html

// Function to give a timing the program and cause it to wait.
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

// Implementation of the Semaphores. Creates a queue and uses semaphores to allow access control for the queue
// between the put and get functions.
class SemaphoreQueue
{
private:
    #define SIZE  10
    int buffer[SIZE];
    int start;
    int end;
    int length = 0;
    sem_t semEmpty;  // Semaphore to track empty slots in the buffer
    sem_t semFull;   // Semaphore to track filled slots in the buffer
    std::mutex mutex; // Mutex for synchronizing access to the buffer

public:
    SemaphoreQueue()
    {
        sem_init(&semEmpty, 0, SIZE);  // Initialize semEmpty to SIZE (maximum allowed empty slots)
        sem_init(&semFull, 0, 0);       // Initialize semFull to 0 (initially no filled slots)
        start = 0;
        end = 0;
    }

    void put(int i)
    {
        sem_wait(&semEmpty);  // Wait until there's space available in the buffer
        mutex.lock();
        buffer[end] = i;
        end += 1;
        end = end % SIZE;
        length += 1;
        printf("Added %d to queue\n", i);
        mutex.unlock();
        sem_post(&semFull);   // Signal that a slot has been filled
    }

    int get() 
    {
        sem_wait(&semFull);  // Wait until there's at least one item in the buffer
        int i;
        mutex.lock();
        i = buffer[start];
        start += 1;
        start = start % SIZE;
        length -= 1;
        mutex.unlock();
        sem_post(&semEmpty); // Signal that a slot has been emptied
        printf("Consuming %d from queue\n", i);
        return i;
    }

    int getLength()
    {
        return length;
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

int main()
{
    unsigned int current_time = time(NULL);
    srand(current_time);

    SemaphoreQueue sq = SemaphoreQueue();
    while(true)
    {
        if(randomIF(0.5))
        {
            // PRODUCER
            int number = rand() % 100;
            sq.put(rand() % 100);
        }
        else
        {
            // CONSUMER
            if(sq.getLength() > 0)
            {
                int i = sq.get();
            }
            else
            {
                printf("Buffer is empty, consumer cannot proceed\n");
            }
        }
        msleep(500);
    }

    return 0;
}
