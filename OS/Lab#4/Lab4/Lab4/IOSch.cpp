//
//  IOSch.cpp
//  Lab4
//
//  Created by Min Guo on 7/27/15.
//  Copyright (c) 2015 Min Guo. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include "IOSch.h"
using namespace std;
iotask temp = {0, 0, 0, false, 0, 0, false,};
iotask prevTask = {0, 0, 0, false, 0, 0, false,};
int minTrack;

// -----------> FIFO <------------
void FIFOScheduler::put_taskQueue(iotask iotask){
    taskQueue.push(iotask);
}

bool FIFOScheduler::taskReady(int time){
    if (taskQueue.front().timeStep <= time) return true;
    return false;
}

bool FIFOScheduler::taskQueueEmpty(){
    if (!taskQueue.empty()) return false;
    return true;
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
    temp = readyTask.front();
    readyTask.pop();
    return temp;
}


//----------> SSTF <-----------
void SSTFScheduler::put_taskQueue(iotask iotask){
    taskQueue.push(iotask);
//    printf("id:%i  timestep:%i  track:%i\n", iotask.taskID, iotask.timeStep, iotask.track);
}

bool SSTFScheduler::taskReady(int time){
    if (taskQueue.front().timeStep <= time) return true;
    return false;
}

bool SSTFScheduler::taskQueueEmpty(){
    if (!taskQueue.empty()) return false;
    return true;
}

void SSTFScheduler::put_readyTask(iotask iotask){
    readyTask.push(iotask);
}

bool SSTFScheduler::bothEmpty(){
    if (!taskQueue.empty() || !readyTask.empty()) return false;
    return true;
}

iotask SSTFScheduler::getReadyTask(){
    iotask iotask = taskQueue.front();
    taskQueue.pop();
    return iotask;
}

bool SSTFScheduler::readyEmpty(){
    if (!readyTask.empty()) return false;
    return true;
}

iotask SSTFScheduler::getRunningTask(){
    if (prevTask.timeState == false) {
        temp = readyTask.front();
        readyTask.pop();
    } else {
        temp = readyTask.front();
        readyTask.pop();
        minTrack = abs(temp.track - prevTask.track);
        for (int i = 0; i < readyTask.size(); i++) {
            if (minTrack > abs(readyTask.front().track - prevTask.track)) {
                minTrack = abs(readyTask.front().track - prevTask.track);
                readyTask.push(temp);
                temp = readyTask.front();
                readyTask.pop();
            } else if (minTrack == abs(readyTask.front().track - prevTask.track)){
                if (readyTask.front().timeStep < temp.timeStep){
                    readyTask.push(temp);
                    temp = readyTask.front();
                    readyTask.pop();
                } else{
                    readyTask.push(readyTask.front());
                    readyTask.pop();
                }
            } else{
                readyTask.push(readyTask.front());
                readyTask.pop();
            }
        }
    }
    prevTask = temp;
    return temp;
}
