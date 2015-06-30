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
int ioTotal = 0;
int ioTime[512][2];
int tempIoTime[2][2];
int k = 0;

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
    int i = 0;
    while (i < k + 1) {
        if (ioTime[i][1] < ioTime[i + 1][0]) {
            tempIoTime[0][0] = ioTime[i][0];
            tempIoTime[0][1] = ioTime[i][1];
            tempIoTime[1][0] = ioTime[i + 1][0];
            tempIoTime[1][1] = ioTime[i + 1][1];
            ioTotal = (tempIoTime[0][1] - tempIoTime[0][0]) + (tempIoTime[1][1] - tempIoTime[1][0]);
            tempIoTime[0][0] = 0;
            tempIoTime[0][1] = 0;
            tempIoTime[1][0] = 0;
            tempIoTime[1][1] = 0;
            i++;
            
        } else if(ioTime[i][1] < ioTime[i + 1][1] && ioTime[i][1] > ioTime[i + 1][0]){
            tempIoTime[0][0] = ioTime[i][0];
            tempIoTime[0][1] = ioTime[i + 1][1];
            ioTotal = tempIoTime[0][1] - tempIoTime[0][0];
            i++;
        }
    }
    return  0;
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
    while (scheduler->bothEmpty() == false) {
        
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
                    //                    if (scheduler->readyEmpty() == true) {
                    //                        scheduler->switchPointer();
                    //                    }
                } else {
                    runningProcess.runState = true;
                    
                    //                printf("priority: %i", runningProcess.priority);
                    
                    if (runningProcess.cpuBurstRemain == 0) {
                        ofs++;
                        cpuBurst = myrandom(runningProcess.CB);
                        runningProcess.cpuBurstRemain = cpuBurst;
                        
                    }
                    //                    std::cout<<"cb:"<<runningProcess.cpuBurstRemain<<"\n";
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
                    //                    if (scheduler->readyEmpty() == true) {
                    //                        scheduler->switchPointer();
                    //                        runningProcess.runState = false;
                    //                    }
                    for (int i = 0; i < quantum + 1; i++) {
                        currentTime = runningTime + i;
                        while (scheduler->isReady(currentTime)== true && !scheduler->eventEmpty()) {
                            scheduler->put_readyqueue(scheduler->get_eventqueue());
                        }
                        //                        std::cout<< "time:" << currentTime << "  Process:" << runningProcess.ID <<"  rem:"<< runningProcess.remainTime<< "  Priority: "<< runningProcess.priority<< "\n";
                    }
                    
                    
                    if (runningProcess.cpuBurstRemain !=0) {
                        runningTime = currentTime;
                        runningProcess.order = runningTime;
                        runningProcess.remainTime -= quantum;
                        runningProcess.AT = runningTime;
                        processList[runningProcess.ID].tempAT = runningTime;
                        runningProcess.priority--;
                        //                        scheduler->decreasePriority(runningProcess);/*differet algorithm*/
                        if (runningProcess.priority == -1) {
                            runningProcess.priority = processList[runningProcess.ID].priority - 1;
                            scheduler->put_expiredqueue(runningProcess);
                            //                            if (scheduler->readyEmpty() == true) {
                            //                                scheduler->switchPointer();
                            //                            }
                            
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
                            std::cout<< "Process"<< runningProcess.ID << "   "<<processList[runningProcess.ID].FT << "   "<< processList[runningProcess.ID].IT << "   " << processList[runningProcess.ID].CW << "\n";
                            
                            
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
    
    return 0;
}