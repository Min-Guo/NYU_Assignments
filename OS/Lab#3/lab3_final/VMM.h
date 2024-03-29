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



class PageMapping{
private:
    unsigned long pageTable[64];
    int frameTable[16];
    unsigned long pte;
    int PhyNumber;
public:
    virtual void insertEmptyPage(Instruction instruction, int a) = 0;
    virtual bool checkReferred(Instruction instruction) = 0;
    virtual void printTable(Instruction instruction, int inputLine, int sameState) = 0;
    virtual int presentBit(unsigned long pte) = 0;
    virtual int modifiedBit(unsigned long pte) = 0;
    virtual int referencedBit(unsigned long pte) = 0;
    virtual int pageoutBit(unsigned long pte) = 0;
    virtual unsigned long calculatePTE(int a, int b, int c, int d, int e) = 0;
    virtual int physicalFrameNumber(int a) = 0;
    virtual void updateFrameTable(int inputLine, int a, Instruction instruction) = 0;
    virtual int choosePage(int a) = 0;
    virtual void replacePage(int inputLine, int oldPage, Instruction instruction, int phyNum) = 0;
    virtual bool sameVaildPage(int inputLine, int Page, Instruction instruction, int state) = 0;
    virtual void outPage(int inputLine,int page, Instruction instruction, int phyNum) = 0;
    virtual void printMap(int inputLine, Instruction instruction, int oldPage, int oldPhy, int sameState) = 0;
    virtual int tablePosition() = 0;
    virtual void resizeFrameTable(int a) = 0;
    virtual void readRfile(const char*rfile) = 0;
    virtual void insertClass() = 0;
    virtual int checkClass(int refernced, int modified) = 0;
    virtual void resetRef() = 0;
    virtual void pageTableOPtion() = 0;
    virtual void clearClass() = 0;
    virtual void updateAgePageTable() = 0;
    virtual void printSummary(int inputLine) = 0;
    virtual void printFrameMap(int frameNum) = 0;
};


class FIFOMapping:public PageMapping{
private:
    unsigned long pageTable[64];
    int frameTable[64];
    unsigned long pte;
    int PhyNumber;
    unsigned int unmapCount;
    unsigned int mapCount;
    unsigned int inCount;
    unsigned int outCount;
    unsigned int zeroCount;
    long long totalCost;
public:
    void insertEmptyPage(Instruction instruction, int a);
    bool checkReferred(Instruction instruction);
    void printTable(Instruction instruction, int inputLine, int sameState);
    int presentBit(unsigned long pte);
    int modifiedBit(unsigned long pte);
    int referencedBit(unsigned long pte);
    int pageoutBit(unsigned long pte);
    unsigned long calculatePTE(int a, int b, int c, int d, int e);
    int physicalFrameNumber(int a);
    void updateFrameTable(int inputLine, int a, Instruction instruction);
    int choosePage(int a);
    void replacePage(int inputLine, int oldPage, Instruction instruction, int phyNum);
    bool sameVaildPage(int inputLine, int page, Instruction instruction, int state);
    void outPage(int inputLine,int page, Instruction instruction, int phyNum);
    void printMap(int inputLine, Instruction instruction, int oldPage, int oldPhy, int sameState);
    int tablePosition();
    void resizeFrameTable(int a);
    void readRfile(const char*rfile);
    void insertClass();
    void resetRef();
    void pageTableOPtion();
    void clearClass();
    int checkClass(int refernced, int modified);
    void updateAgePageTable();
    void printSummary(int inputLine);
    void printFrameMap(int frameNum);
};

