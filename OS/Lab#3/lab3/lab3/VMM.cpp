//
//  VMM.cpp
//  lab3
//
//  Created by Min Guo on 7/12/15.
//  Copyright (c) 2015 Min Guo. All rights reserved.
//

#include "VMM.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include <array>
#include <vector>
#include <fstream>
int k = 0;
int frameCount = 0;
int pageTablePosition = 0;
unsigned int rNum = 0;
char* buffer1;
int nruNum = 0;
unsigned int nruRand = 0;
int nruClassIndex = 0;
int classType = 0;
int j = 0;
int pageCount = 0;
int frameIndex = 0;
int startPoint = 0;

void PageMapping::insertEmptyPage(Instruction instruction, int a){
    pageTable[instruction.virtualPageIndex] = a;
   
}

//FIFO Page Replace Algorithm
void FIFOMapping::resizeFrameTable(int a){};
void FIFOMapping::insertClass(){};
void FIFOMapping::resetRef(){};
void FIFOMapping::readRfile(const char*rfile){};
void FIFOMapping::clearClass(){};
int FIFOMapping::checkClass(int refernced, int modified){return 0;};
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

void FIFOMapping::pageTableOPtion(){
    for (int i = 0; i < 64; i++) {
        if (presentBit(pageTable[i]) == 0 && pageoutBit(pageTable[i]) != 1) {
            cout << "* ";
        }else if (presentBit(pageTable[i]) == 0 && pageoutBit(pageTable[i]) == 1) {
            cout << "# ";
        }else {
            if (referencedBit(pageTable[i]) == 1) {
                cout << " "<< i << ":R";
            } else{
                cout << " "<< i << ":-";
            }
            if (modifiedBit(pageTable[i]) == 1) {
                cout << "M";
            }else{
                cout << "-";
            }
            if (pageoutBit(pageTable[i]) == 1) {
                cout << "S ";
            }else{
                cout << "- ";
            }
        }
    }
    cout <<"\n";
}


//LRU Page Replace Algorithm

void LRUMapping::readRfile(const char*rfile){};
void LRUMapping::resizeFrameTable(int a){};
void LRUMapping::insertClass(){};
void LRUMapping::resetRef(){};
void LRUMapping::clearClass(){};
int LRUMapping::checkClass(int refernced, int modified){return 0;};

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

void LRUMapping::pageTableOPtion(){
    for (int i = 0; i < 64; i++) {
        if (presentBit(pageTable[i]) == 0 && pageoutBit(pageTable[i]) != 1) {
            cout << "* ";
        }else if (presentBit(pageTable[i]) == 0 && pageoutBit(pageTable[i]) == 1) {
            cout << "# ";
        }else {
            if (referencedBit(pageTable[i]) == 1) {
                cout << " "<< i << ":R";
            } else{
                cout << " "<< i << ":-";
            }
            if (modifiedBit(pageTable[i]) == 1) {
                cout << "M";
            }else{
                cout << "-";
            }
            if (pageoutBit(pageTable[i]) == 1) {
                cout << "S ";
            }else{
                cout << "- ";
            }
        }
    }
    cout <<"\n";
}


//Clock Page Replace Algorithm

void ClockMapping::readRfile(const char*rfile){};
void ClockMapping::resizeFrameTable(int a){};
void ClockMapping::insertClass(){};
void ClockMapping::resetRef(){};
void ClockMapping::clearClass(){};
int ClockMapping::checkClass(int refernced, int modified){return 0;};


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
void ClockMapping::pageTableOPtion(){
    for (int i = 0; i < 64; i++) {
        if (presentBit(pageTable[i]) == 0 && pageoutBit(pageTable[i]) != 1) {
            cout << "* ";
        }else if (presentBit(pageTable[i]) == 0 && pageoutBit(pageTable[i]) == 1) {
            cout << "# ";
        }else {
            if (referencedBit(pageTable[i]) == 1) {
                cout << " "<< i << ":R";
            } else{
                cout << " "<< i << ":-";
            }
            if (modifiedBit(pageTable[i]) == 1) {
                cout << "M";
            }else{
                cout << "-";
            }
            if (pageoutBit(pageTable[i]) == 1) {
                cout << "S ";
            }else{
                cout << "- ";
            }
        }
    }
    cout <<"\n";
}


