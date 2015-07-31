//
//  IOSch.h
//  Lab4
//
//  Created by Min Guo on 7/27/15.
//  Copyright (c) 2015 Min Guo. All rights reserved.
//

#ifndef __Lab4__IOSch__
#define __Lab4__IOSch__

#include <stdio.h>
#include <queue>
using namespace std;

struct iotask{
    int taskID;
    int timeStep;
    int track;
    bool timeState;
    int issueTime;
    int completeTime;
    bool runningState;
    bool separateState;
};

class Increasing{
public:
    bool operator()(iotask& iotask1, iotask& iotask2){
        if (iotask1.track > iotask2.track) {
            return true;
        } else if (iotask1.track == iotask2.track){
            if(iotask1.timeStep > iotask2.timeStep){
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
};

class Decreasing{
public:
    bool operator()(iotask& iotask1, iotask& iotask2){
        if (iotask1.track < iotask2.track) {
            return true;
        } else if (iotask1.track == iotask2.track){
            if(iotask1.timeStep > iotask2.timeStep){
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
};

class IOScheduler{
private:
    queue<iotask> taskQueue;
    queue<iotask> readyTask;
public:
    virtual void put_taskQueue(iotask iotask) = 0;
    virtual iotask getReadyTask() = 0;
    virtual bool taskReady(int time) = 0;
    virtual void put_readyTask(iotask iotask) = 0;
    virtual bool bothEmpty() = 0;
    virtual bool readyEmpty() = 0;
    virtual iotask getRunningTask() = 0;
    virtual bool taskQueueEmpty() = 0;
    virtual void separateTask() = 0;
    virtual int chooseDirection() = 0;
};

class FIFOScheduler:public IOScheduler{
private:
    queue<iotask> taskQueue;
    queue<iotask> readyTask;
public:
    void put_taskQueue(iotask iotask);
    iotask getReadyTask();
    bool taskReady(int time);
    void put_readyTask(iotask iotask);
    bool bothEmpty();
    iotask getRunningTask();
    bool readyEmpty();
    bool taskQueueEmpty();
    void  separateTask();
    int chooseDirection();
//    bool chooseDecreasing();
};

class SSTFScheduler:public IOScheduler{
private:
    queue<iotask> taskQueue;
    queue<iotask> readyTask;
public:
    void put_taskQueue(iotask iotask);
    iotask getReadyTask();
    bool taskReady(int time);
    void put_readyTask(iotask iotask);
    bool bothEmpty();
    iotask getRunningTask();
    bool readyEmpty();
    bool taskQueueEmpty();
    void  separateTask();
    int chooseDirection();
//    bool chooseDecreasing();
};

class SCANScheduler:public IOScheduler{
private:
    queue<iotask> taskQueue;
    queue<iotask> readyTask;
    priority_queue<iotask, vector<iotask>, Increasing> IncreasingTrackQueue;
    priority_queue<iotask, vector<iotask>, Decreasing> DecreasingTrackQueue;
public:
    void put_taskQueue(iotask iotask);
    iotask getReadyTask();
    bool taskReady(int time);
    void put_readyTask(iotask iotask);
    bool bothEmpty();
    iotask getRunningTask();
    bool readyEmpty();
    bool taskQueueEmpty();
    void  separateTask();
    int chooseDirection();
//    bool chooseDecreasing();
};

#endif /* defined(__Lab4__IOSch__) */
