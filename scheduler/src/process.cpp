
#include <chrono>
#include <process.h>

unsigned long getCurrentTime()
{
    using namespace std::chrono;
    const auto p1 = system_clock::now();
    uint64_t out = duration_cast<milliseconds>(p1.time_since_epoch()).count();
    return out;
}

unsigned int Process::processCount = 1;

Process::Process(uint64_t timeReq)
{
    processID = processCount;
    scheduleState = 1; // READY.
    timeRequired = timeReq; 
    timeRemaining = timeReq;
    timeSubmitted = getCurrentTime();
}

