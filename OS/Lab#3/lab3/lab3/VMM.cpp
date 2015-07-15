//
//  VMM.cpp
//  lab3
//
//  Created by Min Guo on 7/12/15.
//  Copyright (c) 2015 Min Guo. All rights reserved.
//

#include "VMM.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <math.h>

void PageMapping::insertEmptyPage(Instruction instruction, int a){
    pageTable[instruction.virtualPageIndex] = a;
}

unsigned long FIFOMapping::calculatePTE(int a, int b, int c, int d, int e){
    pte = a * pow(2, 31) + b * pow(2, 30) + c * pow(2, 29) + d * pow(2, 28) + e;
    return pte;
};

void FIFOMapping::insertEmptyPage(Instruction instruction, int x){
    pte = pte + x;
    pageTable[instruction.virtualPageIndex] = pte;
}



int FIFOMapping::presentBit(unsigned long pte){
    int x;
    pte = pte >> 31;
    x = pte & 1;
    return x;
};

int FIFOMapping::modifiedBit(unsigned long pte){
    int x;
    pte = pte >> 30;
    x = pte & 1;
    return x;
};

int FIFOMapping::referencedBit(unsigned long pte){
    int x;
    pte = pte >> 29;
    x = pte & 1;
    return x;
};

int FIFOMapping::pageoutBit(unsigned long pte){
    int x;
    pte = pte >> 28;
    x = pte & 1;
    return x;
};

unsigned long FIFOMapping::physicalFrameNumber(Instruction instruction){
    int i = 6;
    unsigned long x;
    PhyNumber = 0;
    while (i > 0) {
        x = pageTable[instruction.virtualPageIndex];
        x = x >> (i - 1);
        x = x & 1;
        PhyNumber += x * pow(2, i - 1);
        i--;
    }
    return PhyNumber;
};


bool FIFOMapping::checkReferred(Instruction instruction){
    if (referencedBit(pageTable[instruction.virtualPageIndex]) != 0) {
        return true;
    }
    else
    {
        return false;
    }
}

void FIFOMapping::updateFrameTable(int a, Instruction instruction){
    frameTable[a] = instruction.virtualPageIndex;
}

void FIFOMapping::printTable(Instruction instruction, int a){
    cout<<"==> inst: "<<instruction.operation << " "<<instruction.virtualPageIndex<<endl;
    cout<< a << ": ZERO        "<< physicalFrameNumber(instruction)<<endl;
    cout<< a << ": MAP     "<< instruction.virtualPageIndex <<"   "<< physicalFrameNumber(instruction)<<endl;
}

int FIFOMapping::choosePage(int a){
    return frameTable[a];
}

void FIFOMapping::replacePage(int a){
    
}