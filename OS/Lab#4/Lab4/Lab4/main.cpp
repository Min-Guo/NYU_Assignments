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
};
void resetRunningTask(){
    runningTask.timeState = false;
    runningTask.timeStep = 0;
    runningTask.track = 0;
    runningTask.runningState = false;
    runningTask.completeTime = 0;
};

void readInput(const char* file){
    int i = 0;
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
                    tempTask.taskID = i;
                    ioscheduler->put_taskQueue(tempTask);
                    resetTempTask();
                    i++;
                }
            }
        }
    }
};

void runningIO(){
    while (ioscheduler->bothEmpty() == false) {
        while (ioscheduler->taskReady(runningTime) == true) {
            iotask printTest;
            printTest = ioscheduler->getReadyTask();
            ioscheduler->put_readyTask(printTest);
            printf("task:%i  timeStep:%i\n", printTest.taskID, printTest.timeStep);
        }
        if (ioscheduler->readyEmpty() == false && runningTask.runningState == false) {
            runningTask = ioscheduler->getRunningTask();
            runningTask.runningState = true;
            runningTask.issueTime = runningTime;
            printf("task:%i  issueTime:%i\n", runningTask.taskID, runningTask.issueTime);
        } else if (runningTask.runningState == true && runningTime == (runningTask.track + totalCompleteTime)) {
            runningTask.completeTime = runningTime;
            tasksInform.push_back(runningTask);
            totalTrack += abs(runningTask.track - prevTrack);
            prevTrack = runningTask.track;
            printf("task:%i  finishTime:%i" , runningTask.taskID, runningTime);
            resetRunningTask();
        }
        runningTime++;
    }
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
    }
    readInput(argv[2]);
    runningIO();
    return 0;
}
