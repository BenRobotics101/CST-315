#include <process.h>
#include <ready.h>
#include <blocked.h>

#define TIMESLICE 4

Ready rq;
Blocked bq;

int main()
{
    // just a rough sketch/outline. Pseudo code.

    int processesDone = 0;
    Process p1;
    Process p2;
    Process p3;
    Process p4;



    rq.submit(p1);
    rq.submit(p2);
    rq.submit(p3);
    rq.submit(p4);

    while(processesDone < 4)
    {
        Process p = rq.pull();
        if(p == nullptr)
        {
            continue; // no process!
        }
        uint64_t timeRemaining = p.startExecution();
        sleep( min( TIMESLICE,timeRemaining));
        int8_t status = p.stopExecution();
        if(status == 2)
        {
            processesDone++;
            continue;
        }
        if(status == 1)
        {
            // I/O request!
            bq.submit(p, Process::randomNumber(2, 50)); // I/O request time.
        }
        else
        {
            // Preempt. Go to ready state.
            rq.submit(p);
        }
    }
}

int ioThread()
{
    while(true)
    {
        if(bq.isEmpty())
        {
            sleep(1); // wait for io request.
            continue;
        }
        IOStruct p = bq.pull();   
        int32_t timeWait = IOStruct.time;
        sleep(timeWait);
    }
}