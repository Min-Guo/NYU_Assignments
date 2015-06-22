#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <queue>
#include <vector>



struct Process{
    int AT; /* arriving time*/
    bool atRead; /* if AT has been read from input file, it is true*/
    int TC; /* Total CPU time */
    bool tcRead;
    int CB; /* CPU burst*/
    bool cbRead;
    int IO; /* IO burst*/
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
    std::priority_queue<Process, std::vector<Process>, CompareAT> pq;
public:
    void put_event(Process process);
    Process get_event();
};


#endif