#include <mutex>
#include <vector>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <semaphore.h>

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

class MonitorQueue
{
private:
    #define SIZE  10
    int buffer[SIZE];
    int start;
    int end;
    int length = 0;
    sem_t semEmpty;  // Semaphore to track empty slots in the buffer
    sem_t semFull;   // Semaphore to track filled slots in the buffer

public:
    MonitorQueue()
    {
        sem_init(&semEmpty, 0, SIZE);  // Initialize semEmpty to SIZE (maximum allowed empty slots)
        sem_init(&semFull, 0, 0);       // Initialize semFull to 0 (initially no filled slots)
    }

    void put(int i)
    {
        sem_wait(&semEmpty);  // Wait until there's space available in the buffer
        buffer[end] = i;
        end = (end + 1) % SIZE;
        length++;
        printf("Added %d to queue\n", i);
        sem_post(&semFull);   // Signal that a slot has been filled
    }

    int get() 
    {
        sem_wait(&semFull);  // Wait until there's at least one item in the buffer
        int i = buffer[start];
        start = (start + 1) % SIZE;
        length--;
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
                int i = mq.get();
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
