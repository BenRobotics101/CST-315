
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

#define SIZE  5

int buffer[SIZE];
int start;
int end;
int length = 0;

/* msleep(): Sleep for the requested number of milliseconds. 
https://stackoverflow.com/questions/1157209/is-there-an-alternative-sleep-function-in-c-to-milliseconds
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


void put(int i) {
    if (length <= 5) {
        buffer[end] = i;
        end += 1;
        end = end % 5;
        length += 1;
    }
    else {
        printf("Failed: Queue is full.");
    }
}

int get() {
    if (length != 0) {
        int i;
        i = buffer[start];
        start += 1;
        start = start % 5;
        length -= 1;
        return i;
    }
    else {
        printf("Failed: No item in the queue.");
    }
}



// this is from the threading example. The code from the producer and consumer part needs to be palced here



// function must take in a void* and return a void*

// int pthread_create(OUTPUT_STRUCT_REF, PTHREAD_ATTR_T for attributes to pass in normally NULL, FUNC_T>
volatile int counter;
// note, all global variables that the threads will use need "volatile"!


int produce()
{
    return counter++;
}


void* producer(void* inputItem)
{
    printf("Producer thread started\n");
    // printf("Hello!\n"); // NOTE, if there is no \n  IT WILL NOT FLUSH to console, making it invisible. >    return NULL;
    while(1)
    {
        int i = produce();
        put(i);
        msleep(250); // 250 milliseconds
    }
}

void consume(int c)
{
    printf("Your number is: %d. Length of queue: %d\n",c, length);
}
void* consumer(void* inputItem)
{
    printf("Consumer portion started\n");
    msleep(800);
    while(1)
    {
       // if(length == 0)
       //  {
       //      printf("Empty!");
       //  }
       msleep(250);
       int c = get();
       consume(c);
    }
     return NULL;
}

int main()
{
    printf("Program Started\n");
    int a = 0;
    counter = 0;
    pthread_t id;

    const char* c = "Producer Thread";

    pthread_create(&id, NULL, producer, (void*)c);

    consumer((void*)&a);
}
