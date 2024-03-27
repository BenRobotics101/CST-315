
#include <chrono>

unsigned long getCurrentTime();

class Process
{
private:
    static unsigned int processCount;
    unsigned int processID;
    unsigned int scheduleState;
    int priority;
    uint64_t timeSubmitted;
    uint64_t timeRemaining;
    uint64_t timeRequired;

public:
    Process(uint64_t timeReq);

};

