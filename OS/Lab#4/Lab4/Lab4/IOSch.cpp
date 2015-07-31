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
#include <algorithm>
#include "IOSch.h"
using namespace std;
iotask temp = {0, 0, 0, false, 0, 0, false,};
iotask prevTask = {0, 0, 0, false, 0, 0, false,};
int minTrack;
int directionState = 0;
//bool DecState = false;

// -----------> FIFO <------------

int FIFOScheduler::chooseDirection(){return 0;};
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

int SSTFScheduler::chooseDirection(){return 0;};
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


// -----------> SCAN <--------------

void SCANScheduler::put_taskQueue(iotask iotask){
    taskQueue.push(iotask);
    //    printf("id:%i  timestep:%i  track:%i\n", iotask.taskID, iotask.timeStep, iotask.track);
}

bool SCANScheduler::taskReady(int time){
    if (taskQueue.front().timeStep <= time) return true;
    return false;
}

bool SCANScheduler::taskQueueEmpty(){
    if (!taskQueue.empty()) return false;
    return true;
}

void SCANScheduler::put_readyTask(iotask iotask){
    readyTask.push(iotask);
}

bool SCANScheduler::bothEmpty(){
    if (!taskQueue.empty() || !readyTask.empty()) return false;
    return true;
}

iotask SCANScheduler::getReadyTask(){
    iotask iotask = taskQueue.front();
    taskQueue.pop();
    return iotask;
}

bool SCANScheduler::readyEmpty(){
    if (!readyTask.empty()) return false;
    return true;
}


int SCANScheduler::chooseDirection(){
    
    for (int i = 0; i < readyTask.size(); i++) {
        if (prevTask.track < readyTask.front().track) {
            return 2;
        } else {
            readyTask.push(readyTask.front());
            readyTask.pop();
        }
    }
    return 1;
}

iotask SCANScheduler::getRunningTask(){
    if (prevTask.timeState == false) {
        temp = readyTask.front();
        readyTask.pop();
        prevTask = temp;
        return temp;
    } else {
        if (directionState == 0) {
            if (chooseDirection() == 2) {
                directionState = 2;
            } else if (chooseDirection() == 1){
                directionState = 1;
            }
        }
        while (!readyTask.empty()) {
        
            if (directionState == 1){
                for (int i = 0; i < readyTask.size(); i++) {
                    if (readyTask.front().track <= prevTask.track) {
                        temp = readyTask.front();
                        readyTask.pop();
                        break;
                    } else{
                        readyTask.push(readyTask.front());
                        readyTask.pop();
                    }
                }
                if (temp.taskID == prevTask.taskID && !readyTask.empty()) {
                    directionState = 2;
                } else{
                    for (int i = 0; i < readyTask.size(); i++) {
                        if (readyTask.front().track <= prevTask.track) {
                            if (temp.track < readyTask.front().track) {
                                readyTask.push(temp);
                                temp = readyTask.front();
                                readyTask.pop();
                            } else if (temp.track == readyTask.front().track){
                                if(temp.taskID > readyTask.front().taskID){
                                    readyTask.push(temp);
                                    temp = readyTask.front();
                                    readyTask.pop();
                                } else {
                                    readyTask.push(readyTask.front());
                                    readyTask.pop();
                                }
                            } else{
                                readyTask.push(readyTask.front());
                                readyTask.pop();
                            }
                        } else {
                            readyTask.push(readyTask.front());
                            readyTask.pop();
                        }
                    }
                    prevTask = temp;
                    return temp;
                }
                
            }
        if (directionState == 2){
            for (int i = 0; i < readyTask.size(); i++) {
                if (readyTask.front().track >= prevTask.track) {
                    temp = readyTask.front();
                    readyTask.pop();
                    break;
                } else{
                    readyTask.push(readyTask.front());
                    readyTask.pop();
                }
            }
            if (temp.taskID == prevTask.taskID && !readyTask.empty()) {
                directionState = 1;
            } else{
                for (int i = 0; i < readyTask.size(); i++) {
                    if (readyTask.front().track >= prevTask.track) {
                        if (temp.track > readyTask.front().track) {
                            readyTask.push(temp);
                            temp = readyTask.front();
                            readyTask.pop();
                        } else if (temp.track == readyTask.front().track){
                            if(temp.taskID > readyTask.front().taskID){
                                readyTask.push(temp);
                                temp = readyTask.front();
                                readyTask.pop();
                            } else {
                                readyTask.push(readyTask.front());
                                readyTask.pop();
                            }
                        } else{
                            readyTask.push(readyTask.front());
                            readyTask.pop();
                        }
                    } else{
                        readyTask.push(readyTask.front());
                        readyTask.pop();
                    }
                }
                prevTask = temp;
                return temp;
            }
            
        }
    }
    }
    prevTask = temp;
    return temp;
    
}

//------------------> CSCAN <-------------------