// Clock Algorithm with Global Policy
void ClockGlobalMapping::resizeFrameTable(int a){};
void ClockGlobalMapping::readRfile(const char*rfile){};
void ClockGlobalMapping::insertClass(){};
void ClockGlobalMapping::resetRef(){};
void ClockGlobalMapping::clearClass(){};
int ClockGlobalMapping::checkClass(int refernced, int modified){return 0;};


unsigned long ClockGlobalMapping::calculatePTE(int a, int b, int c, int d, int e){
    pte = a * pow(2, 31) + b * pow(2, 30) + c * pow(2, 29) + d * pow(2, 28) + e;
    return pte;
};

void ClockGlobalMapping::insertEmptyPage(Instruction instruction, int x){
    pageTable[instruction.virtualPageIndex] = pageTable[instruction.virtualPageIndex] = calculatePTE(1, instruction.operation, 1, 0, x);
    pageTablePosition++;
    k = pageTablePosition;
}

int ClockGlobalMapping::presentBit(unsigned long pte){
    int x;
    pte = pte >> 31;
    x = pte & 1;
    return x;
};

int ClockGlobalMapping::modifiedBit(unsigned long pte){
    int x;
    pte = pte >> 30;
    x = pte & 1;
    return x;
};

int ClockGlobalMapping::referencedBit(unsigned long pte){
    int x;
    pte = pte >> 29;
    x = pte & 1;
    return x;
};

int ClockGlobalMapping::pageoutBit(unsigned long pte){
    int x;
    pte = pte >> 28;
    x = pte & 1;
    return x;
};

int ClockGlobalMapping::physicalFrameNumber(int a){
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

bool ClockGlobalMapping::checkReferred(Instruction instruction){
    if (referencedBit(pageTable[instruction.virtualPageIndex]) != 0) {
        return true;
    }
    else
    {
        return false;
    }
}

void ClockGlobalMapping::updateFrameTable(int inputLine, int a, Instruction instruction){
    frameTable[a] = instruction.virtualPageIndex;
    //    cout<< "frameNum:"<<a << "  Virtual:"<<frameTable[a] << "  Ref:"<< referencedBit(pageTable[frameTable[a]])<<endl;
}

void ClockGlobalMapping::printTable(Instruction instruction, int a){
    cout<<"==> inst: "<<instruction.operation << " "<<instruction.virtualPageIndex<<endl;
    cout<< a << ": ZERO        "<< physicalFrameNumber(instruction.virtualPageIndex)<<endl;
    cout<< a << ": MAP    "<< instruction.virtualPageIndex <<"   "<< physicalFrameNumber(instruction.virtualPageIndex)<<endl;
}

int ClockGlobalMapping::tablePosition(){
    return pageTablePosition;
}

int ClockGlobalMapping::choosePage(int a){
    int page = 0;
    if (pageCount >= 64) {
        pageCount -= 64;
    }
    
    while (presentBit(pageTable[pageCount]) != 1 || referencedBit(pageTable[pageCount]) != 0) {
        if (presentBit(pageTable[pageCount]) != 1) {
            pageCount++;
            if (pageCount >= 64) {
                pageCount -= 64;
            }
        }
        if (presentBit(pageTable[pageCount]) == 1 && referencedBit(pageTable[pageCount]) != 0) {
            pageTable[pageCount] = calculatePTE(presentBit(pageTable[pageCount]), modifiedBit(pageTable[pageCount]), 0, pageoutBit(pageTable[pageCount]), physicalFrameNumber(pageCount));
            pageCount++;
            if (pageCount >= 64) {
                pageCount -= 64;
            }
        }
    }
    page = pageCount;
    pageCount++;
    return page;
    
}

bool ClockGlobalMapping::sameVaildPage(int a, int b, Instruction instruction){
    for (int i = 0; i < b; i++) {
        if (frameTable[i] == instruction.virtualPageIndex) {
            cout<<"==> inst: "<<instruction.operation<<" "<<instruction.virtualPageIndex<< endl;
            pageTable[instruction.virtualPageIndex] = calculatePTE(presentBit(pageTable[instruction.virtualPageIndex]), modifiedBit(pageTable[instruction.virtualPageIndex]), 1, pageoutBit(pageTable[instruction.virtualPageIndex]), physicalFrameNumber(instruction.virtualPageIndex));
            if (instruction.operation ==  1) {
                pageTable[instruction.virtualPageIndex] = calculatePTE(presentBit(pageTable[instruction.virtualPageIndex]), 1, referencedBit(pageTable[instruction.virtualPageIndex]), pageoutBit(pageTable[instruction.virtualPageIndex]), physicalFrameNumber(instruction.virtualPageIndex));
            }
            pageTableOPtion();
            return true;
        }
    }
    return false;
}

void ClockGlobalMapping::outPage(int inputLine,int page, Instruction instruction){
    cout<< inputLine << ": OUT     "<< page << "   "<< physicalFrameNumber(page)<< endl;
    pageTable[instruction.virtualPageIndex] = calculatePTE(1, instruction.operation, 1, pageoutBit(pageTable[instruction.virtualPageIndex]), physicalFrameNumber(page));
    pageTable[page] = calculatePTE(0, modifiedBit(pageTable[page]), referencedBit(pageTable[page]), 1, 0);
    frameTable[physicalFrameNumber(instruction.virtualPageIndex)] = instruction.virtualPageIndex;
}

void ClockGlobalMapping::printMap(int inputLine, Instruction instruction){
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
void ClockGlobalMapping::replacePage(int inputLine, int oldPage, Instruction instruction){
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

void ClockGlobalMapping::pageTableOPtion(){
    for (int i = 0; i < 64; i++) {
        if (presentBit(pageTable[i]) == 0 && pageoutBit(pageTable[i]) != 1) {
            cout << "* ";
        }else if (presentBit(pageTable[i]) == 0 && pageoutBit(pageTable[i]) == 1) {
            cout << "# ";
        }else {
            if (referencedBit(pageTable[i]) == 1) {
                cout << " "<< i << ":R";
            } else{
                cout << " "<< i << ":-";
            }
            if (modifiedBit(pageTable[i]) == 1) {
                cout << "M";
            }else{
                cout << "-";
            }
            if (pageoutBit(pageTable[i]) == 1) {
                cout << "S ";
            }else{
                cout << "- ";
            }
        }
    }
    cout <<"  || hand = "<< pageCount << "\n";
}


// Second Chance Algorithm

void SecondChanceMapping::insertClass(){};
void SecondChanceMapping::resetRef(){};
void SecondChanceMapping::clearClass(){};
void SecondChanceMapping::readRfile(const char*rfile){};
int SecondChanceMapping::checkClass(int refernced, int modified){return 0;};

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
        frameTable.erase(frameTable.begin());
        frameTable.push_back(tempPage);
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
                pageTable[instruction.virtualPageIndex] = calculatePTE(presentBit(pageTable[instruction.virtualPageIndex]), 1, 1, pageoutBit(pageTable[instruction.virtualPageIndex]), physicalFrameNumber(instruction.virtualPageIndex));
            }
            pageTableOPtion();
            return true;
        }
    }
    return false;
}

