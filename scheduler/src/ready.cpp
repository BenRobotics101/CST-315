# include <iostream>
# include <queue>
# include "process.h" 

class Ready {
    private:
        queue<Process> processQueue;
    public:
        void pushToQueue(Process p) {
            processQueue.push(p);
        } 
        Process popFromQueue() {
            return processQueue.pop()
        }
        
};