//
//  main.cpp
//  Lab4
//
//  Created by Min Guo on 7/27/15.
//  Copyright (c) 2015 Min Guo. All rights reserved.
//

#include <vector>
#include <iostream>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "IOSch.h"
using namespace std;

int c;
char* sValue = NULL;
string lineBuffer;
char* line;
char* token;
int runningTime;
int totalTrack;
int totalCompleteTime;
int waitTime;
int prevTrack;
int taskNum;
vector<iotask> tasksInform;
IOScheduler* ioscheduler;
iotask runningTask;
iotask tempTask;

void resetTempTask(){
    tempTask.timeState = false;
    tempTask.timeStep = 0;
    tempTask.track = 0;
    tempTask.runningState = false;
    tempTask.completeTime = 0;
    tempTask.issueTime = 0;
};
void resetRunningTask(){
    runningTask.timeState = false;
    runningTask.timeStep = 0;
    runningTask.track = 0;
    runningTask.runningState = false;
    runningTask.completeTime = 0;
    runningTask.issueTime = 0;
};

void readInput(const char* file){
    ifstream infile(file);
    if (!infile.is_open()) {
        cout<<"Failed to open input file"<<endl;
    } else{
        while (!infile.eof()) {
            getline(infile, lineBuffer);
            if (lineBuffer.find("#") != 0){
                line = new char[lineBuffer.length() + 1];
                strcpy(line, lineBuffer.c_str());
                if (strcmp(line, "") != 0) {
                    token = strtok(line, "\n");
                    token = strtok(line, " ");
                    while (token != NULL) {
                        if (tempTask.timeState == false) {
                            tempTask.timeStep = atoi(token);
                            tempTask.timeState = true;
                        } else {
                            tempTask.track = atoi(token);
                        }
                        token = strtok(NULL, " ");
                    }
                    tempTask.taskID = taskNum;
                    ioscheduler->put_taskQueue(tempTask);
                    resetTempTask();
                    taskNum++;
                }
            }
        }
    }
};

void runningIO(){
    while (ioscheduler->bothEmpty() == false || runningTask.runningState == true) {
        while (ioscheduler->taskReady(runningTime) == true && ioscheduler->taskQueueEmpty() == false) {
            iotask printTest;
            printTest = ioscheduler->getReadyTask();
            ioscheduler->put_readyTask(printTest);
            printf("%i:     %i add %i\n", runningTime, printTest.taskID, printTest.track);
        }
        if (ioscheduler->readyEmpty() == false && runningTask.runningState == false) {
            runningTask = ioscheduler->getRunningTask();
            runningTask.runningState = true;
            runningTask.issueTime = runningTime;
            printf("%i:     %i issue %i %i\n", runningTime, runningTask.taskID, runningTask.track, prevTrack);
        }
        
        while (runningTask.runningState == true && runningTime == (abs(runningTask.track - prevTrack) + runningTask.issueTime)) {
            runningTask.completeTime = runningTime;
            tasksInform.push_back(runningTask);
            totalTrack += abs(runningTask.track - prevTrack);
            prevTrack = runningTask.track;
            printf("%i:     %i finish %i\n" , runningTime, runningTask.taskID, runningTask.completeTime - runningTask.timeStep);
            resetRunningTask();
            if (ioscheduler->readyEmpty() == false && runningTask.runningState == false){
                runningTask = ioscheduler->getRunningTask();
                runningTask.runningState = true;
                runningTask.issueTime = runningTime;
                printf("%i:     %i issue %i %i\n", runningTime, runningTask.taskID, runningTask.track, prevTrack);
            }
        }
        runningTime++;
    }
};

void printSum(){
    double totalTurnAround;
    double totalWaitTime;
    int maxWaitTime = 0;
    float avg_waittime = 0;
    float avg_turnaround = 0;
    for (int i = 0; i < tasksInform.size(); i++) {
        totalTurnAround += tasksInform[i].completeTime - tasksInform[i].timeStep;
        totalWaitTime += tasksInform[i].issueTime - tasksInform[i].timeStep;
        if (maxWaitTime <= (tasksInform[i].issueTime - tasksInform[i].timeStep)) {
            maxWaitTime = tasksInform[i].issueTime - tasksInform[i].timeStep;
        }
    }
    avg_turnaround = totalTurnAround / taskNum;
    avg_waittime = totalWaitTime / taskNum;
    printf("SUM: %d %d %.2lf %.2lf %d\n", runningTime - 1, totalTrack, avg_turnaround, avg_waittime, maxWaitTime);
};


int main(int argc, char** argv) {
    while ((c = getopt (argc, argv, "s::")) != -1) {
        switch (c) {
            case 's':
                sValue = optarg;
                break;
            default:
                break;
        }
    }
    if (strcmp(sValue, "i") == 0) {
        ioscheduler = new FIFOScheduler();
    } else if (strcmp(sValue, "j") == 0){
        ioscheduler = new SSTFScheduler();
    } else if (strcmp(sValue, "s") == 0){
        ioscheduler = new SCANScheduler();
    } else if (strcmp(sValue, "c") == 0){
        ioscheduler = new CSCANScheduler();
    }
    readInput(argv[2]);
    cout<< "TRACE"<<endl;
    runningIO();
    printSum();
    return 0;
}
