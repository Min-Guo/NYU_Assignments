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
Process* tempProcess;
double quantum = 0;
int currentTime = 0;
int randvals[40000];
char* numtoken;





int parse(FILE *file, Scheduler* scheduler){

    
    while (!feof(file)) {
        if (fgets(line_buffer, 512, file)!= NULL) {
            tempID++;
            Process process = {0, 0, false, 0, false, 0, false, 0, false};
            if (strcmp(line_buffer, "\n") != 0) {   /* read input file line by line */
                token = strtok(line_buffer, "\n");
                token = strtok(line_buffer, " ");
                
                while (token!= NULL) {
                    if (process.atRead == false) {
                        process.AT = atoi(token);
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




int main(int argc, const char * argv[]) {
    
    file = fopen("/Users/Min/Development/NYU_Assignments/OS/Lab#2_total/lab2_assign/rfile", "r");
    readRandNum(file);
    fclose(file);
    Scheduler scheduler;
    file = fopen("/Users/Min/Development/NYU_Assignments/OS/Lab#2_total/lab2_assign/input0", "r");
    parse(file, &scheduler);
    fclose(file);
    Process runningProcess = {0, 0, false, 0, false, 0, false, 0, false};

    while (scheduler.bothEmpty() == false) {
        
        while (scheduler.isReady(runningTime)== true && !scheduler.eventEmpty()) {
           scheduler.put_readyqueue(scheduler.get_eventqueue());
        }
        
        if (runningProcess.ID == 0) {
            if (scheduler.readyEmpty() == true) {
                
                
                std::cout<<"Running time:" << runningTime << "\n";
                runningTime ++;
            } else {
                runningProcess = scheduler.get_readyqueue();
                if (runningProcess.CB < runningProcess.remainTime) {
                    quantum = runningProcess.CB;
                } else {
                    quantum = runningProcess.remainTime;
                }
                for (int i = 0; i < quantum + 1; i++) {
                    currentTime = runningTime + i;
                    if (scheduler.isReady(currentTime)== true && !scheduler.eventEmpty()) {
                        scheduler.put_readyqueue(scheduler.get_eventqueue());
                    }
                    std::cout<< "Running time:" << currentTime << "   Running Process:" << runningProcess.ID << "\n";
                }
                runningTime = currentTime;
                runningProcess.remainTime -= quantum;
                previousProcess.IO = runningProcess.IO;
                runningProcess.AT = runningProcess.AT + runningProcess.CB + runningProcess.IO;
                if (runningProcess.remainTime != 0) {
                    scheduler.put_eventqueue(runningProcess);
                }
                
                runningProcess.ID = 0;
            
            
                for (int i = 0; i < previousProcess.IO + 1; i++) {
                    currentTime = runningTime + i;
                    while (scheduler.isReady(currentTime)== true && !scheduler.eventEmpty()) {
                        scheduler.put_readyqueue(scheduler.get_eventqueue());
                    }
                    std::cout << "Block time:" << currentTime << "   Blocked State" << "\n";
                    
                }
                runningTime = currentTime;
            }
        }
        
   
        
    }

    
    return 0;
}
