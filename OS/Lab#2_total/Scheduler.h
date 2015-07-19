#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <queue>
#include <vector>

#include <stdio.h>


struct Process{
    int ID;
    bool runState;
    int priority;
    int order; /*excute time*/
    int tempAT;
    int AT; /* arriving time*/
    bool atRead; /* if AT has been read from input file, it is true*/
    int TC; /* Total CPU time */
    bool tcRead;
    int CB; /* CPU burst*/
    bool cbRead;
    int IO; /* IO burst*/
    bool ioRead;
    int remainTime; /* remaintime of total cpu time*/
    int FT;
    int IT;
    int CW;
    int cpuBurstRemain;
    int TT;
};


class FCFS{
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

class LCFS{
public:
    bool operator()(Process& process1, Process& process2)
    {
        if (process1.AT < process2.AT) {
            return true;
        } else if (process1.AT == process2.AT){
            if (process1.order < process2.order){
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
    
};

class SJF{
public:
    bool operator()(Process& process1, Process& process2)
    {
        if (process1.remainTime > process2.remainTime) {
            return true;
        } else if (process1.remainTime == process2.remainTime){
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


class PRIO{
public:
    bool operator()(Process& process1, Process& process2)
    {
        if (process1.priority < process2.priority) {
            return true;
        }
        
        if (process1.priority == process2.priority){
            if (process1.AT > process2.AT) {
                return true;
            }
            
            if (process1.AT == process2.AT) {
                if (process1.order > process2.order) {
                    return true;
                } else {
                return false;
                }
            }
        } else {
            return false;
        }
        return false;
    }
    
};


class Scheduler{
protected:
    std::priority_queue<Process, std::vector<Process>, FCFS> event_queue;
public:
    virtual bool expiredEmpty();
    virtual Process decreasePriority(Process* process);
    virtual void switchPointer() = 0;
    void put_eventqueue(Process process);
    virtual void put_expiredqueue(Process process) = 0;
    Process get_eventqueue();
    virtual void put_readyqueue(Process process) = 0;
    virtual Process get_readyqueue() = 0;
    virtual bool bothEmpty() = 0;
    virtual bool isReady(double time) = 0;
    virtual bool readyEmpty() = 0;
    bool eventEmpty();
    Process checkFirstEvent();
};

class FCFSScheduler:public Scheduler{
private:
    std::priority_queue<Process, std::vector<Process>, FCFS> ready_queue;
    std::priority_queue<Process, std::vector<Process>, FCFS> expired_queue;
public:
    Process decreasePriority(Process* process);
    void switchPointer();
    void put_readyqueue(Process process);
    void put_expiredqueue(Process process);
    Process get_readyqueue();
    bool bothEmpty();
    bool isReady(double time);
    bool readyEmpty();
    bool expiredEmpty();
};

class LCFSScheduler:public Scheduler{
private:
    std::priority_queue<Process, std::vector<Process>, LCFS> ready_queue;
    std::priority_queue<Process, std::vector<Process>, LCFS> expired_queue;
public:
    Process decreasePriority(Process* process);
    void switchPointer();
    void put_readyqueue(Process process);
    void put_expiredqueue(Process process);
    Process get_readyqueue();
    bool bothEmpty();
    bool isReady(double time);
    bool readyEmpty();
    bool expiredEmpty();
};

class SJFScheduler:public Scheduler{
private:
    std::priority_queue<Process, std::vector<Process>, SJF> ready_queue;
    std::priority_queue<Process, std::vector<Process>, SJF> expired_queue;
public:
    Process decreasePriority(Process* process);
    void switchPointer();
    void put_readyqueue(Process process);
    void put_expiredqueue(Process process);
    Process get_readyqueue();
    bool bothEmpty();
    bool isReady(double time);
    bool readyEmpty();
    bool expiredEmpty();
};

class PRIOScheduler:public Scheduler{
private:
    std::priority_queue<Process, std::vector<Process>, PRIO> ready_queue;
    std::priority_queue<Process, std::vector<Process>, PRIO> expired_queue;
public:
    Process decreasePriority(Process* process);
    void switchPointer();
    void put_readyqueue(Process process);
    void put_expiredqueue(Process process);
    Process get_readyqueue();
    bool bothEmpty();
    bool isReady(double time);
    bool readyEmpty();
    bool expiredEmpty();
};






#endif