class LRUMapping:public PageMapping{
private:
    unsigned long pageTable[64];
    int frameTable[64][2];
    unsigned long pte;
    int PhyNumber;
    unsigned int unmapCount;
    unsigned int mapCount;
    unsigned int inCount;
    unsigned int outCount;
    unsigned int zeroCount;
    long long totalCost;
public:
    void insertEmptyPage(Instruction instruction, int a);
    bool checkReferred(Instruction instruction);
    void printTable(Instruction instruction, int inputLine, int sameState);
    int presentBit(unsigned long pte);
    int modifiedBit(unsigned long pte);
    int referencedBit(unsigned long pte);
    int pageoutBit(unsigned long pte);
    unsigned long calculatePTE(int a, int b, int c, int d, int e);
    int physicalFrameNumber(int a);
    void updateFrameTable(int inputLine, int a, Instruction instruction);
    int choosePage(int a);
    void replacePage(int inputLine, int oldPage, Instruction instruction, int phyNum);
    bool sameVaildPage(int inputLine, int page, Instruction instruction, int state);
    void outPage(int inputLine,int page, Instruction instruction, int phyNum);
    void printMap(int inputLine, Instruction instruction, int oldPage, int oldPhy, int sameState);
    int tablePosition();
    void resizeFrameTable(int a);
    void readRfile(const char*rfile);
    void insertClass();
    void resetRef();
    void pageTableOPtion();
    void clearClass();
    int checkClass(int refernced, int modified);
    void updateAgePageTable();
    void printSummary(int inputLine);
    void printFrameMap(int frameNum);
};

class ClockMapping:public PageMapping{
private:
    unsigned long pageTable[64];
    int frameTable[64];
    unsigned long pte;
    int PhyNumber;
    unsigned int unmapCount;
    unsigned int mapCount;
    unsigned int inCount;
    unsigned int outCount;
    unsigned int zeroCount;
    long long totalCost;
public:
    void insertEmptyPage(Instruction instruction, int a);
    bool checkReferred(Instruction instruction);
    void printTable(Instruction instruction, int inputLine, int sameState);
    int presentBit(unsigned long pte);
    int modifiedBit(unsigned long pte);
    int referencedBit(unsigned long pte);
    int pageoutBit(unsigned long pte);
    unsigned long calculatePTE(int a, int b, int c, int d, int e);
    int physicalFrameNumber(int a);
    void updateFrameTable(int inputLine, int a, Instruction instruction);
    int choosePage(int a);
    void replacePage(int inputLine, int oldPage, Instruction instruction, int phyNum);
    bool sameVaildPage(int inputLine, int page, Instruction instruction, int state);
    void outPage(int inputLine,int page, Instruction instruction, int phyNum);
    void printMap(int inputLine, Instruction instruction, int oldPage, int oldPhy, int sameState);
    int tablePosition();
    void resizeFrameTable(int a);
    void readRfile(const char*rfile);
    void insertClass();
    void resetRef();
    void pageTableOPtion();
    void clearClass();
    int checkClass(int refernced, int modified);
    void updateAgePageTable();
    void printSummary(int inputLine);
    void printFrameMap(int frameNum);
};

class SecondChanceMapping:public PageMapping{
private:
    unsigned long pageTable[64];
    vector<int> pageList;
    int frameTable[64];
    unsigned long pte;
    int PhyNumber;
    unsigned int unmapCount;
    unsigned int mapCount;
    unsigned int inCount;
    unsigned int outCount;
    unsigned int zeroCount;
    long long totalCost;
public:
    void insertEmptyPage(Instruction instruction, int a);
    bool checkReferred(Instruction instruction);
    void printTable(Instruction instruction, int inputLine, int sameState);
    int presentBit(unsigned long pte);
    int modifiedBit(unsigned long pte);
    int referencedBit(unsigned long pte);
    int pageoutBit(unsigned long pte);
    unsigned long calculatePTE(int a, int b, int c, int d, int e);
    int physicalFrameNumber(int a);
    void updateFrameTable(int inputLine, int a, Instruction instruction);
    int choosePage(int a);
    void replacePage(int inputLine, int oldPage, Instruction instruction, int phyNum);
    bool sameVaildPage(int inputLine, int page, Instruction instruction, int state);
    void outPage(int inputLine,int page, Instruction instruction, int phyNum);
    void printMap(int inputLine, Instruction instruction, int oldPage, int oldPhy, int sameState);
    int tablePosition();
    void resizeFrameTable(int a);
    void readRfile(const char*rfile);
    void insertClass();
    void resetRef();
    void pageTableOPtion();
    void clearClass();
    int checkClass(int refernced, int modified);
    void updateAgePageTable();
    void printSummary(int inputLine);
    void printFrameMap(int frameNum);
};