void SecondChanceMapping::outPage(int inputLine,int page, Instruction instruction){
    cout<< inputLine << ": OUT     "<< page << "   "<< physicalFrameNumber(page)<< endl;
    pageTable[instruction.virtualPageIndex] = calculatePTE(1, instruction.operation, 1, pageoutBit(pageTable[instruction.virtualPageIndex]), physicalFrameNumber(page));
    pageTable[page] = calculatePTE(0, modifiedBit(pageTable[page]), referencedBit(pageTable[page]), 1, 0);
//    frameTable.front() = instruction.virtualPageIndex;
    frameTable.erase(frameTable.begin());
    frameTable.push_back(instruction.virtualPageIndex);

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
//        frameTable.front() = instruction.virtualPageIndex;
        frameTable.erase(frameTable.begin());
        frameTable.push_back(instruction.virtualPageIndex);
        printMap(inputLine, instruction);
        
    }
}
void SecondChanceMapping::pageTableOPtion(){
    for (int i = 0; i < 64; i++) {
        if (presentBit(pageTable[i]) == 0 && pageoutBit(pageTable[i]) != 1) {
            cout << "* ";
        }else if (presentBit(pageTable[i]) == 0 && pageoutBit(pageTable[i]) == 1) {
            cout << "# ";
        }else {
            if (referencedBit(pageTable[i]) == 1) {
                cout << " "<< i << ":R";
            } else{
                cout << " "<< i << ":-";
            }
            if (modifiedBit(pageTable[i]) == 1) {
                cout << "M";
            }else{
                cout << "-";
            }
            if (pageoutBit(pageTable[i]) == 1) {
                cout << "S ";
            }else{
                cout << "- ";
            }
        }
    }
    cout <<"\n";
}


