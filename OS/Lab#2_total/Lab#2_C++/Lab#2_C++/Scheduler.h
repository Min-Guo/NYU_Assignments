#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <queue>
#include <vector>

#include <stdio.h>


struct Process{
    int ID;
    int priority;
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
    int remainTime;
    int FT;
    int IT;
    int CW;
    int randCPU;
    int randIO;
    int cpuBurstRemain;
};

//class SchedulerAlgorithm{
//public:
//    virtual bool operator()(Process& process1, Process& process2);
//};


//class S{
//public:
//    void put_eventqueue(Process process);
//    Process get_eventqueue();
//    void put_readyqueue(Process process);
//    Process get_readyqueue();
//    bool bothEmpty();
//    bool isReady(double time);
//    bool readyEmpty();
//    bool eventEmpty();
//    Process checkFirstEvent();
//};


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


class Scheduler{
protected:
    std::priority_queue<Process, std::vector<Process>, FCFS> event_queue;
//    std::priority_queue<Process, std::vector<Process>, FCFS> ready_queue;    /*FCFS: both eventqueue and readyqueue are assigned to FCFS*/
    /*LCFS: eventqueue:FCFS, readyqueue:LCFS*/
    /*SJF: readyqueue:SJF*/
public:
 
    void put_eventqueue(Process process);
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
public:
    void put_readyqueue(Process process);
    Process get_readyqueue();
    bool bothEmpty();
    bool isReady(double time);
    bool readyEmpty();
};

class LCFSScheduler:public Scheduler{
private:
    std::priority_queue<Process, std::vector<Process>, LCFS> ready_queue;
public:
    void put_readyqueue(Process process);
    Process get_readyqueue();
    bool bothEmpty();
    bool isReady(double time);
    bool readyEmpty();
};

class SJFScheduler:public Scheduler{
private:
    std::priority_queue<Process, std::vector<Process>, SJF> ready_queue;
public:
    void put_readyqueue(Process process);
    Process get_readyqueue();
    bool bothEmpty();
    bool isReady(double time);
    bool readyEmpty();
};




//template <class T>
//void Scheduler<T>::put_eventqueue(Process process){
//    event_queue.push(process);
//}
//
//template <class T>
//Process Scheduler<T>::checkFirstEvent(){
//    Process process = event_queue.top();
//    return process;
//}
//
//template <class T>
//Process Scheduler<T>::get_eventqueue(){
//    Process process = event_queue.top();
//    event_queue.pop();
//    return process;
//}
//
//template <class T>
//void Scheduler<T>::put_readyqueue(Process process){
//    ready_queue.push(process);
//}
//
//template <class T>
//Process Scheduler<T>::get_readyqueue(){
//    Process process = ready_queue.top();
//    ready_queue.pop();
//    return process;
//}
//
//template <class T>
//bool Scheduler<T>::bothEmpty() {
//    if (!event_queue.empty() || !ready_queue.empty()) return false;
//    return true;
//}
//
//template <class T>
//bool Scheduler<T>::isReady(double time) {
//    if (event_queue.top().AT <= time) return true;
//    return false;
//}
//
//
//template <class T>
//bool Scheduler<T>::readyEmpty(){
//    if (ready_queue.empty()) return true;
//    return false;
//}
//
//template <class T>
//bool Scheduler<T>::eventEmpty(){
//    if (event_queue.empty()) return true;
//    return false;
//}
//
//

#endif