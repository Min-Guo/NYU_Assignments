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
int cpuQuantum = 0;
double ioQuantum = 0;
int currentTime = 0;
int randvals[40001];
char* numtoken;
int ofs;




int parse(FILE *file, Scheduler* scheduler){
    
    int i = 0;
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
                tempID++;
                i++;
                scheduler->put_eventqueue(process);
                
                
            }
        }
    }
    return 0;
};

int readRandNum(FILE* file) {
    int i = 0;
    while (!feof(file)) {
        if (fgets(line_secondbuffer, 40001, file)!= NULL){
            numtoken = strtok(line_secondbuffer, "\n");
            randvals[i] = atoi(numtoken);
            //            printf("%i  %i\n",i, randvals[i]);
            i++;
        }
    }
    return 0;
}

int myrandom(int burst){
    return 1 + (randvals[ofs]  % burst);
}

int readyTime(int j){
    processList[j].CW = processList[j].CW + (runningTime - processList[j].tempAT);
    return 0;
}

int main(int argc, const char * argv[]) {
    
    file = fopen("/Users/Min/Development/NYU_Assignments/OS/Lab#2_total/lab2_assign/rfile", "r");
    readRandNum(file);
    fclose(file);
    Scheduler scheduler;
    file = fopen("/Users/Min/Development/NYU_Assignments/OS/Lab#2_total/lab2_assign/input6", "r");
    parse(file, &scheduler);
    fclose(file);
    Process runningProcess = {0, 0, false, 0, false, 0, false, 0, false};
    ofs = tempID;
    while (scheduler.bothEmpty() == false) {
        
        while (scheduler.isReady(runningTime)== true && !scheduler.eventEmpty()) {
            scheduler.put_readyqueue(scheduler.get_eventqueue());
        }
        
        if (runningProcess.ID == 0) {
            if (scheduler.readyEmpty() == true) {
//                std::cout<<"Running time:" << runningTime << "\n";
                while (scheduler.isReady(runningTime)== true && !scheduler.eventEmpty()) {
                    scheduler.put_readyqueue(scheduler.get_eventqueue());
                }
                runningTime ++;
            } else {
                runningProcess = scheduler.get_readyqueue();
                //                printf("RunningProcess is %i", runningProcess.ID);
                ofs++;
                
                cpuQuantum = myrandom(runningProcess.CB);
                if (cpuQuantum > runningProcess.remainTime) {
                    cpuQuantum = runningProcess.remainTime;
                }
                readyTime(runningProcess.ID);
                for (int i = 0; i < cpuQuantum + 1; i++) {
                    currentTime = runningTime + i;
                    while (scheduler.isReady(currentTime)== true && !scheduler.eventEmpty()) {
                        scheduler.put_readyqueue(scheduler.get_eventqueue());
                    }
//                    std::cout<< "Running time:" << currentTime << "   Running Process:" << runningProcess.ID << "\n";
                }
                
                runningTime = currentTime;
                runningProcess.order = runningTime;
                runningProcess.remainTime -= cpuQuantum;
                if (runningProcess.remainTime != 0) {
                    previousProcess.IO = runningProcess.IO;
                    ofs++;
                    ioQuantum = myrandom(previousProcess.IO);
                    processList[runningProcess.ID].IT += ioQuantum;
                    runningProcess.AT = runningTime + ioQuantum;
                    processList[runningProcess.ID].tempAT = runningProcess.AT;
                    scheduler.put_eventqueue(runningProcess);
                } else {
                    processList[runningProcess.ID].FT = runningTime;
                    printf("Process%i  finish:%i\n", runningProcess.ID, processList[runningProcess.ID].FT);
                    printf("Process%i  CW:%i\n", runningProcess.ID, processList[runningProcess.ID].CW);
                    
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
    
    
    return 0;
}