//Random Page Replace Algorithm
void RandomMapping::resizeFrameTable(int a){};
void RandomMapping::insertClass(){};
void RandomMapping::resetRef(){};
void RandomMapping::clearClass(){};
int RandomMapping::checkClass(int refernced, int modified){return 0;};

unsigned long RandomMapping::calculatePTE(int a, int b, int c, int d, int e){
    pte = a * pow(2, 31) + b * pow(2, 30) + c * pow(2, 29) + d * pow(2, 28) + e;
    return pte;
};

void RandomMapping::insertEmptyPage(Instruction instruction, int x){
    pageTable[instruction.virtualPageIndex] = pageTable[instruction.virtualPageIndex] = calculatePTE(1, instruction.operation, 1, 0, x);
    pageTablePosition++;
    k = pageTablePosition;
}

int RandomMapping::presentBit(unsigned long pte){
    int x;
    pte = pte >> 31;
    x = pte & 1;
    return x;
};

int RandomMapping::modifiedBit(unsigned long pte){
    int x;
    pte = pte >> 30;
    x = pte & 1;
    return x;
};

int RandomMapping::referencedBit(unsigned long pte){
    int x;
    pte = pte >> 29;
    x = pte & 1;
    return x;
};

int RandomMapping::pageoutBit(unsigned long pte){
    int x;
    pte = pte >> 28;
    x = pte & 1;
    return x;
};

