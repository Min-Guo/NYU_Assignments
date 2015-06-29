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
int IOTime = 0;
Process tempProcess;

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
        if (isdigit(argv[3][3])) {
            number = atoi(&argv[3][3]);
            quantum = number;
        } else {
            printf("Need a number.\n");
        }
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
//            scheduler->put_readyqueue(scheduler->get_eventqueue());
            tempProcess = scheduler->get_eventqueue();
            tempProcess.IOState = false;
            processList[tempProcess.ID].IOState = false;
            scheduler->put_readyqueue(tempProcess);
        }
        
        if (runningProcess.ID == 0) {
            if (scheduler->readyEmpty() == true) {
                while (scheduler->isReady(runningTime)== true && !scheduler->eventEmpty()) {
//                    scheduler->put_readyqueue(scheduler->get_eventqueue());
                    tempProcess = scheduler->get_eventqueue();
                    tempProcess.IOState = false;
                    processList[tempProcess.ID].IOState = false;
                    scheduler->put_readyqueue(tempProcess);
                }
                runningTime ++;
            } else {
                runningProcess = scheduler->get_readyqueue();
                
                if (runningProcess.cpuBurstRemain == 0) {
                    ofs++;
                    cpuBurst = myrandom(runningProcess.CB);
                    runningProcess.cpuBurstRemain = cpuBurst;
                    //                    printf("ofs:%i   cb:%i    rem:%i\n", ofs, cpuBurst, runningProcess.remainTime);
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
//                processList[runningProcess.ID].randCPU += quantum;
                
                readyTime(runningProcess.ID);
                for (int i = 0; i < quantum + 1; i++) {
                    currentTime = runningTime + i;
                    while (scheduler->isReady(currentTime)== true && !scheduler->eventEmpty()) {
//                        scheduler->put_readyqueue(scheduler->get_eventqueue());
                        tempProcess = scheduler->get_eventqueue();
                        tempProcess.IOState = false;
                        processList[tempProcess.ID].IOState = false;
                        scheduler->put_readyqueue(tempProcess);
                    }
                    //                                        std::cout<< "Running time:" << currentTime << "   Running Process:" << runningProcess.ID << "\n";
                }
                
                if (runningProcess.cpuBurstRemain !=0) {
                    runningTime = currentTime;
                    runningProcess.order = runningTime;
                    runningProcess.remainTime -= quantum;
                    runningProcess.AT = runningTime;
                    processList[runningProcess.ID].tempAT = runningTime;
                    scheduler->put_readyqueue(runningProcess);
                } else{
                    runningTime = currentTime;
                    runningProcess.order = runningTime;
                    runningProcess.remainTime -= quantum;
                    if (runningProcess.remainTime != 0) {
                        previousProcess.IO = runningProcess.IO;
                        ofs++;
                        ioQuantum = myrandom(previousProcess.IO);
                        processList[runningProcess.ID].IOState = true;
                        for (int k = 0; k < ioQuantum; k++){
                            for (int j = 0; j < 512; j++) {
                                if (processList[j].IOState == true) {
                                    goto Endloop;
                                }
                            }
                        Endloop:
                            IOTime++;
                        }
                        processList[runningProcess.ID].IT += ioQuantum;
                        runningProcess.AT = runningTime + ioQuantum;
                        processList[runningProcess.ID].tempAT = runningProcess.AT;
                        scheduler->put_eventqueue(runningProcess);
                        
                        
                    } else {
                        processList[runningProcess.ID].FT = runningTime;
                        std::cout<< "Process"<< runningProcess.ID << "   "<<processList[runningProcess.ID].FT << "   "<< processList[runningProcess.ID].IT << "   " << processList[runningProcess.ID].CW << "\n";
//                        printf("Process%i  finish:%i\n", runningProcess.ID, processList[runningProcess.ID].FT);
//                        printf("Process%i  CW:%i\n", runningProcess.ID, processList[runningProcess.ID].CW);
////                        printf("Process%i  randcpu:%i\n", runningProcess.ID, processList[runningProcess.ID].randCPU);
//                        printf("Process%i  IT:%i\n", runningProcess.ID, processList[runningProcess.ID].IT);
//                        printf("Process%i  PRIO:%i\n", runningProcess.ID, processList[runningProcess.ID].priority);
////                        printf("Total CPU: %i\n", TotalCpu);
//                        
                        
                        
                    }
                }
                runningProcess.ID = 0;
                runningProcess.AT = 0;
                runningProcess.TC = 0;
                runningProcess.CB = 0;
                runningProcess.IO = 0;
                runningProcess.order = 0;
            }
        }

    
        
    }
//    printf("IOTime : %i\n", IOTime);
    
    return 0;
}