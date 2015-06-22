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




FILE *file;
char line_buffer[512];
char *token;
int i = 0;

struct inputInformation{
    int AT; /* arriving time*/
    bool atRead; /* if AT has been read from input file, it is true*/
    int TC; /* Total CPU time */
    bool tcRead;
    int CB; /* CPU burst*/
    bool cbRead;
    int IO; /* IO burst*/
    bool ioRead;
};
struct inputInformation inputInformations[512]; /* Struct to store 4 parameters */


class CompareAT {

};

int parse(FILE *file){
    while (!feof(file)) {
        if (fgets(line_buffer, 512, file)!= NULL) {
            if (strcmp(line_buffer, "\n") != 0) {   /* read input file line by line */
                token = strtok(line_buffer, "\n");
                token = strtok(line_buffer, " ");
                
                while (token!= NULL) {
                    if (inputInformations[i].atRead == false) {
                        inputInformations[i].AT = atoi(token);
                        inputInformations[i].atRead = true;
                    } else if (inputInformations[i].tcRead == false){
                        inputInformations[i].TC = atoi(token);
                        inputInformations[i].tcRead = true;
                    } else if (inputInformations[i].cbRead == false){
                        inputInformations[i].CB = atoi(token);
                        inputInformations[i].cbRead = true;
                    } else if (inputInformations[i].ioRead == false){
                        inputInformations[i].IO = atoi(token);
                        inputInformations[i].ioRead = true;
                    }
                    token = strtok(NULL, " ");

                }
                i++;
                            }
        }
    }
    return 0;
};

int printFunction(){
    printf("AT TC CB IO\n");
    for (int j = 0; j < i; j++) {
        printf("%i %i %i %i\n", inputInformations[j].AT, inputInformations[j].TC, inputInformations[j].CB, inputInformations[j].IO);
    }
    return 0;
};

int main(int argc, const char * argv[]) {
    file = fopen("/Users/Min/Development/NYU_Assignments/OS/Lab#2_total/lab2_assign/input0", "r");
    parse(file);
    fclose(file);
    printFunction();
    return 0;
}
