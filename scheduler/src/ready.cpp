# include <iostream>
# include <list>
# include <queue>
# include "process.h" 

class Ready {
    private:
        std::queue<Process> processQueue;
        std::list<Process> sortingList;
        bool sort_by_remaining_time(const Process &a, const Process &b) { return a.timeRemaining < b.timeRemaining; }
    public:
        void pushToQueue(Process p) {
            processQueue.push(p);
            sortingList.push_back(p);
            std::sort(sortingList.begin(), sortingList.end(), sort_by_remaining_time);
        } 
        Process popFromQueue() {
            return sortingList.pop_back();
        }

};