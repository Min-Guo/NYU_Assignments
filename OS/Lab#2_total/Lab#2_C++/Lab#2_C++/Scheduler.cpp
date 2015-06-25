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

Process Scheduler::checkFirstEvent(){
    Process process = event_queue.top();
    return process;
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
    if (!event_queue.empty() || !ready_queue.empty()) return false;
    return true;
}


bool Scheduler::isReady(double time) {
    if (event_queue.top().AT <= time) return true;
    return false;
}

bool Scheduler::readyEmpty(){
    if (ready_queue.empty()) return true;
    return false;
}

bool Scheduler::eventEmpty(){
    if (event_queue.empty()) return true;
    return false;
}