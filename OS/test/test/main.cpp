//
//  main.cpp
//  test
//
//  Created by Min Guo on 6/21/15.
//  Copyright (c) 2015 Min Guo. All rights reserved.
//

#include <iostream>
#include <queue>
#include <iomanip>

using namespace std;

struct Time {
    int h;
    int m;
    int s;
};

class CompareTime {
public:
    bool operator() (Time& t1, Time& t2) {
        if (t1.h > t2.h) return true;
        if (t1.h == t2.h && t1.m > t2.m) return true;
        return false;
    }
};

int main() {
    std::priority_queue<Time, vector<Time>, CompareTime> pq;
    Time t[4] = { {3, 2, 40}, {3, 2, 26}, {5, 16, 13}, {5, 14, 20}};
    for (int i =0; i < 4; i++)
        pq.push(t[i]);
        
    
    while (!pq.empty()) {
        Time t = pq.top();
        cout << setw(3) << t.h << " " << setw(3) << t.m << " " <<
        setw(3) << t.s << endl;
        pq.pop();
    }
    
    return 0;
}




#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <queue>
#include <vector>



struct Process{
    //    double ID;
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
};


#endif




//
//  Scheduler.cpp
//  Lab#2_C++
//
//  Created by Min Guo on 6/21/15.
//  Copyright (c) 2015 Min Guo. All rights reserved.
//

#include <stdio.h>
#include "Scheduler.h"




void Scheduler::put_eventqueue(Process process){
    event_queue.push(process);
}

Process Scheduler::get_eventqueue(){
    Process process = event_queue.top();
    event_queue.pop();
    return process;
}


void Scheduler::put_readyqueue(Process process){
    ready_queue.push(process);
}

Process Scheduler::get_readyqueue(){
    Process process = ready_queue.top();
    ready_queue.pop();
    return process;
}

bool Scheduler::bothEmpty() {
    if (event_queue.empty() && ready_queue.empty()) return true;
    return false;
}


bool Scheduler::isReady(double time) {
    if (event_queue.top().AT == time) return true;
    return false;
}
