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
    virtual int chooseDirection() = 0;
    virtual void switchQueue() = 0;
    virtual void putAnotherQueue() = 0;
    virtual void init() = 0;
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
    int chooseDirection();
    void switchQueue();
    void putAnotherQueue();
    void init();
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
    int chooseDirection();
    void switchQueue();
    void putAnotherQueue();
    void init();
};

class SCANScheduler:public IOScheduler{
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
    int chooseDirection();
    void switchQueue();
    void putAnotherQueue();
    void init();
};


class CSCANScheduler:public IOScheduler{
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
    int chooseDirection();
    void switchQueue();
    void putAnotherQueue();
    void init();
};

class FSCANScheduler:public IOScheduler{
private:
    queue<iotask> taskQueue;
    queue<iotask> readyQueueOne;
    queue<iotask>* readyTask;
    queue<iotask> readyQueueTwo;
public:
    void put_taskQueue(iotask iotask);
    iotask getReadyTask();
    bool taskReady(int time);
    void put_readyTask(iotask iotask);
    bool bothEmpty();
    iotask getRunningTask();
    bool readyEmpty();
    bool taskQueueEmpty();
    int chooseDirection();
    void switchQueue();
    void putAnotherQueue();
    void init();
};
#endif /* defined(__Lab4__IOSch__) */
