#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <queue>
#include <vector>



struct Process{
    int ID;
    int order;
    int tempAT;
    int AT; /* arriving time*/
    bool atRead; /* if AT has been read from input file, it is true*/
    int TC; /* Total CPU time */
    bool tcRead;
    int CB; /* CPU burst*/
    bool cbRead;
    int IO; /* IO burst*/
    bool ioRead;
    double remainTime;
    int FT;
    int IT;
    int CW;
}; 


class CompareAT {
public:
    bool operator()(Process& process1, Process& process2)
    {
        if (process1.AT > process2.AT) {
            return true;
        } else if (process1.AT == process2.AT){
            if (process1.order > process2.order){
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
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