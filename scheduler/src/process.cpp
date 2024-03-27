
#include <chrono>
#include <process.h>
#include <random>

uint64_t getCurrentTime()
{
    using namespace std::chrono;
    const auto p1 = system_clock::now();
    uint64_t out = duration_cast<milliseconds>(p1.time_since_epoch()).count();
    return out;
}


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
    float randomVal = (float)std::rand() / (float)RAND_MAX;
    
    return randomVal <= percent; 
}

int randomNumber(int min, int max)
{
    return std::rand() % (max - min) + min;
}


unsigned int Process::processCount = 1;

Process::Process(int64_t timeReq)
{
    processID = processCount;
    scheduleState = 1; // READY.
    timeRequired = timeReq; 
    timeRemaining = timeReq;
    timeSubmitted = getCurrentTime();
    lastExc = 0;
}

void Process::changeState(unsigned int state)
{
    scheduleState = state;
}

unsigned int Process::getState()
{
    return scheduleState;
}

unsigned int Process::getID()
{
    return processID;
}

unsigned int Process::getPriority()
{
    return priority;
}

void Process::setPriority(int p)
{
    priority = p;
}

int64_t Process::startExecution()
{
    if(done)
    {
        return;
    }
    lastExc = getCurrentTime();
    scheduleState = 2; // RUNNING.
    return timeRemaining;
}

int8_t Process::stopExecution()
{
    uint64_t currentTime = getCurrentTime();
    timeRemaining = timeRemaining - (currentTime - lastExc);
    if(timeRemaining < 0)
    {
        done = true;
        return 2;
    }
    scheduleState = -1; // will be assigned.
    return randomIF(ioRate); // 1 if IO, 0 if not!
}