void CSCANScheduler::put_taskQueue(iotask iotask){
    taskQueue.push(iotask);
    //    printf("id:%i  timestep:%i  track:%i\n", iotask.taskID, iotask.timeStep, iotask.track);
}

bool CSCANScheduler::taskReady(int time){
    if (taskQueue.front().timeStep <= time) return true;
    return false;
}

bool CSCANScheduler::taskQueueEmpty(){
    if (!taskQueue.empty()) return false;
    return true;
}

void CSCANScheduler::put_readyTask(iotask iotask){
    readyTask.push(iotask);
}

bool CSCANScheduler::bothEmpty(){
    if (!taskQueue.empty() || !readyTask.empty()) return false;
    return true;
}

iotask CSCANScheduler::getReadyTask(){
    iotask iotask = taskQueue.front();
    taskQueue.pop();
    return iotask;
}

bool CSCANScheduler::readyEmpty(){
    if (!readyTask.empty()) return false;
    return true;
}


int CSCANScheduler::chooseDirection(){
    
    for (int i = 0; i < readyTask.size(); i++) {
        if (prevTask.track <= readyTask.front().track) {
            return 2;
        } else {
            readyTask.push(readyTask.front());
            readyTask.pop();
        }
    }
    return 1;
}

iotask CSCANScheduler::getRunningTask(){
    if (prevTask.timeState == false) {
        temp = readyTask.front();
        readyTask.pop();
        prevTask = temp;
        return temp;
    } else {
        if (directionState == 0) {
            if (chooseDirection() == 2) {
                directionState = 2;
            } else if (chooseDirection() == 1){
                directionState = 1;
            }
        }
        while (!readyTask.empty()) {
            if (directionState == 1){
                while (chooseDirection() == 2) {
                  for (int i = 0; i < readyTask.size(); i++) {
                    if (readyTask.front().track >= prevTask.track) {
                        temp = readyTask.front();
                        readyTask.pop();
                        break;
                    } else{
                        readyTask.push(readyTask.front());
                        readyTask.pop();
                    }
                  }
                    
                for (int i = 0; i < readyTask.size(); i++) {
                    if (readyTask.front().track >= prevTask.track) {
                        if (readyTask.front().track < temp.track) {
                            readyTask.push(temp);
                            temp = readyTask.front();
                            readyTask.pop();
                        } else if (temp.track == readyTask.front().track){
                            if(temp.taskID > readyTask.front().taskID){
                                readyTask.push(temp);
                                temp = readyTask.front();
                                readyTask.pop();
                            } else {
                                readyTask.push(readyTask.front());
                                readyTask.pop();
                            }
                        } else{
                            readyTask.push(readyTask.front());
                            readyTask.pop();
                        }
                    } else{
                        readyTask.push(readyTask.front());
                        readyTask.pop();
                    }
                }
                prevTask = temp;
                return temp;
            }
                
                if (chooseDirection() == 1) {
                    temp = readyTask.front();
                    readyTask.pop();
                    for (int i = 0 ; i < readyTask.size(); i++) {
                        if (temp.track > readyTask.front().track) {
                            readyTask.push(temp);
                            temp = readyTask.front();
                            readyTask.pop();
                        } else if (temp.track == readyTask.front().track){
                            if(temp.taskID > readyTask.front().taskID){
                                readyTask.push(temp);
                                temp = readyTask.front();
                                readyTask.pop();
                            } else {
                                readyTask.push(readyTask.front());
                                readyTask.pop();
                            }
                        } else {
                            readyTask.push(readyTask.front());
                            readyTask.pop();
                        }
                    }
                    prevTask = temp;
                    return temp;
                }
            }
            if (directionState == 2){
                for (int i = 0; i < readyTask.size(); i++) {
                    if (readyTask.front().track >= prevTask.track) {
                        temp = readyTask.front();
                        readyTask.pop();
                        break;
                    } else{
                        readyTask.push(readyTask.front());
                        readyTask.pop();
                    }
                }
                if (temp.taskID == prevTask.taskID && !readyTask.empty()) {
                    directionState = 1;
                } else{
                    for (int i = 0; i < readyTask.size(); i++) {
                        if (readyTask.front().track >= prevTask.track) {
                            if (temp.track > readyTask.front().track) {
                                readyTask.push(temp);
                                temp = readyTask.front();
                                readyTask.pop();
                            } else if (temp.track == readyTask.front().track){
                                if(temp.taskID > readyTask.front().taskID){
                                    readyTask.push(temp);
                                    temp = readyTask.front();
                                    readyTask.pop();
                                } else {
                                    readyTask.push(readyTask.front());
                                    readyTask.pop();
                                }
                            } else{
                                readyTask.push(readyTask.front());
                                readyTask.pop();
                            }
                        } else{
                            readyTask.push(readyTask.front());
                            readyTask.pop();
                        }
                    }
                    prevTask = temp;
                    return temp;
                }
                
            }
        }
    }
    prevTask = temp;
    return temp;
    
}

