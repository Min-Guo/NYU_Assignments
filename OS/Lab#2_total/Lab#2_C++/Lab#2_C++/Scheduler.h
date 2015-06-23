#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <queue>
#include <vector>



struct Process{
    int ID;
    double AT; /* arriving time*/
    bool atRead; /* if AT has been read from input file, it is true*/
    double TC; /* Total CPU time */
    bool tcRead;
    double CB; /* CPU burst*/
    bool cbRead;
    double IO; /* IO burst*/
    bool ioRead;
}; 


class CompareAT {
public:
    bool operator()(Process& process1, Process& process2)
    {
        if (process1.AT > process2.AT) return true;
        return false;
    }
};

class Scheduler{
private:
    std::priority_queue<Process, std::vector<Process>, CompareAT> event_queue;
    std::priority_queue<Process, std::vector<Process>, CompareAT> ready_queue;
public:
    void put_eventqueue(Process process);
    Process get_eventqueue();
    void put_readyqueue(Process process);
    Process get_readyqueue();
    bool bothEmpty();
    bool isReady(double time);
    bool readyEmpty();
    bool eventEmpty();
    Process checkFirstEvent();
};


#endif