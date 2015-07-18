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
#include <vector>

int k = 0;
int frameCount = 0;
int pageTablePosition = 0;

void PageMapping::insertEmptyPage(Instruction instruction, int a){
    pageTable[instruction.virtualPageIndex] = a;
   
}

//FIFO Page Replace Algorithm
unsigned long FIFOMapping::calculatePTE(int a, int b, int c, int d, int e){
    pte = a * pow(2, 31) + b * pow(2, 30) + c * pow(2, 29) + d * pow(2, 28) + e;
    return pte;
};

void FIFOMapping::insertEmptyPage(Instruction instruction, int x){
    pageTable[instruction.virtualPageIndex] = pageTable[instruction.virtualPageIndex] = calculatePTE(1, instruction.operation, 1, 0, x);
    pageTablePosition++;
    k = pageTablePosition;
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

int FIFOMapping::tablePosition(){
    return pageTablePosition;
}

int FIFOMapping::choosePage(int a){
    int page = 0;
    if (k >= a) {
        k -= a;
    }
    page = frameTable[k];
    k++;
    return page;
}

bool FIFOMapping::sameVaildPage(int a, int b, Instruction instruction){
    for (int i = 0; i < b; i++) {
        if (frameTable[i] == instruction.virtualPageIndex) {
            cout<<"==> inst: "<<instruction.operation<<" "<<instruction.virtualPageIndex<< endl;
            if (instruction.operation ==  1) {
                pageTable[instruction.virtualPageIndex] = calculatePTE(presentBit(pageTable[instruction.virtualPageIndex]), 1, referencedBit(pageTable[instruction.virtualPageIndex]), pageoutBit(pageTable[instruction.virtualPageIndex]), physicalFrameNumber(instruction.virtualPageIndex));
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
    pageTable[instruction.virtualPageIndex] = calculatePTE(1, instruction.operation, 1, 0, x);
    pageTablePosition++;
    k = pageTablePosition;
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

int LRUMapping::tablePosition(){
    return pageTablePosition;
}
int LRUMapping::choosePage(int a){
    int useTime = frameTable[0][0];
    int page = 0;
    for(int i = 0; i < a; i++){
        if (frameTable[i][0] <= useTime) {
            useTime = frameTable[i][0];
            page = frameTable[i][1];
        }
    
    }
    return page;
}

bool LRUMapping::sameVaildPage(int a, int b, Instruction instruction){
    for (int i = 0; i < b; i++) {
        if (frameTable[i][1] == instruction.virtualPageIndex) {
            frameTable[i][0] = a;
            cout<<"==> inst: "<<instruction.operation<<" "<<instruction.virtualPageIndex<< endl;
            if (instruction.operation ==  1) {
                pageTable[instruction.virtualPageIndex] = calculatePTE(presentBit(pageTable[instruction.virtualPageIndex]), 1, referencedBit(pageTable[instruction.virtualPageIndex]), pageoutBit(pageTable[instruction.virtualPageIndex]), physicalFrameNumber(instruction.virtualPageIndex));
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
    frameTable[physicalFrameNumber(instruction.virtualPageIndex)][0] = inputLine;
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
        frameTable[physicalFrameNumber(instruction.virtualPageIndex)][0] =inputLine;
        printMap(inputLine, instruction);
    }
}


//Clock Page Replace Algorithm

unsigned long ClockMapping::calculatePTE(int a, int b, int c, int d, int e){
    pte = a * pow(2, 31) + b * pow(2, 30) + c * pow(2, 29) + d * pow(2, 28) + e;
    return pte;
};

void ClockMapping::insertEmptyPage(Instruction instruction, int x){
    pageTable[instruction.virtualPageIndex] = pageTable[instruction.virtualPageIndex] = calculatePTE(1, instruction.operation, 1, 0, x);
    pageTablePosition++;
    k = pageTablePosition;
}

int ClockMapping::presentBit(unsigned long pte){
    int x;
    pte = pte >> 31;
    x = pte & 1;
    return x;
};

int ClockMapping::modifiedBit(unsigned long pte){
    int x;
    pte = pte >> 30;
    x = pte & 1;
    return x;
};

int ClockMapping::referencedBit(unsigned long pte){
    int x;
    pte = pte >> 29;
    x = pte & 1;
    return x;
};

int ClockMapping::pageoutBit(unsigned long pte){
    int x;
    pte = pte >> 28;
    x = pte & 1;
    return x;
};

int ClockMapping::physicalFrameNumber(int a){
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

bool ClockMapping::checkReferred(Instruction instruction){
    if (referencedBit(pageTable[instruction.virtualPageIndex]) != 0) {
        return true;
    }
    else
    {
        return false;
    }
}

void ClockMapping::updateFrameTable(int inputLine, int a, Instruction instruction){
    frameTable[a] = instruction.virtualPageIndex;
//    cout<< "frameNum:"<<a << "  Virtual:"<<frameTable[a] << "  Ref:"<< referencedBit(pageTable[frameTable[a]])<<endl;
}

void ClockMapping::printTable(Instruction instruction, int a){
    cout<<"==> inst: "<<instruction.operation << " "<<instruction.virtualPageIndex<<endl;
    cout<< a << ": ZERO        "<< physicalFrameNumber(instruction.virtualPageIndex)<<endl;
    cout<< a << ": MAP    "<< instruction.virtualPageIndex <<"   "<< physicalFrameNumber(instruction.virtualPageIndex)<<endl;
}

int ClockMapping::tablePosition(){
    return pageTablePosition;
}
int ClockMapping::choosePage(int a){
    int page = 0;
    if (k >= a) {
        k -= a;
    }
    while (referencedBit(pageTable[frameTable[k]]) != 0) {
        pageTable[frameTable[k]] = calculatePTE(presentBit(pageTable[frameTable[k]]), modifiedBit(pageTable[frameTable[k]]), 0, pageoutBit(pageTable[frameTable[k]]), physicalFrameNumber(frameTable[k]));
        k++;
        if (k >= a) {
            k -= a;
        }
    }
    page = frameTable[k];
    k++;
    return page;
}

bool ClockMapping::sameVaildPage(int a, int b, Instruction instruction){
    for (int i = 0; i < b; i++) {
        if (frameTable[i] == instruction.virtualPageIndex) {
            cout<<"==> inst: "<<instruction.operation<<" "<<instruction.virtualPageIndex<< endl;
            pageTable[instruction.virtualPageIndex] = calculatePTE(presentBit(pageTable[instruction.virtualPageIndex]), modifiedBit(pageTable[instruction.virtualPageIndex]), 1, pageoutBit(pageTable[instruction.virtualPageIndex]), physicalFrameNumber(instruction.virtualPageIndex));
            if (instruction.operation ==  1) {
                pageTable[instruction.virtualPageIndex] = calculatePTE(presentBit(pageTable[instruction.virtualPageIndex]), 1, referencedBit(pageTable[instruction.virtualPageIndex]), pageoutBit(pageTable[instruction.virtualPageIndex]), physicalFrameNumber(instruction.virtualPageIndex));
            }
            return true;
        }
    }
//    k--;
    return false;
}

void ClockMapping::outPage(int inputLine,int page, Instruction instruction){
    cout<< inputLine << ": OUT     "<< page << "   "<< physicalFrameNumber(page)<< endl;
    pageTable[instruction.virtualPageIndex] = calculatePTE(1, instruction.operation, 1, pageoutBit(pageTable[instruction.virtualPageIndex]), physicalFrameNumber(page));
    pageTable[page] = calculatePTE(0, modifiedBit(pageTable[page]), referencedBit(pageTable[page]), 1, 0);
    frameTable[physicalFrameNumber(instruction.virtualPageIndex)] = instruction.virtualPageIndex;
}

void ClockMapping::printMap(int inputLine, Instruction instruction){
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
void ClockMapping::replacePage(int inputLine, int oldPage, Instruction instruction){
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

void FIFOMapping::resizeFrameTable(int a){};
void LRUMapping::resizeFrameTable(int a){};
void ClockMapping::resizeFrameTable(int a){};


// Second Chance Algorithm
void SecondChanceMapping::resizeFrameTable(int a){
    frameTable.resize(a);
}

unsigned long SecondChanceMapping::calculatePTE(int a, int b, int c, int d, int e){
    pte = a * pow(2, 31) + b * pow(2, 30) + c * pow(2, 29) + d * pow(2, 28) + e;
    return pte;
};

void SecondChanceMapping::insertEmptyPage(Instruction instruction, int x){
    pageTable[instruction.virtualPageIndex] = pageTable[instruction.virtualPageIndex] = calculatePTE(1, instruction.operation, 1, 0, x);
    pageTablePosition++;
    k = pageTablePosition;
}

int SecondChanceMapping::presentBit(unsigned long pte){
    int x;
    pte = pte >> 31;
    x = pte & 1;
    return x;
};

int SecondChanceMapping::modifiedBit(unsigned long pte){
    int x;
    pte = pte >> 30;
    x = pte & 1;
    return x;
};

int SecondChanceMapping::referencedBit(unsigned long pte){
    int x;
    pte = pte >> 29;
    x = pte & 1;
    return x;
};

int SecondChanceMapping::pageoutBit(unsigned long pte){
    int x;
    pte = pte >> 28;
    x = pte & 1;
    return x;
};

int SecondChanceMapping::physicalFrameNumber(int a){
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

bool SecondChanceMapping::checkReferred(Instruction instruction){
    if (referencedBit(pageTable[instruction.virtualPageIndex]) != 0) {
        return true;
    }
    else
    {
        return false;
    }
}

void SecondChanceMapping::updateFrameTable(int inputLine, int a, Instruction instruction){
    frameTable[a] = instruction.virtualPageIndex;
}

void SecondChanceMapping::printTable(Instruction instruction, int a){
    cout<<"==> inst: "<<instruction.operation << " "<<instruction.virtualPageIndex<<endl;
    cout<< a << ": ZERO        "<< physicalFrameNumber(instruction.virtualPageIndex)<<endl;
    cout<< a << ": MAP    "<< instruction.virtualPageIndex <<"   "<< physicalFrameNumber(instruction.virtualPageIndex)<<endl;
}

int SecondChanceMapping::tablePosition(){
    return pageTablePosition;
}

int SecondChanceMapping::choosePage(int a){
    int page = 0;
    int refBit = 0;
    int tempPage = 0;
    if (k >= a) {
        k -= a;
    }
    tempPage = frameTable.front();
    refBit = referencedBit(pageTable[tempPage]);
    while ( refBit != 0) {
        pageTable[tempPage] = calculatePTE(presentBit(pageTable[tempPage]), modifiedBit(pageTable[tempPage]), 0, pageoutBit(pageTable[tempPage]), physicalFrameNumber(tempPage));
//        cout<< referencedBit(pageTable[tempPage])<<endl;
        frameTable.erase(frameTable.begin());
        frameTable.push_back(tempPage);
//        cout<<"After  "<<referencedBit(pageTable[frameTable.back()])<< endl;
        tempPage = frameTable.front();
        refBit = referencedBit(pageTable[tempPage]);
    }
    page = tempPage;
    return page;
}

bool SecondChanceMapping::sameVaildPage(int a, int b, Instruction instruction){
    for (int i = 0; i < b; i++) {
        if (frameTable[i] == instruction.virtualPageIndex) {
            cout<<"==> inst: "<<instruction.operation<<" "<<instruction.virtualPageIndex<< endl;
            pageTable[instruction.virtualPageIndex] = calculatePTE(presentBit(pageTable[instruction.virtualPageIndex]), modifiedBit(pageTable[instruction.virtualPageIndex]), 1, pageoutBit(pageTable[instruction.virtualPageIndex]), physicalFrameNumber(instruction.virtualPageIndex));
            if (instruction.operation ==  1) {
                pageTable[instruction.virtualPageIndex] = calculatePTE(presentBit(pageTable[instruction.virtualPageIndex]), 1, referencedBit(pageTable[instruction.virtualPageIndex]), pageoutBit(pageTable[instruction.virtualPageIndex]), physicalFrameNumber(instruction.virtualPageIndex));
            }
            return true;
        }
    }
    
    for (int i = 0; i < 8; i++) {
        cout<< frameTable[i]<<"  "<< referencedBit(pageTable[frameTable[i]])<<endl;
    }
    return false;
}

void SecondChanceMapping::outPage(int inputLine,int page, Instruction instruction){
    cout<< inputLine << ": OUT     "<< page << "   "<< physicalFrameNumber(page)<< endl;
    pageTable[instruction.virtualPageIndex] = calculatePTE(1, instruction.operation, 1, pageoutBit(pageTable[instruction.virtualPageIndex]), physicalFrameNumber(page));
    pageTable[page] = calculatePTE(0, modifiedBit(pageTable[page]), referencedBit(pageTable[page]), 1, 0);
    frameTable.front() = instruction.virtualPageIndex;
}

void SecondChanceMapping::printMap(int inputLine, Instruction instruction){
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
void SecondChanceMapping::replacePage(int inputLine, int oldPage, Instruction instruction){
    cout<< "==> inst: " << instruction.operation << " "<<instruction.virtualPageIndex <<endl;
    cout<< inputLine << ": UNMAP   "<< oldPage << "   "<< physicalFrameNumber(oldPage)<< endl;
    if (modifiedBit(pageTable[oldPage]) == 1) {
        outPage(inputLine, oldPage, instruction);
        printMap(inputLine, instruction);
    } else{
        pageTable[instruction.virtualPageIndex] = calculatePTE(1, instruction.operation, 1, pageoutBit(pageTable[instruction.virtualPageIndex]), physicalFrameNumber(oldPage));
        pageTable[oldPage] = calculatePTE(0, modifiedBit(pageTable[oldPage]), referencedBit(pageTable[oldPage]), pageoutBit(pageTable[oldPage]), 0);
        frameTable.front() = instruction.virtualPageIndex;
        printMap(inputLine, instruction);
    }
}

