//
//  VMM.h
//  lab3
//
//  Created by Min Guo on 7/12/15.
//  Copyright (c) 2015 Min Guo. All rights reserved.
//

#ifndef __lab3__VMM__
#define __lab3__VMM__

#include <stdio.h>
#include <vector>
#include <string.h>
#include <iostream>
using namespace std;

struct Instruction {
    int operation;
    bool operationState;
    int virtualPageIndex;
    bool virtualPageState;
};

struct PageTableEntry {
    int present;
    int modified;
    int referenced;
    string pageout;
    int physicalFrame;
};

class PageMapping{
private:
    PageTableEntry pageTable[64];
public:
    virtual void insertEmptyPage(Instruction instruction, int a) = 0;
};

class FIFOMapping:public PageMapping{
private:
    PageTableEntry pageTable[64];
public:
    void insertEmptyPage(Instruction instruction, int a);
};
#endif /* defined(__lab3__VMM__) */
