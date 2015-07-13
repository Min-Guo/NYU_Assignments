//
//  main.cpp
//  lab3
//
//  Created by Min Guo on 7/11/15.
//  Copyright (c) 2015 Min Guo. All rights reserved.
//

#include <iostream>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <vector>
#include "VMM.h"
using namespace std;

string lineBuffer;
char* line;
char* token;
const char* file;
const int* frameNumber;
PageMapping* pageMapping;
Instruction tempInstruction = {
    0,
    false,
    0,
    false,
};



void resetTempIns(){
    tempInstruction.operationState = false;
    tempInstruction.operation = 0;
    tempInstruction.virtualPageIndex = 0;
    tempInstruction.virtualPageState = false;
};



int readFile(const char* file){
    int i = 0;
    pageMapping = new FIFOMapping();
    ifstream infile(file);
    if(!infile.is_open()){
        cout<<"Failed to open"<<endl;
    }
    else{
        while (!infile.eof()) {
            getline(infile, lineBuffer);
            if (lineBuffer.find("#") != 0) {
                line = new char[lineBuffer.length() + 1];
                strcpy(line, lineBuffer.c_str());
                token = strtok(line, "\n");
                token = strtok(line, " ");
                while (token!= NULL) {
                    if (tempInstruction.operationState == false) {
                        tempInstruction.operation = atoi(token);
                        tempInstruction.operationState = true;
                    }
                    else
                    {
                        tempInstruction.virtualPageIndex = atoi(token);
                    }
                    token = strtok(NULL, " ");
                }
                cout<<i<< " "<<tempInstruction.operation<<" "<<tempInstruction.virtualPageIndex<< endl;
                pageMapping->insertEmptyPage(tempInstruction, i);
                resetTempIns();
                i++;
            }
        }
    }

    return 0;
};


int main(int argc, const char * argv[]) {
    argv[1] = "/Users/Min/Development/NYU_Assignments/OS/Lab#3/lab3_assign/in1K4";
    readFile(argv[1]);
    return 0;
}
