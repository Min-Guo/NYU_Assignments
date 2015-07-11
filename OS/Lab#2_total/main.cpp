//
//  main.c
//  Lab#2
//
//  Created by Min Guo on 6/20/15.
//  Copyright (c) 2015 Min Guo. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <iostream>
#include <queue>
#include <iomanip>
#include "Scheduler.h"




FILE *file;
char line_buffer[512];
char line_secondbuffer[40001];
char *token;
int runningTime = 0;
Process runningProcess;
Process previousProcess;
int tempID = 0;
Process processList[512];
int cpuBurst = 0;
int ioQuantum = 0;
int currentTime = 0;
int randvals[40001];
char* numtoken;
int ofs;
int quantum = 0;
int number = 0;
char quantumAssign[15];
int TotalCpu = 0;
int ioTotal = 0.0;
int ioTime[512][2];
int tempIoTime[2];
int k = 0;
double io_util = 0.0;
int maxfintime = 0;
double cpu_util = 0.0;
double throughput = 0.0;
double avg_waittime = 0.0;
double avg_turnaround = 0.0;
int totalCW = 0;
int totalTT = 0;

/* put random number into randvals[i]*/
int readRandNum(FILE* file) {
    int i = 0;
    while (!feof(file)) {
        if (fgets(line_secondbuffer, 40001, file)!= NULL){
            numtoken = strtok(line_secondbuffer, "\n");
            randvals[i] = atoi(numtoken);
            i++;
        }
    }
    return 0;
}

/* Calcaluate cpuBurst or ioBurst */
int myrandom(int burst){
    return 1 + (randvals[ofs]  % burst);
}


/* read from input file and parse all processes to eventqueue*/
int parse(FILE *file, Scheduler* scheduler){
    
    int i = 0;
    ofs = 1;
    while (!feof(file)) {
        if (fgets(line_buffer, 512, file)!= NULL) {
            
            Process process = {0, 0, false, 0, false, 0, false, 0, false};
            if (strcmp(line_buffer, "\n") != 0) {   /* read input file line by line */
                token = strtok(line_buffer, "\n");
                token = strtok(line_buffer, " ");
                
                while (token!= NULL) {
                    if (process.atRead == false) {
                        process.AT = atoi(token);
                        /* store the original arrival time of each process to processlist*/
                        processList[i].AT = atoi(token);
                        processList[i].tempAT = atoi(token);
                        process.atRead = true;
                    } else if (process.tcRead == false){
                        process.TC = atoi(token);
                        process.remainTime = atoi(token);
                        processList[i].TC = atoi(token);
                        TotalCpu += process.TC;
                        //                        printf("TotalCpu: %i\n", TotalCpu);
                        process.tcRead = true;
                    } else if (process.cbRead == false){
                        process.CB = atoi(token);
                        process.cbRead = true;
                        processList[i].CB = atoi(token);
                    } else if (process.ioRead == false){
                        process.IO = atoi(token);
                        process.ioRead = true;
                        processList[i].IO = atoi(token);
                    }
                    token = strtok(NULL, " ");
                    
                }
                process.ID = tempID;
                process.order = tempID;
                process.priority = myrandom(4);
                processList[i].priority = process.priority; /* store the static priority to processlist*/
                process.priority--;
                tempID++;
                i++;
                ofs++;
                /* put the process into eventqueue*/
                scheduler->put_eventqueue(process);
                
                
            }
        }
    }
    return 0;
};


/* Calcaluate the cpu waittime of each process*/
int readyTime(int j){
    processList[j].CW = processList[j].CW + (runningTime - processList[j].tempAT);
    return 0;
}

