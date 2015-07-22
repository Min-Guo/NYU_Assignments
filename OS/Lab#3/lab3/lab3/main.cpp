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
#include <ctype.h>
#include <unistd.h>
using namespace std;

string lineBuffer;
char* line;
char* token;
const char* file;
int physicalFrameNumber;
unsigned long temppte;
int pageIndex;
int i = 0;
int insertState = false;
int pageReplace = 0;
const char* rfile;
char* aValue = NULL;
char* oValue = NULL;
char* fValue = NULL;
int c;
int Oflag;
int Pflag;
int Fflag;
int Sflag;
char* tempA;
char* tempF;
int tempOldPhy;

PageMapping* pageMapping;
Instruction tempInstruction = {
    0,
    false,
    0,
    false,
};

int searchOperation(const char* arg2){
    string tempO(arg2);
    for (int i = 2; i < tempO.length(); i++) {
        if (tempO[i] == 'O') {
            Oflag = 1;
        }
        if (tempO[i] == 'P') {
            Pflag = 1;
        }
        if (tempO[i] == 'F') {
            Fflag = 1;
        }
        if (tempO[i] == 'S') {
            Sflag = 1;
        }
    }
    
    
    return 0;
};

int searchOperation(){
    string tempO(oValue);
    for (int i = 0; i < tempO.length(); i++) {
        if (tempO[i] == 'O') {
            Oflag = 1;
        }
        if (tempO[i] == 'P') {
            Pflag = 1;
        }
        if (tempO[i] == 'F') {
            Fflag = 1;
        }
        if (tempO[i] == 'S') {
            Sflag = 1;
        }
    }
    
    
    return 0;
};
void resetTempIns(){
    tempInstruction.operationState = false;
    tempInstruction.operation = 0;
    tempInstruction.virtualPageIndex = 0;
    tempInstruction.virtualPageState = false;
};

int readFile(const char* file, const char* rfile){
    int j = 0;
//    if (strcmp(aValue, "a") == 0) {
//        pageMapping = new AgingLocalMapping();
//    }
    if (strcmp(aValue, "f") == 0) {
        pageMapping = new FIFOMapping();
    }
    if (strcmp(aValue, "l") == 0) {
        pageMapping = new LRUMapping();
    }
//    if (strcmp(aValue, "s") == 0) {
//        pageMapping = new SecondChanceMapping();
//    }
//    if (strcmp(aValue, "r") == 0) {
//        pageMapping = new RandomMapping();
//    }
//    if (strcmp(aValue, "N") == 0) {
//        pageMapping = new NRUMapping();
//    }
    if (strcmp(aValue, "c") == 0) {
        pageMapping = new ClockMapping();
    }
//    if (strcmp(aValue, "X") == 0) {
//        pageMapping = new ClockGlobalMapping();
//    }
//    if (strcmp(aValue, "Y") == 0) {
//        pageMapping = new AgingMapping();
//    }
    physicalFrameNumber = atoi(fValue);
    pageMapping->resizeFrameTable(physicalFrameNumber);
    pageMapping->readRfile(rfile);
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
                    if (i < physicalFrameNumber) {
                        if(pageMapping->sameVaildPage(j, i, tempInstruction, 0) == false){
                            pageMapping->insertEmptyPage(tempInstruction, i);
                            pageMapping->updateFrameTable(j, i, tempInstruction);
                            tempOldPhy = pageMapping->physicalFrameNumber(pageIndex);
                            if (Oflag == 1) {
                                pageMapping->printTable(tempInstruction, j, 0);
                            }
                        } else{
                            if (Oflag == 1) {
                                pageMapping->printTable(tempInstruction, j, 1);
                            }
                        }
                    } else {
                        if ((pageMapping->sameVaildPage(j, physicalFrameNumber, tempInstruction, 1) == false)) {
                            pageReplace++;
                            pageMapping->insertClass();
                            pageIndex = pageMapping->choosePage(physicalFrameNumber);
                            tempOldPhy = pageMapping->physicalFrameNumber(pageIndex);
                            if (pageReplace == 10) {
                                pageMapping->resetRef();
                                pageReplace = 0;
                            }
                            pageMapping->replacePage(j, pageIndex, tempInstruction, physicalFrameNumber);
                            if (Oflag == 1) {
                                pageMapping->printMap(j, tempInstruction, pageIndex, tempOldPhy, 0);
                            }
                        } else {
                            if (Oflag == 1) {
                                pageMapping->printMap(j, tempInstruction, pageIndex, tempOldPhy, 1);
                            }
                        }
                    }
                
                    resetTempIns();
                    j++;
                }
            }
        }
        if (Pflag == 1) {
            pageMapping->pageTableOPtion();
        }
        if (Fflag == 1) {
            pageMapping->printFrameMap(physicalFrameNumber);
        }
        if (Sflag == 1) {
            pageMapping->printSummary(j);
        }
    }
    infile.close();
    return 0;
};


int main(int argc, char** argv) {
    while ((c = getopt (argc, argv, "a::o::f::")) != -1) {
        switch (c) {
            case 'a':
                aValue = optarg;
                break;
            case 'o':
                oValue = optarg;
                break;
            case 'f':
                fValue = optarg;
            default:
                break;
        }
    }
    searchOperation();
    readFile(argv[4], argv[5]);
    return 0;
}
