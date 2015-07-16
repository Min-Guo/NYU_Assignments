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
#include <array>

int k = 0;

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

int FIFOMapping::physicalFrameNumber(int a){
    int i = 6;
    unsigned long x;
    int y;
    PhyNumber = 0;
    while (i > 0) {
        x = pageTable[a];
        x = x >> (i - 1);
        y = x & 1;
        PhyNumber += y * pow(2, i - 1);
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

void FIFOMapping::updateFrameTable(int inputLine, int a, Instruction instruction){
    frameTable[a] = instruction.virtualPageIndex;
}

void FIFOMapping::printTable(Instruction instruction, int a){
    cout<<"==> inst: "<<instruction.operation << " "<<instruction.virtualPageIndex<<endl;
    cout<< a << ": ZERO        "<< physicalFrameNumber(instruction.virtualPageIndex)<<endl;
    cout<< a << ": MAP    "<< instruction.virtualPageIndex <<"   "<< physicalFrameNumber(instruction.virtualPageIndex)<<endl;
}

int FIFOMapping::choosePage(int a){
    int page;
    if (k >= a) {
        k = k - a;
    }
    page = frameTable[k];
    k++;
    return page;
}

bool FIFOMapping::sameVaildPage(int inputLine, int page, Instruction instruction){
    for (int i = 0; i < 16; i++) {
        if (frameTable[i] == page) {
            if (instruction.operation ==  1) {
                pageTable[page] = calculatePTE(presentBit(pageTable[page]), 1, referencedBit(pageTable[page]), pageoutBit(pageTable[page]), physicalFrameNumber(page));
            }
            return true;
        }
    }
    return false;
}

void FIFOMapping::outPage(int inputLine,int page, Instruction instruction){
    cout<< inputLine << ": OUT     "<< page << "   "<< physicalFrameNumber(page)<< endl;
    pageTable[instruction.virtualPageIndex] = calculatePTE(1, instruction.operation, 1, pageoutBit(pageTable[instruction.virtualPageIndex]), physicalFrameNumber(page));
    pageTable[page] = calculatePTE(0, modifiedBit(pageTable[page]), referencedBit(pageTable[page]), 1, 0);
    frameTable[physicalFrameNumber(instruction.virtualPageIndex)] = instruction.virtualPageIndex;
}


void FIFOMapping::printMap(int inputLine, Instruction instruction){
    if (referencedBit(pageTable[instruction.virtualPageIndex]) == 0 && modifiedBit(pageTable[instruction.virtualPageIndex]) == 0){
        cout<< inputLine << ": ZERO        "<< physicalFrameNumber(instruction.virtualPageIndex) << endl;
        cout<< inputLine << ": MAP    "<< instruction.virtualPageIndex << "   "<< physicalFrameNumber(instruction.virtualPageIndex)<< endl;
    } else if (pageoutBit(pageTable[instruction.virtualPageIndex]) == 0) {
        cout<< inputLine << ": ZERO        "<< physicalFrameNumber(instruction.virtualPageIndex) << endl;
        cout<< inputLine << ": MAP    "<< instruction.virtualPageIndex << "   "<< physicalFrameNumber(instruction.virtualPageIndex)<< endl;
    } else {
        cout<< inputLine << ": IN      "<< instruction.virtualPageIndex <<"   " << physicalFrameNumber(instruction.virtualPageIndex) << endl;
        cout<< inputLine << ": MAP    "<< instruction.virtualPageIndex << "   "<< physicalFrameNumber(instruction.virtualPageIndex)<< endl;
    }
    
}
void FIFOMapping::replacePage(int inputLine, int oldPage, Instruction instruction){
    cout<< "==> inst: " << instruction.operation << " "<<instruction.virtualPageIndex <<endl;
    cout<< inputLine << ": UNMAP   "<< oldPage << "   "<< physicalFrameNumber(oldPage)<< endl;
    if (modifiedBit(pageTable[oldPage]) == 1) {
        outPage(inputLine, oldPage, instruction);
        printMap(inputLine, instruction);
    } else{
        pageTable[instruction.virtualPageIndex] = calculatePTE(1, instruction.operation, 1, pageoutBit(pageTable[instruction.virtualPageIndex]), physicalFrameNumber(oldPage));
        pageTable[oldPage] = calculatePTE(0, modifiedBit(pageTable[oldPage]), referencedBit(pageTable[oldPage]), pageoutBit(pageTable[oldPage]), 0);
        frameTable[physicalFrameNumber(instruction.virtualPageIndex)] = instruction.virtualPageIndex;
        printMap(inputLine, instruction);
    }
}

//LRU Page Replace Algorithm

unsigned long LRUMapping::calculatePTE(int a, int b, int c, int d, int e){
    pte = a * pow(2, 31) + b * pow(2, 30) + c * pow(2, 29) + d * pow(2, 28) + e;
    return pte;
};

void LRUMapping::insertEmptyPage(Instruction instruction, int x){
    pte = pte + x;
    pageTable[instruction.virtualPageIndex] = pte;
}





int LRUMapping::presentBit(unsigned long pte){
    int x;
    pte = pte >> 31;
    x = pte & 1;
    return x;
};

int LRUMapping::modifiedBit(unsigned long pte){
    int x;
    pte = pte >> 30;
    x = pte & 1;
    return x;
};

int LRUMapping::referencedBit(unsigned long pte){
    int x;
    pte = pte >> 29;
    x = pte & 1;
    return x;
};

int LRUMapping::pageoutBit(unsigned long pte){
    int x;
    pte = pte >> 28;
    x = pte & 1;
    return x;
};

int LRUMapping::physicalFrameNumber(int a){
    int i = 6;
    unsigned long x;
    int y;
    PhyNumber = 0;
    while (i > 0) {
        x = pageTable[a];
        x = x >> (i - 1);
        y = x & 1;
        PhyNumber += y * pow(2, i - 1);
        i--;
    }
    return PhyNumber;
};


bool LRUMapping::checkReferred(Instruction instruction){
    if (referencedBit(pageTable[instruction.virtualPageIndex]) != 0) {
        return true;
    }
    else
    {
        return false;
    }
}

void LRUMapping::updateFrameTable(int inputLine, int a, Instruction instruction){
    frameTable[a][0] = inputLine;
    frameTable[a][1] = instruction.virtualPageIndex;
}

void LRUMapping::printTable(Instruction instruction, int a){
    cout<<"==> inst: "<<instruction.operation << " "<<instruction.virtualPageIndex<<endl;
    cout<< a << ": ZERO        "<< physicalFrameNumber(instruction.virtualPageIndex)<<endl;
    cout<< a << ": MAP    "<< instruction.virtualPageIndex <<"   "<< physicalFrameNumber(instruction.virtualPageIndex)<<endl;
}

int LRUMapping::choosePage(int a){
    int useTime = frameTable[0][0];
    int page = 0;
    for(int i = 0; i < 16; i++){
        if (frameTable[i][0] <= useTime) {
            useTime = frameTable[i][0];
            page = frameTable[i][1];
        }
        
    }
    return page;
}

bool LRUMapping::sameVaildPage(int inputLine, int page, Instruction instruction){
    for (int i = 0; i < 16; i++) {
        if (frameTable[i][1] == page) {
            frameTable[physicalFrameNumber(instruction.virtualPageIndex)][0] =  inputLine;
            if (instruction.operation ==  1) {
                pageTable[page] = calculatePTE(presentBit(pageTable[page]), 1, referencedBit(pageTable[page]), pageoutBit(pageTable[page]), physicalFrameNumber(page));
            }
            return true;
        }
    }
    return false;
}

void LRUMapping::outPage(int inputLine,int page, Instruction instruction){
    cout<< inputLine << ": OUT     "<< page << "   "<< physicalFrameNumber(page)<< endl;
    pageTable[instruction.virtualPageIndex] = calculatePTE(1, instruction.operation, 1, pageoutBit(pageTable[instruction.virtualPageIndex]), physicalFrameNumber(page));
    pageTable[page] = calculatePTE(0, modifiedBit(pageTable[page]), referencedBit(pageTable[page]), 1, 0);
    frameTable[physicalFrameNumber(instruction.virtualPageIndex)][1] = instruction.virtualPageIndex;
    frameTable[physicalFrameNumber(instruction.virtualPageIndex)][0] =  inputLine;
}


void LRUMapping::printMap(int inputLine, Instruction instruction){
    if (referencedBit(pageTable[instruction.virtualPageIndex]) == 0 && modifiedBit(pageTable[instruction.virtualPageIndex]) == 0){
        cout<< inputLine << ": ZERO        "<< physicalFrameNumber(instruction.virtualPageIndex) << endl;
        cout<< inputLine << ": MAP    "<< instruction.virtualPageIndex << "   "<< physicalFrameNumber(instruction.virtualPageIndex)<< endl;
    } else if (pageoutBit(pageTable[instruction.virtualPageIndex]) == 0) {
        cout<< inputLine << ": ZERO        "<< physicalFrameNumber(instruction.virtualPageIndex) << endl;
        cout<< inputLine << ": MAP    "<< instruction.virtualPageIndex << "   "<< physicalFrameNumber(instruction.virtualPageIndex)<< endl;
    } else {
        cout<< inputLine << ": IN      "<< instruction.virtualPageIndex <<"   " << physicalFrameNumber(instruction.virtualPageIndex) << endl;
        cout<< inputLine << ": MAP    "<< instruction.virtualPageIndex << "   "<< physicalFrameNumber(instruction.virtualPageIndex)<< endl;
    }
    
}
void LRUMapping::replacePage(int inputLine, int oldPage, Instruction instruction){
    cout<< "==> inst: " << instruction.operation << " "<<instruction.virtualPageIndex <<endl;
    cout<< inputLine << ": UNMAP   "<< oldPage << "   "<< physicalFrameNumber(oldPage)<< endl;
    if (modifiedBit(pageTable[oldPage]) == 1) {
        outPage(inputLine, oldPage, instruction);
        printMap(inputLine, instruction);
    } else{
        pageTable[instruction.virtualPageIndex] = calculatePTE(1, instruction.operation, 1, pageoutBit(pageTable[instruction.virtualPageIndex]), physicalFrameNumber(oldPage));
        pageTable[oldPage] = calculatePTE(0, modifiedBit(pageTable[oldPage]), referencedBit(pageTable[oldPage]), pageoutBit(pageTable[oldPage]), 0);
        frameTable[physicalFrameNumber(instruction.virtualPageIndex)][1] = instruction.virtualPageIndex;
        frameTable[physicalFrameNumber(instruction.virtualPageIndex)][0] =  inputLine;
        printMap(inputLine, instruction);
    }
}

