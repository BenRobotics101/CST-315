#include <mutex>
#include <vector>
#include <ctime>


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
    std::mutex mainMutex;

public:
    MonitorQueue()
    {

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