class RandomMapping:public PageMapping{
private:
    unsigned long pageTable[64];
    int frameTable[64];
    vector<unsigned int> randomNum;
    FILE *rfile;
    unsigned long pte;
    int PhyNumber;
    unsigned int unmapCount;
    unsigned int mapCount;
    unsigned int inCount;
    unsigned int outCount;
    unsigned int zeroCount;
    long long totalCost;
public:
    void insertEmptyPage(Instruction instruction, int a);
    bool checkReferred(Instruction instruction);
    void printTable(Instruction instruction, int inputLine, int sameState);
    int presentBit(unsigned long pte);
    int modifiedBit(unsigned long pte);
    int referencedBit(unsigned long pte);
    int pageoutBit(unsigned long pte);
    unsigned long calculatePTE(int a, int b, int c, int d, int e);
    int physicalFrameNumber(int a);
    void updateFrameTable(int inputLine, int a, Instruction instruction);
    int choosePage(int a);
    void replacePage(int inputLine, int oldPage, Instruction instruction, int phyNum);
    bool sameVaildPage(int inputLine, int page, Instruction instruction, int state);
    void outPage(int inputLine,int page, Instruction instruction, int phyNum);
    void printMap(int inputLine, Instruction instruction, int oldPage, int oldPhy, int sameState);
    int tablePosition();
    void resizeFrameTable(int a);
    void readRfile(const char*rfile);
    void insertClass();
    void resetRef();
    void pageTableOPtion();
    void clearClass();
    int checkClass(int refernced, int modified);
    void updateAgePageTable();
    void printSummary(int inputLine);
    void printFrameMap(int frameNum);
};

class NRUMapping:public PageMapping{
private:
    unsigned long pageTable[64];
    int frameTable[64];
    unsigned long pte;
    int PhyNumber;
    vector<int> NRUClass[4];
    vector<unsigned int> randomNum;
    unsigned int unmapCount;
    unsigned int mapCount;
    unsigned int inCount;
    unsigned int outCount;
    unsigned int zeroCount;
    long long totalCost;
public:
    void insertEmptyPage(Instruction instruction, int a);
    bool checkReferred(Instruction instruction);
    void printTable(Instruction instruction, int inputLine, int sameState);
    int presentBit(unsigned long pte);
    int modifiedBit(unsigned long pte);
    int referencedBit(unsigned long pte);
    int pageoutBit(unsigned long pte);
    unsigned long calculatePTE(int a, int b, int c, int d, int e);
    int physicalFrameNumber(int a);
    void updateFrameTable(int inputLine, int a, Instruction instruction);
    int choosePage(int a);
    void replacePage(int inputLine, int oldPage, Instruction instruction, int phyNum);
    bool sameVaildPage(int inputLine, int page, Instruction instruction, int state);
    void outPage(int inputLine,int page, Instruction instruction, int phyNum);
    void printMap(int inputLine, Instruction instruction, int oldPage, int oldPhy, int sameState);
    int tablePosition();
    void resizeFrameTable(int a);
    void readRfile(const char*rfile);
    void insertClass(int a, int b);
    void clearClass();
    int checkClass(int refernced, int modified);
    void insertClass();
    void resetRef();
    void pageTableOPtion();
    void updateAgePageTable();
    void printSummary(int inputLine);
    void printFrameMap(int frameNum);
};


class ClockGlobalMapping:public PageMapping{
private:
    unsigned long pageTable[64];
    int frameTable[64];
    unsigned long pte;
    int PhyNumber;
    unsigned int unmapCount;
    unsigned int mapCount;
    unsigned int inCount;
    unsigned int outCount;
    unsigned int zeroCount;
    long long totalCost;
public:
    void insertEmptyPage(Instruction instruction, int a);
    bool checkReferred(Instruction instruction);
    void printTable(Instruction instruction, int inputLine, int sameState);
    int presentBit(unsigned long pte);
    int modifiedBit(unsigned long pte);
    int referencedBit(unsigned long pte);
    int pageoutBit(unsigned long pte);
    unsigned long calculatePTE(int a, int b, int c, int d, int e);
    int physicalFrameNumber(int a);
    void updateFrameTable(int inputLine, int a, Instruction instruction);
    int choosePage(int a);
    void replacePage(int inputLine, int oldPage, Instruction instruction, int phyNum);
    bool sameVaildPage(int inputLine, int page, Instruction instruction, int state);
    void outPage(int inputLine,int page, Instruction instruction, int phyNum);
    void printMap(int inputLine, Instruction instruction, int oldPage, int oldPhy, int sameState);
    int tablePosition();
    void resizeFrameTable(int a);
    void readRfile(const char*rfile);
    void pageTableOPtion();
    void insertClass();
    void resetRef();
    void clearClass();
    int checkClass(int refernced, int modified);
    void updateAgePageTable();
    void printSummary(int inputLine);
    void printFrameMap(int frameNum);
};

