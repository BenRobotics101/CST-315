
#include <ctime>

class Process
{
private:
    static unsigned int processCount;
    unsigned int processID;
    unsigned int scheduleState;
    int priority;
    unsigned long timeSubmitted;
    unsigned long timeRemaining;
    unsigned long timeRequired;

public:
    Process(unsigned long timeReq)
    {
        processID = processCount;
        scheduleState = 1; // READY.
        timeRequired = timeReq; 
        timeRemaining = timeReq;
        timeSubmitted = 
    }

};

unsigned int Process::processCount = 1;