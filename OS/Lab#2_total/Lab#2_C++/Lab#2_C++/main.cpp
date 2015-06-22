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









int parse(FILE *file, Scheduler * scheduler){

    
    while (!feof(file)) {
        if (fgets(line_buffer, 512, file)!= NULL) {
            Process process = {0, false, 0, false, 0, false, 0, false};
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
                
                scheduler->put_event(process);
   
                
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
    
    std::cout << "myqueue contains:" << scheduler.get_event().AT;

    
    return 0;
}
