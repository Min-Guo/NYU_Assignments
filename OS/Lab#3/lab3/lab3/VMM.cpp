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


void FIFOMapping::insertEmptyPage(Instruction instruction, int a){
    pageTable[instruction.virtualPageIndex].present = 1;
    pageTable[instruction.virtualPageIndex].referenced = 1;
    pageTable[instruction.virtualPageIndex].modified = instruction.operation;
    pageTable[instruction.virtualPageIndex].pageout = "zero";
    pageTable[instruction.virtualPageIndex].physicalFrame = a;
}