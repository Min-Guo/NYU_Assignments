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
Process tempProcess;
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

int myrandom(int burst){
    return 1 + (randvals[ofs]  % burst);
}


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
                        processList[i].AT = atoi(token);
                        processList[i].tempAT = atoi(token);
                        process.atRead = true;
                    } else if (process.tcRead == false){
                        process.TC = atoi(token);
                        process.remainTime = atoi(token);
                        TotalCpu += process.TC;
                        //                        printf("TotalCpu: %i\n", TotalCpu);
                        process.tcRead = true;
                    } else if (process.cbRead == false){
                        process.CB = atoi(token);
                        process.cbRead = true;
                    } else if (process.ioRead == false){
                        process.IO = atoi(token);
                        process.ioRead = true;
                    }
                    token = strtok(NULL, " ");
                    
                }
                process.ID = tempID;
                process.order = tempID;
                process.priority = myrandom(4);
                processList[i].priority = process.priority;
                process.priority--;
                tempID++;
                i++;
                ofs++;
                scheduler->put_eventqueue(process);
                
                
            }
        }
    }
    return 0;
};



int readyTime(int j){
    processList[j].CW = processList[j].CW + (runningTime - processList[j].tempAT);
    return 0;
}

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
    std::cout<<"io "<< io_util <<"\n";
    return  0;
}

void printResult(){
    for (int i = 0; i < tempID ; i++) {
        std::cout<< "Process"<< i << "  " << processList[i].priority << "   "<<processList[i].FT << "   "<< processList[i].IT << "   " << processList[i].CW << "\n";
    }
}

int main(int argc, const char * argv[]) {
    
    file = fopen(argv[1], "r");
    readRandNum(file);
    fclose(file);
    Scheduler* scheduler;
    
    if (argv[3][2] == 'F') {
        scheduler = new FCFSScheduler();
        strcpy(quantumAssign, "runningCpuBurst");
    } else if (argv[3][2] == 'L') {
        scheduler = new LCFSScheduler();
        strcpy(quantumAssign, "runningCpuBurst");
    } else if (argv[3][2] == 'S'){
        scheduler = new SJFScheduler();
        strcpy(quantumAssign, "runningCpuBurst");
    } else if (argv[3][2] == 'R') {
        scheduler = new FCFSScheduler();
        number = atoi(&argv[3][3]);
        quantum = number;
    } else if (argv[3][2] == 'P') {
        scheduler = new PRIOScheduler();
        number = atoi(&argv[3][3]);
        quantum = number;
    } else {
        printf("Argument is expected.");
    }
    
    file = fopen(argv[2], "r");
    parse(file, scheduler);
    fclose(file);
    Process runningProcess = {0, 0, false, 0, false, 0, false, 0, false};
    ofs = tempID;
    while (scheduler->bothEmpty() == false || scheduler->expiredEmpty() == false) {
        
        while (scheduler->isReady(runningTime)== true && !scheduler->eventEmpty()) {
            scheduler->put_readyqueue(scheduler->get_eventqueue());
        }
        
        if (runningProcess.runState == false) {
            if (scheduler->readyEmpty() == true) {
                while (scheduler->isReady(runningTime)== true && !scheduler->eventEmpty()) {
                    scheduler->put_readyqueue(scheduler->get_eventqueue());
                    
                }
                if (scheduler->readyEmpty() == true) {
                    scheduler->switchPointer();
                }
                if (scheduler->readyEmpty() == true) {
                    runningTime ++;
                }
                
                
            } else {
                runningProcess = scheduler->get_readyqueue();
                if (runningProcess.priority == -1) {
                    scheduler->put_expiredqueue(runningProcess);
                } else {
                    runningProcess.runState = true;
                    if (runningProcess.cpuBurstRemain == 0) {
                        ofs++;
                        if (ofs > 40000) {
                            ofs = 1;
                        }
                        cpuBurst = myrandom(runningProcess.CB);
                        runningProcess.cpuBurstRemain = cpuBurst;
                        
                    }
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
                        runningProcess.priority = processList[runningProcess.ID].priority - 1;
                        scheduler->put_expiredqueue(runningProcess);
                    }
                    for (int i = 0; i < quantum + 1; i++) {
                        currentTime = runningTime + i;
                        while (scheduler->isReady(currentTime)== true && !scheduler->eventEmpty()) {
                            scheduler->put_readyqueue(scheduler->get_eventqueue());
                        }
                    }
                    
                    
                    if (runningProcess.cpuBurstRemain !=0) {
                        runningTime = currentTime;
                        runningProcess.order = runningTime;
                        runningProcess.remainTime -= quantum;
                        runningProcess.AT = runningTime;
                        processList[runningProcess.ID].tempAT = runningTime;
                        runningProcess.priority--;
//                                                scheduler->decreasePriority(runningProcess);/*differet algorithm*/
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
                            processList[runningProcess.ID].IT += ioQuantum;
                            runningProcess.AT = runningTime + ioQuantum;
                            processList[runningProcess.ID].tempAT = runningProcess.AT;
                            runningProcess.priority = processList[runningProcess.ID].priority - 1;
                            scheduler->put_eventqueue(runningProcess);
                            
                            
                        } else {
                            processList[runningProcess.ID].FT = runningTime;
                            maxfintime = processList[runningProcess.ID].FT;
                        }
                    }
                    
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
//    std::cout<<"finish time  "<<finishTime<< "\n";
//    printResult();
    for (int i = 0; i < tempID; i++) {
        totalCW += processList[i].CW;
    }
    
    for (int i = 0; i < tempID; i++) {
        processList[i].TT = processList[i].FT - processList[i].AT;
    }
    
    for (int i = 0; i < tempID; i++) {
        totalTT += processList[i].TT;
    }
    
    avg_turnaround = 1.0 * totalTT / tempID;
    std::cout<<"avg_turnaround "<< avg_turnaround<<"\n";
    cpu_util = 100.0 * TotalCpu / maxfintime;
    std::cout<<"Cpu "<< cpu_util <<"\n";
    throughput = 100.0 * tempID / maxfintime;
    std::cout<<"throughput "<<throughput<<"\n";
    avg_waittime = 1.0 * totalCW / tempID;
    std::cout<<"avg_waittime "<< avg_waittime<<"\n";
    ioUtilize();
    return 0;
}