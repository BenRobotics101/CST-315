#ifndef READY_H
#define READY_H

#include <iostream>
#include <list>
#include <queue>
#include <algorithm>
#include "process.h"

class Ready {
    private:
        std::list<Process> readyQueue;
        bool sort_by_remaining_time(const Process &a, const Process &b) { return a.getTimeRemaining() < b.getTimeRemaining(); }
    public:
        // Constructor
        Ready() {}

        void pushToQueue(Process p);
        Process popFromQueue();
};

#endif /* READY_H */
