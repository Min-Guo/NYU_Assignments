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
#include <math.h>
using namespace std;

string lineBuffer;
char* line;
char* token;
const char* file;
int physicalFrameNumber = 32;
unsigned long temppte;
int pageIndex;
int i = 0;
int insertState = false;
int pageReplace = 0;

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
    int j = 0;
    pageMapping = new SecondChanceMapping();
    pageMapping->resizeFrameTable(physicalFrameNumber);
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
                if (strcmp(line, "") != 0) {
                    token = strtok(line, "\n");
                    token = strtok(line, " ");
                    while (token!= NULL) {
                        if (tempInstruction.operationState == false) {
                            tempInstruction.operation = atoi(token);
                            tempInstruction.operationState = true;
                            pageMapping->calculatePTE(1, tempInstruction.operation, 0, 0, 0);
                            temppte = pageMapping->calculatePTE(1, tempInstruction.operation, 0, 0, 0);
                        }
                        else
                        {
                            tempInstruction.virtualPageIndex = atoi(token);
                        }
                        token = strtok(NULL, " ");
                    }
                    i = pageMapping->tablePosition();
                    pageMapping->readRfile("/Users/Min/Development/NYU_Assignments/OS/Lab#3/lab3_assign/rfile");
                    if (i < physicalFrameNumber) {
                        if(pageMapping->sameVaildPage(j, i, tempInstruction, 0) == false){
                            pageMapping->insertEmptyPage(tempInstruction, i);
                            pageMapping->updateFrameTable(j, i, tempInstruction);
                            pageMapping->printTable(tempInstruction, j);
                        }
                    } else {
                        if ((pageMapping->sameVaildPage(j, physicalFrameNumber, tempInstruction, 1) == false)) {
                            pageReplace++;
                            pageMapping->insertClass();
                            pageIndex = pageMapping->choosePage(physicalFrameNumber);
                            if (pageReplace == 10) {
                                pageMapping->resetRef();
                                pageReplace = 0;
                            }
                            pageMapping->replacePage(j, pageIndex, tempInstruction);
                        }
                    }
                
                    resetTempIns();
                    j++;
                }
            }
        }
        pageMapping->pageTableOPtion();
        pageMapping->printFrameMap(physicalFrameNumber);
        pageMapping->printSummary(j);
    
    }
    infile.close();
    return 0;
};


int main(int argc, const char * argv[]) {
    //    argv[1] = "/Users/Min/Development/NYU_Assignments/OS/Lab#3/lab3_assign/in1K4";
    //    frameNumber = atoi(argv[1]);
    
    readFile("/Users/Min/Development/NYU_Assignments/OS/Lab#3/lab3_assign/in1M2");
    return 0;
}
