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
char *token;
int runningTime = 0;
Process runningProcess;
Process previousProcess;
int tempID = 0;
Process* tempProcess;







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






int main(int argc, const char * argv[]) {
    Scheduler scheduler;
    file = fopen("/Users/Min/Development/NYU_Assignments/OS/Lab#2_total/lab2_assign/input0", "r");
    parse(file, &scheduler);
    fclose(file);
    Process runningProcess = {0, 0, false, 0, false, 0, false, 0, false};

    while (scheduler.bothEmpty() == false) {
        
        while (scheduler.isReady(runningTime)== true) {
            scheduler.put_readyqueue(scheduler.get_eventqueue());
        }
        
        if (runningProcess.ID == 0) {
            if (scheduler.readyEmpty() == true) {
                runningTime ++;
            } else {
                runningProcess.ID = scheduler.get_readyqueue().ID;
                runningProcess.AT = scheduler.get_readyqueue().AT;
                runningProcess.TC = scheduler.get_readyqueue().TC;
                runningProcess.CB = scheduler.get_readyqueue().CB;
                runningProcess.IO = scheduler.get_readyqueue().IO;
                std::cout<< "Process" << runningProcess.ID << "\n";
                for (int i = 1; i < runningProcess.CB + 1; i++) {
                    runningTime ++;
                    if (scheduler.isReady(runningTime)== true) {
                        scheduler.put_readyqueue(scheduler.get_eventqueue());
                    }
                    std::cout<< "Running time:" << runningTime << "   Running Process:" << runningProcess.ID << "\n";
                }
                previousProcess.IO = runningProcess.IO;
                runningProcess.AT = runningProcess.AT + runningProcess.CB + runningProcess.IO;
                scheduler.put_eventqueue(runningProcess);
                runningProcess.ID = 0;
            
            
                for (int i = 1; i < previousProcess.IO + 1; i++) {
                    runningTime ++;
                    if (scheduler.isReady(runningTime)== true) {
                        scheduler.put_readyqueue(scheduler.get_eventqueue());
                    }
                    std::cout << "Block time:" << runningTime << "   Blocked State" << "\n";
                }
            }
        }
        
//        if (runningID != 0) {
//            runningTime++;
//            if (scheduler.isReady(runningTime)== true) {
//                scheduler.put_readyqueue(scheduler.get_eventqueue());
//            }
//            
//        }
//        
       
        
    }

    
    return 0;
}
