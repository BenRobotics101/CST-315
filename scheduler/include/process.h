
#include <chrono>

uint64_t getCurrentTime();

class Process
{
private:
    static unsigned int processCount;
    unsigned int processID;
    unsigned int scheduleState;
    int priority;
    int64_t timeSubmitted;
    int64_t timeRemaining;
    int64_t timeRequired;
    uint64_t lastExc;
    float ioRate;
    bool done;

public:
    Process(int64_t timeReq);

    void changeState(unsigned int state);
    unsigned int getState();
    unsigned int getID();
    unsigned int getPriority();
    void setPriority(int p);

    int64_t startExecution();
    int8_t stopExecution();

    bool isDone();


};