class AgingMapping:public PageMapping{
private:
    unsigned long pageTable[64];
    int frameTable[64];
    unsigned long pte;
    int PhyNumber;
    vector<unsigned long> agebitF;
    vector<unsigned long> agebitP;
    vector<unsigned long> tempAge;
    int order[64];
    unsigned int unmapCount;
    unsigned int mapCount;
    unsigned int inCount;
    unsigned int outCount;
    unsigned int zeroCount;
    long long totalCost;
public:
    void insertEmptyPage(Instruction instruction, int a);
    bool checkReferred(Instruction instruction);
    void printTable(Instruction instruction, int inputLine, int sameState);
    int presentBit(unsigned long pte);
    int modifiedBit(unsigned long pte);
    int referencedBit(unsigned long pte);
    int pageoutBit(unsigned long pte);
    unsigned long calculatePTE(int a, int b, int c, int d, int e);
    int physicalFrameNumber(int a);
    void updateFrameTable(int inputLine, int a, Instruction instruction);
    int choosePage(int a);
    void replacePage(int inputLine, int oldPage, Instruction instruction, int phyNum);
    bool sameVaildPage(int inputLine, int page, Instruction instruction, int state);
    void outPage(int inputLine,int page, Instruction instruction, int phyNum);
    void printMap(int inputLine, Instruction instruction, int oldPage, int oldPhy, int sameState);
    int tablePosition();
    void resizeFrameTable(int a);
    void readRfile(const char*rfile);
    void pageTableOPtion();
    void insertClass();
    void resetRef();
    void clearClass();
    int checkClass(int refernced, int modified);
    void updateAgePageTable();
    void printSummary(int inputLine);
    void printFrameMap(int frameNum);
};

class AgingLocalMapping:public PageMapping{
private:
    unsigned long pageTable[64];
    int frameTable[64];
    unsigned long pte;
    int PhyNumber;
    vector<unsigned long> agebitF;
    vector<unsigned long> agebitP;
    vector<int> tempAge;
    int order[64];
    unsigned int unmapCount;
    unsigned int mapCount;
    unsigned int inCount;
    unsigned int outCount;
    unsigned int zeroCount;
    long long totalCost;
public:
    void insertEmptyPage(Instruction instruction, int a);
    bool checkReferred(Instruction instruction);
    void printTable(Instruction instruction, int inputLine, int sameState);
    int presentBit(unsigned long pte);
    int modifiedBit(unsigned long pte);
    int referencedBit(unsigned long pte);
    int pageoutBit(unsigned long pte);
    unsigned long calculatePTE(int a, int b, int c, int d, int e);
    int physicalFrameNumber(int a);
    void updateFrameTable(int inputLine, int a, Instruction instruction);
    int choosePage(int a);
    void replacePage(int inputLine, int oldPage, Instruction instruction, int phyNum);
    bool sameVaildPage(int inputLine, int page, Instruction instruction, int state);
    void outPage(int inputLine,int page, Instruction instruction, int phyNum);
    void printMap(int inputLine, Instruction instruction, int oldPage, int oldPhy, int sameState);
    int tablePosition();
    void resizeFrameTable(int a);
    void readRfile(const char*rfile);
    void pageTableOPtion();
    void insertClass();
    void resetRef();
    void clearClass();
    int checkClass(int refernced, int modified);
    void updateAgePageTable();
    void printSummary(int inputLine);
    void printFrameMap(int frameNum);
};


#endif /* defined(__lab3__VMM__) */