/* Calcaluate the io time of all processes */
int ioUtilize(){
    tempIoTime[0] = ioTime[0][0];
    tempIoTime[1] = ioTime[0][1];
    ioTotal = tempIoTime[1] - tempIoTime[0];
        for (int i = 0; i < k + 1; i++) {
           
        if (tempIoTime[1] < ioTime[i + 1][0]) {
            ioTotal += (ioTime[i + 1][1] - ioTime[i + 1][0]);
            tempIoTime[0] = ioTime[i + 1][0];
            tempIoTime[1] = ioTime[i + 1][1];
            
        } else if(tempIoTime[1] < ioTime[i + 1][1] && tempIoTime[1] > ioTime[i + 1][0]){
            ioTotal += ioTime[i + 1][1] - tempIoTime[1];
            tempIoTime[0] = ioTime[i][0];
            tempIoTime[1] = ioTime[i + 1][1];
        } else if(tempIoTime[0] < ioTime[i + 1][0] && tempIoTime[1] > ioTime[i + 1][1]){
            ioTotal = ioTotal;
        } else if(tempIoTime[1] == ioTime[i + 1][0]){
            tempIoTime[1] = ioTime[i + 1][1];
            tempIoTime[0] = ioTime[i + 1][0];
            ioTotal += ioTime[i + 1][1] - ioTime[i + 1][0];
        }
    }
    io_util = 100.0 * ioTotal / maxfintime;
    return  0;
}

void printResult(){
    for (int i = 0; i < tempID ; i++) {
        printf("%04d: %4d %4d %4d %4d %1d | %5d %5d %5d %5d\n",
               processList[i].ID,
               processList[i].AT,
               processList[i].TC,
               processList[i].CB,
               processList[i].IO,
               processList[i].priority,
               processList[i].FT,
               processList[i].TT,
               processList[i].IT,
               processList[i].CW);
    }
    
    printf("SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n",
           maxfintime,
           cpu_util,
           io_util,
           avg_turnaround,
           avg_waittime,
           throughput);
}

