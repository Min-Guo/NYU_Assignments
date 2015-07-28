//
//  IOSch.cpp
//  Lab4
//
//  Created by Min Guo on 7/27/15.
//  Copyright (c) 2015 Min Guo. All rights reserved.
//
#include <stdio.h>
#include "IOSch.h"
using namespace std;

void FIFOScheduler::put_taskQueue(iotask iotask){
    taskQueue.push(iotask);
}

bool FIFOScheduler::taskReady(int time){
    if (taskQueue.front().timeStep <= time) return true;
    return false;
}

void FIFOScheduler::put_readyTask(iotask iotask){
    readyTask.push(iotask);
}

bool FIFOScheduler::bothEmpty(){
    if (!taskQueue.empty() || !readyTask.empty()) return false;
    return true;
}

iotask FIFOScheduler::getReadyTask(){
    iotask iotask = taskQueue.front();
    taskQueue.pop();
    return iotask;
}

bool FIFOScheduler::readyEmpty(){
    if (!readyTask.empty()) return false;
    return true;
}

iotask FIFOScheduler::getRunningTask(){
    iotask iotask = readyTask.front();
    readyTask.pop();
    return iotask;
}