int RandomMapping::physicalFrameNumber(int a){
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

bool RandomMapping::checkReferred(Instruction instruction){
    if (referencedBit(pageTable[instruction.virtualPageIndex]) != 0) {
        return true;
    }
    else
    {
        return false;
    }
}

void RandomMapping::updateFrameTable(int inputLine, int a, Instruction instruction){
    frameTable[a] = instruction.virtualPageIndex;
}

void RandomMapping::printTable(Instruction instruction, int a){
    cout<<"==> inst: "<<instruction.operation << " "<<instruction.virtualPageIndex<<endl;
    cout<< a << ": ZERO        "<< physicalFrameNumber(instruction.virtualPageIndex)<<endl;
    cout<< a << ": MAP    "<< instruction.virtualPageIndex <<"   "<< physicalFrameNumber(instruction.virtualPageIndex)<<endl;
}

int RandomMapping::tablePosition(){
    return pageTablePosition;
}

int RandomMapping::choosePage(int a){
    int j = 0;
    int page = 0;
    if (rNum >= randomNum.size()) {
        rNum -= randomNum.size();
    }
    j = randomNum[rNum] % a;
    page = frameTable[j];
    rNum++;
    return page;
}

bool RandomMapping::sameVaildPage(int a, int b, Instruction instruction){
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

void RandomMapping::outPage(int inputLine,int page, Instruction instruction){
    cout<< inputLine << ": OUT     "<< page << "   "<< physicalFrameNumber(page)<< endl;
    pageTable[instruction.virtualPageIndex] = calculatePTE(1, instruction.operation, 1, pageoutBit(pageTable[instruction.virtualPageIndex]), physicalFrameNumber(page));
    pageTable[page] = calculatePTE(0, modifiedBit(pageTable[page]), referencedBit(pageTable[page]), 1, 0);
    frameTable[physicalFrameNumber(instruction.virtualPageIndex)] = instruction.virtualPageIndex;
}

void RandomMapping::printMap(int inputLine, Instruction instruction){
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
void RandomMapping::replacePage(int inputLine, int oldPage, Instruction instruction){
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

void RandomMapping::readRfile(const char* rfile){
    int i = 0;
    string buffer;
    ifstream infile(rfile);
    char* numtoken;
    if(!infile.is_open()){
        cout<<"Failed to open"<<endl;
    }
    else{
        while (!infile.eof()) {
            getline(infile, buffer);
            buffer1 = new char[buffer.length() + 1];
            strcpy(buffer1, buffer.c_str());
            numtoken = strtok(buffer1, "\n");
            if (i == 0) {
                randomNum.resize(atoi(numtoken));
            } else if (i <= randomNum.size()){
                randomNum[i - 1] = atoi(numtoken);
            }
            i++;
        }
    }
    infile.close();
}
void RandomMapping::pageTableOPtion(){
    for (int i = 0; i < 64; i++) {
        if (presentBit(pageTable[i]) == 0 && pageoutBit(pageTable[i]) != 1) {
            cout << "* ";
        }else if (presentBit(pageTable[i]) == 0 && pageoutBit(pageTable[i]) == 1) {
            cout << "# ";
        }else {
            if (referencedBit(pageTable[i]) == 1) {
                cout << " "<< i << ":R";
            } else{
                cout << " "<< i << ":-";
            }
            if (modifiedBit(pageTable[i]) == 1) {
                cout << "M";
            }else{
                cout << "-";
            }
            if (pageoutBit(pageTable[i]) == 1) {
                cout << "S ";
            }else{
                cout << "- ";
            }
        }
    }
    cout <<"\n";
}


//NRU Algorithm
void NRUMapping::resizeFrameTable(int a){};
unsigned long NRUMapping::calculatePTE(int a, int b, int c, int d, int e){
    pte = a * pow(2, 31) + b * pow(2, 30) + c * pow(2, 29) + d * pow(2, 28) + e;
    return pte;
};

void NRUMapping::insertEmptyPage(Instruction instruction, int x){
    pageTable[instruction.virtualPageIndex] = pageTable[instruction.virtualPageIndex] = calculatePTE(1, instruction.operation, 1, 0, x);
    pageTablePosition++;
}


int NRUMapping::presentBit(unsigned long pte){
    int x;
    pte = pte >> 31;
    x = pte & 1;
    return x;
};

int NRUMapping::modifiedBit(unsigned long pte){
    int x;
    pte = pte >> 30;
    x = pte & 1;
    return x;
};

int NRUMapping::referencedBit(unsigned long pte){
    int x;
    pte = pte >> 29;
    x = pte & 1;
    return x;
};

int NRUMapping::pageoutBit(unsigned long pte){
    int x;
    pte = pte >> 28;
    x = pte & 1;
    return x;
};

int NRUMapping::physicalFrameNumber(int a){
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

bool NRUMapping::checkReferred(Instruction instruction){
    if (referencedBit(pageTable[instruction.virtualPageIndex]) != 0) {
        return true;
    }
    else
    {
        return false;
    }
}

void NRUMapping::updateFrameTable(int inputLine, int a, Instruction instruction){
    frameTable[a] = instruction.virtualPageIndex;
}

void NRUMapping::printTable(Instruction instruction, int a){
    cout<<"==> inst: "<<instruction.operation << " "<<instruction.virtualPageIndex<<endl;
    cout<< a << ": ZERO        "<< physicalFrameNumber(instruction.virtualPageIndex)<<endl;
    cout<< a << ": MAP    "<< instruction.virtualPageIndex <<"   "<< physicalFrameNumber(instruction.virtualPageIndex)<<endl;
}

int NRUMapping::tablePosition(){
    return pageTablePosition;
}

int NRUMapping::checkClass(int refernced, int modified){
    if (refernced == 1 && modified == 1) {
        return 3;
    } else if (refernced == 1 && modified == 0){
        return 2;
    } else if (refernced == 0 && modified == 1){
        return 1;
    } else {
        return 0;
    }
}
void NRUMapping::insertClass(){
    for (int i = 0 ; i < 64; i++) {
        if (presentBit(pageTable[i]) == 1) {
            NRUClass[checkClass(referencedBit(pageTable[i]), modifiedBit(pageTable[i]))].push_back(i);
        }
    }
}

void NRUMapping::clearClass(){
    for (int i = 0; i < 4; i++) {
        NRUClass[i].clear();
    }
}

void NRUMapping::resetRef(){
    for (int i = 0; i < 64; i++) {
        pageTable[i] = calculatePTE(presentBit(pageTable[i]), modifiedBit(pageTable[i]), 0, pageoutBit(pageTable[i]), physicalFrameNumber(i));
    }
}


void NRUMapping::readRfile(const char* rfile){
    int i = 0;
    string buffer;
    ifstream infile(rfile);
    char* numtoken;
    if(!infile.is_open()){
        cout<<"Failed to open"<<endl;
    }
    else{
        while (!infile.eof()) {
            getline(infile, buffer);
            buffer1 = new char[buffer.length() + 1];
            strcpy(buffer1, buffer.c_str());
            numtoken = strtok(buffer1, "\n");
            if (i == 0) {
                randomNum.resize(atoi(numtoken));
            } else if (i <= randomNum.size()){
                randomNum[i - 1] = atoi(numtoken);
            }
            i++;
        }
    }
    infile.close();
}

int NRUMapping::choosePage(int a){
    int page = 0;
    for (int i = 0; i < 4; i++) {
        if (!NRUClass[i].empty()) {
            classType = i;
            nruClassIndex = randomNum[rNum] % (NRUClass[i].size());
            page = NRUClass[i][nruClassIndex];
            rNum++;
            clearClass();
            return page;
        }
    }
    return 0;
}

bool NRUMapping::sameVaildPage(int a, int b, Instruction instruction){
    for (int i = 0; i < b; i++) {
        if (frameTable[i] == instruction.virtualPageIndex) {
            cout<<"==> inst: "<<instruction.operation<<" "<<instruction.virtualPageIndex<< endl;
            if (instruction.operation ==  1) {
                pageTable[instruction.virtualPageIndex] = calculatePTE(presentBit(pageTable[instruction.virtualPageIndex]), 1, 1, pageoutBit(pageTable[instruction.virtualPageIndex]), physicalFrameNumber(instruction.virtualPageIndex));
                
            }
            if (instruction.operation == 0) {
                pageTable[instruction.virtualPageIndex] = calculatePTE(presentBit(pageTable[instruction.virtualPageIndex]), modifiedBit(pageTable[instruction.virtualPageIndex]), 1, pageoutBit(pageTable[instruction.virtualPageIndex]), physicalFrameNumber(instruction.virtualPageIndex));
            }
            pageTableOPtion();
            return true;
        }
    }
    return false;
}

void NRUMapping::outPage(int inputLine,int page, Instruction instruction){
    cout<< inputLine << ": OUT     "<< page << "   "<< physicalFrameNumber(page)<< endl;
    pageTable[instruction.virtualPageIndex] = calculatePTE(1, instruction.operation, 1, pageoutBit(pageTable[instruction.virtualPageIndex]), physicalFrameNumber(page));
    pageTable[page] = calculatePTE(0, 0, referencedBit(pageTable[page]), 1, 0);
    frameTable[physicalFrameNumber(instruction.virtualPageIndex)] = instruction.virtualPageIndex;
}

void NRUMapping::printMap(int inputLine, Instruction instruction){
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
void NRUMapping::replacePage(int inputLine, int oldPage, Instruction instruction){
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

void NRUMapping::pageTableOPtion(){
    for (int i = 0; i < 64; i++) {
        if (presentBit(pageTable[i]) == 0 && pageoutBit(pageTable[i]) != 1) {
            cout << "* ";
        }else if (presentBit(pageTable[i]) == 0 && pageoutBit(pageTable[i]) == 1) {
            cout << "# ";
        }else {
            if (referencedBit(pageTable[i]) == 1) {
                cout << " "<< i << ":R";
            } else{
                cout << " "<< i << ":-";
            }
            if (modifiedBit(pageTable[i]) == 1) {
                cout << "M";
            }else{
                cout << "-";
            }
            if (pageoutBit(pageTable[i]) == 1) {
                cout << "S ";
            }else{
                cout << "- ";
            }
        }
    }
    cout <<"\n";
}


//Aging Algorithm

//void AgingMapping::readRfile(const char*rfile){};
//void AgingMapping::resizeFrameTable(int a){};
//void AgingMapping::insertClass(){};
//void AgingMapping::resetRef(){};
//void AgingMapping::clearClass(){};
//int AgingMapping::checkClass(int refernced, int modified){return 0;};
//
//
//unsigned long AgingMapping::calculatePTE(int a, int b, int c, int d, int e){
//    pte = a * pow(2, 31) + b * pow(2, 30) + c * pow(2, 29) + d * pow(2, 28) + e;
//    return pte;
//};
//
//unsigned long AgingMapping::calculateAge(int a, unsigned long b){
//    b = b >> 1;
//    b = a * pow(2, 31) + b;
//    return b;
//}
//
//void AgingMapping::insertEmptyPage(Instruction instruction, int x){
//    agebitP.resize(64);
//    pageTable[instruction.virtualPageIndex] = pageTable[instruction.virtualPageIndex] = calculatePTE(1, instruction.operation, 1, 0, x);
//    pageTablePosition++;
//    agebitF.push_back(calculateAge(1, 0));
//    agebitP[instruction.virtualPageIndex] = calculateAge(1, 0);
//    k = pageTablePosition;
//}
//
//int AgingMapping::presentBit(unsigned long pte){
//    int x;
//    pte = pte >> 31;
//    x = pte & 1;
//    return x;
//};
//
//int AgingMapping::modifiedBit(unsigned long pte){
//    int x;
//    pte = pte >> 30;
//    x = pte & 1;
//    return x;
//};
//
//int AgingMapping::referencedBit(unsigned long pte){
//    int x;
//    pte = pte >> 29;
//    x = pte & 1;
//    return x;
//};
//
//int AgingMapping::pageoutBit(unsigned long pte){
//    int x;
//    pte = pte >> 28;
//    x = pte & 1;
//    return x;
//};
//
//int AgingMapping::physicalFrameNumber(int a){
//    int i = 6;
//    unsigned long x;
//    int y;
//    PhyNumber = 0;
//    while (i > 0) {
//        x = pageTable[a];
//        x = x >> (i - 1);
//        y = x & 1;
//        PhyNumber += y * pow(2, i - 1);
//        i--;
//    }
//    return PhyNumber;
//};
//
//bool AgingMapping::checkReferred(Instruction instruction){
//    if (referencedBit(pageTable[instruction.virtualPageIndex]) != 0) {
//        return true;
//    }
//    else
//    {
//        return false;
//    }
//}
//
//void AgingMapping::updateFrameTable(int inputLine, int a, Instruction instruction){
//    frameTable[a] = instruction.virtualPageIndex;
//    //    cout<< "frameNum:"<<a << "  Virtual:"<<frameTable[a] << "  Ref:"<< referencedBit(pageTable[frameTable[a]])<<endl;
//}
//
//void AgingMapping::printTable(Instruction instruction, int a){
//    cout<<"==> inst: "<<instruction.operation << " "<<instruction.virtualPageIndex<<endl;
//    cout<< a << ": ZERO        "<< physicalFrameNumber(instruction.virtualPageIndex)<<endl;
//    cout<< a << ": MAP    "<< instruction.virtualPageIndex <<"   "<< physicalFrameNumber(instruction.virtualPageIndex)<<endl;
//}
//
//int AgingMapping::tablePosition(){
//    return pageTablePosition;
//}
//int AgingMapping::choosePage(int a){
//    int page = 0;
//    unsigned long min = agebitF[startPoint];
//    agebitF.resize(a);
//    for (int i = 0; i < a; i++) {
//        if (agebitF[i] < min) {
//            min = agebitF[i];
//            frameIndex = i;
//        } else{
//            min = agebitF[startPoint];
//        }
//    }
//    if (min == agebitF[startPoint]) {
//        frameIndex = startPoint;
//        startPoint++;
//    }
//    page = frameTable[frameIndex];
//    return page;
//}
//
//
//bool AgingMapping::sameVaildPage(int a, int b, Instruction instruction){
//    for (int i = 0; i < b; i++) {
//        if (frameTable[i] == instruction.virtualPageIndex) {
//            agebitF[i] = calculateAge(1, agebitF[i]);
//            agebitP[frameTable[i]] = calculateAge(1, agebitF[i]);
//            cout<<"==> inst: "<<instruction.operation<<" "<<instruction.virtualPageIndex<< endl;
//            pageTable[instruction.virtualPageIndex] = calculatePTE(presentBit(pageTable[instruction.virtualPageIndex]), modifiedBit(pageTable[instruction.virtualPageIndex]), 1, pageoutBit(pageTable[instruction.virtualPageIndex]), physicalFrameNumber(instruction.virtualPageIndex));
//            if (instruction.operation ==  1) {
//                pageTable[instruction.virtualPageIndex] = calculatePTE(presentBit(pageTable[instruction.virtualPageIndex]), 1, referencedBit(pageTable[instruction.virtualPageIndex]), pageoutBit(pageTable[instruction.virtualPageIndex]), physicalFrameNumber(instruction.virtualPageIndex));
//            }
//            return true;
//        }
//    }
//    //    k--;
//    return false;
//}
//
//void AgingMapping::outPage(int inputLine,int page, Instruction instruction){
//    cout<< inputLine << ": OUT     "<< page << "   "<< physicalFrameNumber(page)<< endl;
//    pageTable[instruction.virtualPageIndex] = calculatePTE(1, instruction.operation, 1, pageoutBit(pageTable[instruction.virtualPageIndex]), physicalFrameNumber(page));
//    pageTable[page] = calculatePTE(0, modifiedBit(pageTable[page]), referencedBit(pageTable[page]), 1, 0);
//    frameTable[physicalFrameNumber(instruction.virtualPageIndex)] = instruction.virtualPageIndex;
//}
//
//void AgingMapping::printMap(int inputLine, Instruction instruction){
//    if (referencedBit(pageTable[instruction.virtualPageIndex]) == 0 && modifiedBit(pageTable[instruction.virtualPageIndex]) == 0){
//        cout<< inputLine << ": ZERO        "<< physicalFrameNumber(instruction.virtualPageIndex) << endl;
//        cout<< inputLine << ": MAP    "<< instruction.virtualPageIndex << "   "<< physicalFrameNumber(instruction.virtualPageIndex)<< endl;
//    } else if (pageoutBit(pageTable[instruction.virtualPageIndex]) == 0) {
//        cout<< inputLine << ": ZERO        "<< physicalFrameNumber(instruction.virtualPageIndex) << endl;
//        cout<< inputLine << ": MAP    "<< instruction.virtualPageIndex << "   "<< physicalFrameNumber(instruction.virtualPageIndex)<< endl;
//    } else {
//        cout<< inputLine << ": IN      "<< instruction.virtualPageIndex <<"   " << physicalFrameNumber(instruction.virtualPageIndex) << endl;
//        cout<< inputLine << ": MAP    "<< instruction.virtualPageIndex << "   "<< physicalFrameNumber(instruction.virtualPageIndex)<< endl;
//    }
//    
//}
//
//void AgingMapping::updateAgeBit(int page){
//    
//}
//void AgingMapping::replacePage(int inputLine, int oldPage, Instruction instruction){
////    int x = 0;
//    cout<< "==> inst: " << instruction.operation << " "<<instruction.virtualPageIndex <<endl;
//    cout<< inputLine << ": UNMAP   "<< oldPage << "   "<< physicalFrameNumber(oldPage)<< endl;
////    for (int i = 0; i < frameIndex; i++) {
////        agebitF[i] = calculateAge(0, agebitF[i]);
////    }
////    for (int i = frameIndex + 1; i < agebitF.size() ; i++) {
////        agebitF[i] = calculateAge(0, agebitF[i]);
////    }
////    
////    for (int i = 0; i < frameTable[frameIndex]; i++) {
////        agebitP[i] = calculateAge(0, agebitF[i]);
////    }
////    for (int i = frameTable[frameIndex] + 1; i < 64 ; i++) {
////        agebitF[i] = calculateAge(0, agebitF[i]);
////    }
////    agebitP[oldPage] = agebitF[frameIndex];
////    agebitF[frameIndex] = calculateAge(1, agebitP[instruction.virtualPageIndex]);
//    if (modifiedBit(pageTable[oldPage]) == 1) {
//        outPage(inputLine, oldPage, instruction);
//        printMap(inputLine, instruction);
//    } else{
//        pageTable[instruction.virtualPageIndex] = calculatePTE(1, instruction.operation, 1, pageoutBit(pageTable[instruction.virtualPageIndex]), physicalFrameNumber(oldPage));
//        pageTable[oldPage] = calculatePTE(0, modifiedBit(pageTable[oldPage]), referencedBit(pageTable[oldPage]), pageoutBit(pageTable[oldPage]), 0);
//        frameTable[physicalFrameNumber(instruction.virtualPageIndex)] = instruction.virtualPageIndex;
//        printMap(inputLine, instruction);
//    }
//}
//void AgingMapping::pageTableOPtion(){
//    for (int i = 0; i < 64; i++) {
//        if (presentBit(pageTable[i]) == 0 && pageoutBit(pageTable[i]) != 1) {
//            cout << "* ";
//        }else if (presentBit(pageTable[i]) == 0 && pageoutBit(pageTable[i]) == 1) {
//            cout << "# ";
//        }else {
//            if (referencedBit(pageTable[i]) == 1) {
//                cout << " "<< i << ":R";
//            } else{
//                cout << " "<< i << ":-";
//            }
//            if (modifiedBit(pageTable[i]) == 1) {
//                cout << "M";
//            }else{
//                cout << "-";
//            }
//            if (pageoutBit(pageTable[i]) == 1) {
//                cout << "S ";
//            }else{
//                cout << "- ";
//            }
//        }
//    }
//    cout <<"\n";
//}
//