int main(int argc, const char * argv[]) {
    
    file = fopen(argv[3], "r");
    readRandNum(file);
    fclose(file);
    Scheduler* scheduler;
    
    /* command line arguments with different scheduler method*/
    if (argv[1][2] == 'F') {
        scheduler = new FCFSScheduler();
        strcpy(quantumAssign, "runningCpuBurst");
    } else if (argv[1][2] == 'L') {
        scheduler = new LCFSScheduler();
        strcpy(quantumAssign, "runningCpuBurst");
    } else if (argv[1][2] == 'S'){
        scheduler = new SJFScheduler();
        strcpy(quantumAssign, "runningCpuBurst");
    } else if (argv[1][2] == 'R') {
        scheduler = new FCFSScheduler();
        number = atoi(&argv[3][3]);
        quantum = number;
    } else if (argv[1][2] == 'P') {
        scheduler = new PRIOScheduler();
        number = atoi(&argv[1][3]);   /* get the number from the argument*/
        quantum = number;
    } else {
        printf("Argument is expected.");
    }
    
    file = fopen(argv[2], "r");
    parse(file, scheduler);
    fclose(file);
    Process runningProcess = {0, 0, false, 0, false, 0, false, 0, false};
    /* tempID is the number of the processes in the input file */
    ofs = tempID;
    
    /* at least one queue is not empty*/
    while (scheduler->bothEmpty() == false || scheduler->expiredEmpty() == false) {
        /* check the process in eventqueue is ready or not*/
        while (scheduler->isReady(runningTime)== true && !scheduler->eventEmpty()) {
            scheduler->put_readyqueue(scheduler->get_eventqueue());
        }
        
        if (runningProcess.runState == false) {
            if (scheduler->readyEmpty() == true) {
                while (scheduler->isReady(runningTime)== true && !scheduler->eventEmpty()) {
                    scheduler->put_readyqueue(scheduler->get_eventqueue());
                    
                }
                /* If there is no process in eventqueue is ready and readyqueue is empty, switch the pointers of readyqueue and expiredqueue*/
                if (scheduler->readyEmpty() == true) {
                    scheduler->switchPointer();
                }
                if (scheduler->readyEmpty() == true) {
                    runningTime ++;
                }
                
                
            } else {
                /* push the process out of readyqueue to cpu*/
                runningProcess = scheduler->get_readyqueue();
                /* If the priority of process is -1, put it to expiredqueue*/
                if (runningProcess.priority == -1) {
                    scheduler->put_expiredqueue(runningProcess);
                } else {
                    runningProcess.runState = true;
                    if (runningProcess.cpuBurstRemain == 0) {
                        ofs++;
                        /* wrap around when running out of number in rfile*/
                        if (ofs > 40000) {
                            ofs = 1;
                        }
                        cpuBurst = myrandom(runningProcess.CB);
                        runningProcess.cpuBurstRemain = cpuBurst;
                        
                    }
                    /* Different scheduler method assign different quantum*/
                    if (strcmp(quantumAssign, "runningCpuBurst") == 0 ) {
                        quantum = runningProcess.cpuBurstRemain;
                    } else {
                        quantum = number;
                    }
                    
                    if (quantum >= runningProcess.cpuBurstRemain) {
                        quantum = runningProcess.cpuBurstRemain;
                        runningProcess.cpuBurstRemain = 0;
                        if (quantum > runningProcess.remainTime) {
                            quantum = runningProcess.remainTime;
                        }
                    } else {
                        runningProcess.cpuBurstRemain = runningProcess.cpuBurstRemain - quantum;
                        if (quantum >= runningProcess.remainTime) {
                            quantum = runningProcess.remainTime;
                            runningProcess.cpuBurstRemain = 0;
                        }
                    }
                    
                    readyTime(runningProcess.ID);
                    if (runningProcess.priority == -1) {
                        /*reset the static_priority and put the process into expiredqueue*/
                        runningProcess.priority = processList[runningProcess.ID].priority - 1;
                        scheduler->put_expiredqueue(runningProcess);
                    }
                    for (int i = 0; i < quantum + 1; i++) {
                        currentTime = runningTime + i;
                        while (scheduler->isReady(currentTime)== true && !scheduler->eventEmpty()) {
                            scheduler->put_readyqueue(scheduler->get_eventqueue());
                        }
                    }
                     /* decrease the priority after running out cpu*/
                    scheduler->decreasePriority(&runningProcess);
                    
                    /* if the process doesn't run out of the cpuBurst, put it into the readyqueue */
                    if (runningProcess.cpuBurstRemain !=0) {
                        runningTime = currentTime;
                        runningProcess.order = runningTime;
                        runningProcess.remainTime -= quantum;
                        runningProcess.AT = runningTime;
                        processList[runningProcess.ID].tempAT = runningTime;
                       
                        if (runningProcess.priority == -1) {
                            runningProcess.priority = processList[runningProcess.ID].priority - 1;
                            scheduler->put_expiredqueue(runningProcess);
                            
                        } else{
                            scheduler->put_readyqueue(runningProcess);
                        }
                    } else{
                        runningTime = currentTime;
                        runningProcess.order = runningTime;
                        runningProcess.remainTime -= quantum;
                        if (runningProcess.remainTime != 0) {
                            previousProcess.IO = runningProcess.IO;
                            ofs++;
                            if (ofs > 40000) {
                                ofs = 1;
                            }
                            ioQuantum = myrandom(previousProcess.IO);
                            ioTime[k][0] = currentTime;
                            ioTime[k][1] = currentTime + ioQuantum;
                            k++;
                            /* run io and calcaluate the next arrival time*/
                            processList[runningProcess.ID].IT += ioQuantum;
                            runningProcess.AT = runningTime + ioQuantum;
                            processList[runningProcess.ID].tempAT = runningProcess.AT;
                            runningProcess.priority = processList[runningProcess.ID].priority - 1;
                            scheduler->put_eventqueue(runningProcess);
                            
                            
                        } else {
                            /* run out of the total cpu time*/
                            processList[runningProcess.ID].FT = runningTime;
                            maxfintime = processList[runningProcess.ID].FT;
                        }
                    }
                    /* reset runningProcess*/
                    runningProcess.runState = false;
                    runningProcess.AT = 0;
                    runningProcess.TC = 0;
                    runningProcess.CB = 0;
                    runningProcess.IO = 0;
                    runningProcess.order = 0;
                }
            }
            
        }
        
    }

    for (int i = 0; i < tempID; i++) {
        totalCW += processList[i].CW;
    }
    
    for (int i = 0; i < tempID; i++) {
        processList[i].TT = processList[i].FT - processList[i].AT;
    }
    
    for (int i = 0; i < tempID; i++) {
        totalTT += processList[i].TT;
    }
    ioUtilize();
    cpu_util = 100.0 * TotalCpu / maxfintime;
    avg_turnaround = 1.0 * totalTT / tempID;
    avg_waittime = 1.0 * totalCW / tempID;
    throughput = 100.0 * tempID / maxfintime;
    printResult();
    
    return 0;
}