# include <iostream>
# include <list>
# include <queue>
# include <algorithm>
# include "process.h"

class Ready {
    private:
        std::list<Process> readyQueue;
        bool sort_by_remaining_time(const Process &a, const Process &b) { return a.getTimeRemaining() < b.getTimeRemaining(); }
    public:
        void pushToQueue(Process p) {
            readyQueue.push_back(p);
            std::sort(readyQueue.begin(), readyQueue.end(), sort_by_remaining_time);
        } 
        Process popFromQueue() {
            Process p = readyQueue.back();
            readyQueue.pop_back();
            return p;
        }
};