# include <iostream>
# include <list>
# include <queue>
# include <algorithm>
# include "process.h"

class Ready {
    private:
        std::list<Process> blockedQueue;
    public:
        void pushToQueue(Process p) {
            blockedQueue.push_back(p);
        } 
        Process popFromQueue() {
            Process p = blockedQueue.back();
            blockedQueue.pop_back();
            return p;
        }
};