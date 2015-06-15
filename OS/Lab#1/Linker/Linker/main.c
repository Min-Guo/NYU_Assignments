//
//  main.c
//  main
//
//  Created by Min Guo on 6/6/15.
//  Copyright (c) 2015 Min Guo. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

FILE *file;
char getLine[512];
int tempIns;
int baseAddress = 0;
char line_buffer[512];
char *token;
char tokenBuffer[256][2];
int lineNum = 0;
int listType = 0;
int defOperationNum = -1;
int ProOperationNum = -1;
int operationNum = -1;
bool middleState = false;
int moduleNumber = 0;
int numInstructions[10][3];
char *tempToken;
int lengthModule = 0;
int defCount = 0;
int declareCount = 0;
int prevTotalDefcount = 0;
int prevTotalDeclareCount = 0;
int totalLengthModule = 0;
int i = 0;
//int j = 0;
int k = 0;
int k_parseTwo = 0;
int baseAdd_parseTwo = 0;
int i_parseTwo = 0;
int j_parseTwo = 0;
bool foundState = false;
int offset = 0;
char prevLine[512];
//bool checkUseState = false;
/* define symbol table */
struct symbolDef
{
    char symbolName[16];
    char symbolAddress[4];
    int symbolAbsoluteAddress;
    int modulePostion;
    bool checkUseState;
};
struct symbolDef symbolDefs[256] = {NULL};

struct symbolList
{
    char symbolDeclare[16];
    int modulePosition;
    bool usedState;
};
struct symbolList symbolLists[256];

struct programText
{
    char addType;
    int instruction;
};
struct programText programTexts[256];

struct programText_parseTwo
{
    char addType;
    int instruction;
};
struct programText_parseTwo programTexts_parseTwo[256];




int main() {


    file = fopen("/Users/Min/Development/NYU_Assignments/OS/Lab#1/labsamples/input-19", "r");
    ParseOne(file);
    fclose(file);
    
    
    file = fopen("/Users/Min/Development/NYU_Assignments/OS/Lab#1/labsamples/input-19", "r");
    ParseTwo(file);
    
    printf("\n");
    useListState();
    PrintWarning();
    fclose(file);
    
    return 0;
}

int CheckReAdd(){
    for (int n = prevTotalDefcount; n < prevTotalDefcount + defCount; n ++) {
        if (atoi(&symbolDefs[n].symbolAddress) > lengthModule) {
            
            printf("Warning: Module %i : %s to big %i (max=%i) assume zero relative.\n", moduleNumber, &symbolDefs[n].symbolName, atoi(&symbolDefs[n].symbolAddress), lengthModule - 1);
            strcpy(symbolDefs[n].symbolAddress, "0");
            
        }
    }
    return 0;
};



//
///* Absolute Address */
//
int CalculateAbAddress(){
    for (int s = prevTotalDefcount ; s < i ; s++) {
        symbolDefs[s].symbolAbsoluteAddress = (baseAddress + atoi(&symbolDefs[s].symbolAddress));
        //        printf("%s=%i \n", &symbolDefs[s].symbolName, symbolDefs[s].symbolAbsoluteAddress);
    }
    
    return 0;
};

/* print symbol table*/

int PrintSymbolTable(){
    printf("Symbol Table\n");
    for(int s = 0; s < prevTotalDefcount; s++) {
        for (int n = s + 1; n < prevTotalDefcount; n++) {
            if(strcmp(&symbolDefs[s].symbolName, &symbolDefs[n].symbolName)== 0){
                strcpy(&symbolDefs[n].symbolName, &symbolDefs[n+1].symbolName);
                strcpy(&symbolDefs[n].symbolAddress, &symbolDefs[n+1].symbolAddress);
                symbolDefs[n].symbolAbsoluteAddress= symbolDefs[n + 1].symbolAbsoluteAddress;
                i -= 1;
                prevTotalDefcount -= 1;
                printf("%s=%i Error: This variable is multiple times defined; first value used\n", &symbolDefs[s].symbolName, symbolDefs[s].symbolAbsoluteAddress);
            }
        }
        printf("%s=%i\n", &symbolDefs[s].symbolName, symbolDefs[s].symbolAbsoluteAddress);
    }
    
    
    return 0;
};

int ParseOne(FILE* file){
    while (!feof(file)) {
        
        if(fgets(line_buffer, 512, file)!= NULL) {
            lineNum ++;
            strcpy(&getLine, &line_buffer);
//            printf("NewLine is %s\n", getLine);
            if (strcmp(line_buffer,  "\n") != 0) {
                
                token = strtok(line_buffer, "\n");
                
                token = strtok(line_buffer, " ");
                
                
            
                while( token!= NULL )
                {
                    removeTab();
                    
                    if (listType == 0) {
                        if (defOperationNum == -1) {
                            defOperationNum = atoi(token);
                            moduleNumber++;
                            defCount = defOperationNum;
                            if (defOperationNum > 16) {
                                
                                int lenString = strlen(getLine);
                                for (int a = 0; a < lenString; a++) {
                                    if (getLine[a] == token[0]) {
                                        for (int b = 0; b < strlen(token); b++) {
                                            if (getLine[a + b] == token[b]) {
                                                offset = a + 1;
                                                goto printState;
                                            }
                                        }
                                    }
                                }
                            printState:
                                printf("Parse Error line %i offset %i: TO_MANY_DEF_IN_MODULE", lineNum, offset);
                                exit(0);
                                //                                printf("OperationNum is %i\n", defOperationNum);
                            } else if (defOperationNum == 0) {
                                listType = 1;
                                defOperationNum = -1;
                                middleState = false;
                            }
                            
                        } else {
                            if (middleState) {
                                strcpy(&symbolDefs[i].symbolAddress, token);
                                
                                
                                for (int t =0; t < 4; t++) {
                                    
                                    if (!isdigit(symbolDefs[i].symbolAddress[t]) && (symbolDefs[i].symbolAddress[t]!= NULL)) {
                                        int lenString = strlen(getLine);
                                        for (int a = 0; a < lenString; a++) {
                                            if (getLine[a] == token[0]) {
                                                for (int b = 0; b < strlen(token); b++) {
                                                    if (getLine[a + b] == token[b]) {
                                                        offset = a + 1;
                                                        goto printState1;
                                                    }
                                                }
                                            }
                                        }
                                    printState1:
                                        printf("Parse Error line %i offset %i: NUM_EXPECTED", lineNum, offset);
                                        exit(0);
                                    }
                                }
                                
                                symbolDefs[i].modulePostion = moduleNumber;
                                defOperationNum-- ;
                                middleState = false;
                                i++;
                                if (defOperationNum == 0) {
                                    listType = 1;
                                    defOperationNum = -1;
                                    middleState = false;
                                    //                                        CalculateAbAddress();
                                }
                            } else {
                                strcpy(&symbolDefs[i].symbolName, token);
                                //                                    printf("Symbol[%i]Name is %s ", i,  &symbolDefs[i].symbolName);
                                middleState = true;
                            }
                        }
                    } else if (listType == 1){
                        if (operationNum == -1) {
                            operationNum = atoi(token);
                            declareCount = operationNum;
                            if (operationNum > 16) {
                                int lenString = strlen(getLine);
                                for (int a = 0; a < lenString; a++) {
                                    if (getLine[a] == token[0]) {
                                        for (int b = 0; b < strlen(token); b++) {
                                            if (getLine[a + b] == token[b]) {
                                                offset = a + 1;
                                                goto printState2;
                                            }
                                        }
                                    }
                                }
                            printState2:
                                printf("Parse Error line %i offset %i: TO_MANY_USE_IN_MODULE", lineNum, offset);
                                exit(0);
                            } else if (operationNum == 0) {
                                listType = 2;
                                operationNum = -1;
                            }
                        } else {
                            
                            strcpy(&symbolLists[prevTotalDeclareCount].symbolDeclare, token);
                            symbolLists[prevTotalDeclareCount].modulePosition = moduleNumber;
                            
                            if (!isalpha(symbolLists[prevTotalDeclareCount].symbolDeclare[0])) {
                                int lenString = strlen(getLine);
                                for (int a = 0; a < lenString; a++) {
                                    if (getLine[a] == token[0]) {
                                        for (int b = 0; b < strlen(token); b++) {
                                            if (getLine[a + b] == token[b]) {
                                                offset = a + 1;
                                                goto printState3;
                                            }
                                        }
                                    }
                                }
                            printState3:
                                printf("Parse Error line %i offset %i: SYM_EXPECTED", lineNum, offset);
                                exit(0);
                            } else{
                                
                                operationNum--;
                                prevTotalDeclareCount++;
                                if (operationNum == 0) {
                                    listType = 2;
                                    operationNum = -1;
                                }
                            }
                        }
                        
                        
                    } else if (listType == 2) {
                        if (ProOperationNum == -1) {
                            ProOperationNum = atoi(token);
                            lengthModule = ProOperationNum;
                            totalLengthModule += lengthModule;
                            CheckReAdd();
                            CalculateAbAddress();
                            baseAddress += ProOperationNum;
                            if (totalLengthModule > 512) {
                                int lenString = strlen(getLine);
                                for (int a = 0; a < lenString; a++) {
                                    if (getLine[a] == token[0]) {
                                        for (int b = 0; b < strlen(token); b++) {
                                            if (getLine[a + b] == token[b]) {
                                                offset = a + 1;
                                                goto printState4;
                                            }
                                        }
                                    }
                                }
                                
                            printState4:
                                printf("Parse Error line %i offset %i: TO_MANY_INSTR", lineNum, offset);
                                exit(0);
                            }
                            if (ProOperationNum == 0) {
                                listType = 0;
                                ProOperationNum = -1;
                                middleState = false;
                            }
                        } else {
                            if (middleState) {
                                
                                strcpy(&programTexts[k].instruction, token);
                                //                                    printf("Instrution[%i] is %s\n", k, &programTexts[k].instruction);
                                ProOperationNum--;
                                k++;
                                middleState = false;
                                if (ProOperationNum == 0) {
                                    listType = 0;
                                    ProOperationNum = -1;
                                    middleState = false;
                                    
                                    prevTotalDefcount += defCount;
                                    //                                    prevTotalDeclareCount += declareCount;
                                    
                                }
                            } else {
                                strcpy(&programTexts[k].addType, token);
                                middleState = true;
                            }
                        }
                    }
                    
                    token = strtok(NULL, " ");
                    
                }
            } else {
                continue;
            }
        }
        strcpy(&prevLine, &getLine);
    }
    
    if (feof(file) && (defOperationNum != -1)) {
        int lenString = strlen(prevLine);
        for (int a = 0; a < lenString; a++) {
            if (prevLine[a] == '\n') {
                offset = a + 1;
                goto printState5;
            }
        }
        
    printState5:
        printf("Parse Error line %i offset %i: SYM_EXPECTED", lineNum, offset);
        exit(0);
    }
    if (feof(file) && (ProOperationNum != -1)) {
        
        int lenString = strlen(prevLine);
        for (int a = 0; a < lenString; a++) {
            if (prevLine[a] == '\n') {
                offset = a + 1;
                goto printState6;
            }
        }
    printState6:
        printf("Parse Error line %i offset %i: ADDR_EXPECTED", lineNum, offset);
        exit(0);
    }
    PrintSymbolTable();
    return 0;
}







int ParseTwo(FILE* file){
    printf("\nMemory Map\n");
    while (!feof(file)) {
        if(fgets(line_buffer, 512, file)!= NULL) {
            lineNum ++;
            if (strcmp(line_buffer,  "\n") != 0) {
                
                token = strtok(line_buffer, "\n");
                token = strtok(line_buffer, " ");
                while( token!= NULL )
                {
                    
                    removeTab();
                    if (listType == 0) {
                        if (defOperationNum == -1) {
                            defOperationNum = atoi(token);
                            defCount = defOperationNum;
                            if (defOperationNum > 16) {
                                //                                printf("Parse Error line %i: TO_MANY_DEF_IN_MODULE", lineNum);
                                exit(0);
                                //                                printf("OperationNum is %i\n", defOperationNum);
                            } else if (defOperationNum == 0) {
                                listType = 1;
                                defOperationNum = -1;
                                middleState = false;
                            }
                            
                        } else {
                            if (middleState) {
                                //                                strcpy(&symbolDefs[i].symbolAddress, token);
                                //                                for (int t =0; t < 4; t++) {
                                //
                                //                                    if (!isdigit(symbolDefs[i].symbolAddress[t]) && (symbolDefs[i].symbolAddress[t]!= NULL)) {
                                ////                                        printf("Parse Error line %i: NUM_EXPECTED", lineNum);
                                //                                        exit(0);
                                //                                    }
                                //                                }
                                
                                //                                    printf("Symbol[%i]Address is % s\n", i,  &symbolDefs[i].symbolAddress);
                                defOperationNum-- ;
                                middleState = false;
                                i_parseTwo++;
                                if (defOperationNum == 0) {
                                    listType = 1;
                                    defOperationNum = -1;
                                    middleState = false;
                                    //                                        CalculateAbAddress();
                                }
                            } else {
                                //                                strcpy(&symbolDefs[i].symbolName, token);
                                //                                                                    printf("Symbol[%i]Name is %s ", i,  &symbolDefs[i].symbolName);
                                middleState = true;
                            }
                        }
                    } else if (listType == 1){
                        
                        if (operationNum == -1) {
                            operationNum = atoi(token);
                            declareCount = operationNum;
                            if (operationNum > 16) {
                                //                                printf("Parse Error line %i: TO_MANY_USE_IN_MODULE", lineNum);
                                exit(0);
                            } else if (operationNum == 0) {
                                listType = 2;
                                operationNum = -1;
                            }
                        } else {
                            operationNum--;
                            j_parseTwo++;
                            if (operationNum == 0) {
                                listType = 2;
                                operationNum = -1;
                            }
                            //                            }
                        }
                        
                        
                    } else if (listType == 2) {
                        if (ProOperationNum == -1) {
                            ProOperationNum = atoi(token);
                            lengthModule = ProOperationNum;
                            //                            totalLengthModule += lengthModule;
                            //                            CheckReAdd();
                            //                            CalculateAbAddress();
                            
                            if (totalLengthModule > 512) {
                                exit(0);
                            }
                            if (ProOperationNum == 0) {
                                listType = 0;
                                ProOperationNum = -1;
                                middleState = false;
                            }
                        } else {
                            if (middleState) {
                                tempIns = atoi(token);
                                programTexts_parseTwo[k_parseTwo].instruction = tempIns;
                                int firstDigit;
                                int lastThreeDigit;
                                firstDigit = tempIns/1000;
                                lastThreeDigit = tempIns % 1000;
                                if (strcmp(&programTexts_parseTwo[k_parseTwo].addType, "I") == 0) {
                                    if (tempIns > 9999) {
                                        programTexts_parseTwo[k_parseTwo].instruction = 9999;
                                        printf("%.3d: %i Error: Illegal immediate value; treated as 9999\n", k_parseTwo, programTexts_parseTwo[k_parseTwo].instruction);
                                    } else {
                                        printf("%.3d: %i\n", k_parseTwo, programTexts_parseTwo[k_parseTwo].instruction);
                                    }
                                } else if (strcmp(&programTexts_parseTwo[k_parseTwo].addType, "A") == 0){
                                    if (tempIns > 9999) {
                                        programTexts_parseTwo[k_parseTwo].instruction = 9999;
                                        printf("%.3d: %i Error: Illegal opcode; treated as 9999\n", k_parseTwo, programTexts_parseTwo[k_parseTwo].instruction);
                                    } else {
                                        if (lastThreeDigit > 512) {
                                            programTexts_parseTwo[k_parseTwo].instruction = firstDigit * 1000 + 0;
                                            printf("%.3d: %i Error: Absolute address exceeds machine size; zero used\n", k_parseTwo, programTexts_parseTwo[k_parseTwo].instruction);
                                        } else {
                                            
                                            printf("%.3d: %i\n", k_parseTwo, programTexts_parseTwo[k_parseTwo].instruction);
                                        }
                                    }
                                } else if (strcmp(&programTexts_parseTwo[k_parseTwo].addType, "E") == 0) {
                                    if (tempIns > 9999) {
                                        programTexts_parseTwo[k_parseTwo].instruction = 9999;
                                        printf("%.3d: %i Error: Illegal opcode; treated as 9999\n", k_parseTwo, programTexts_parseTwo[k_parseTwo].instruction);
                                    } else {
                                        if (lastThreeDigit > (declareCount - 1)) {
                                            printf("%.3d: %i Error: External address exceeds length of uselist; treated as immediate\n", k_parseTwo, programTexts_parseTwo[k_parseTwo].instruction);
                                        } else {
                                            
                                            if (lastThreeDigit >= 0 && lastThreeDigit < declareCount) {
                                                for (int j = 0; j < declareCount; j++) {
                                                    symbolLists[j_parseTwo - declareCount + j].usedState = true;
                                                }
                                            }
                                            
                                            for (int m = 0; m < i ; m++) {
                                            
                                                if (strcmp(&symbolLists[j_parseTwo - declareCount + lastThreeDigit], &symbolDefs[m].symbolName) == 0) {
                                                    programTexts_parseTwo[k_parseTwo].instruction = firstDigit * 1000 + symbolDefs[m].symbolAbsoluteAddress;
                                                    printf("%.3d: %i\n", k_parseTwo, programTexts_parseTwo[k_parseTwo].instruction);
                                                    foundState = true;
//                                                    symbolLists[j_parseTwo - declareCount + lastThreeDigit].usedState = true;
                                                }
                                            }
                                            
                                            
                                            if (foundState == false) {
                                                programTexts_parseTwo[k_parseTwo].instruction = firstDigit * 1000;
                                                printf("%.3d: %i Error: %s is not defined; zero used\n", k_parseTwo, programTexts_parseTwo[k_parseTwo].instruction, &symbolLists[j_parseTwo - declareCount + lastThreeDigit].symbolDeclare);
                                            }
                                        }
                                    }
                                } else if (strcmp(&programTexts_parseTwo[k_parseTwo].addType, "R") == 0) {
                                    if (tempIns > 9999) {
                                        programTexts_parseTwo[k_parseTwo].instruction = 9999;
                                        printf("%.3d: %i Error: Illegal opcode; treated as 9999\n", k_parseTwo, programTexts_parseTwo[k_parseTwo].instruction);
                                    } else {
                                        
                                        if (lastThreeDigit > totalLengthModule) {
                                            programTexts_parseTwo[k_parseTwo].instruction = firstDigit * 1000 + baseAdd_parseTwo;
                                            printf("%.3d: %i Error: Relative address exceeds module size; zero used\n", k_parseTwo, programTexts_parseTwo[k_parseTwo].instruction);
                                        } else {
                                            programTexts_parseTwo[k_parseTwo].instruction += baseAdd_parseTwo;
                                            printf("%.3d: %i\n", k_parseTwo, programTexts_parseTwo[k_parseTwo].instruction);
                                        }
                                    }
                                }
                                
                                ProOperationNum--;
                                k_parseTwo++;
                                middleState = false;
                                if (ProOperationNum == 0) {
                                    listType = 0;
                                    ProOperationNum = -1;
                                    middleState = false;
                                    //                                    moduleNumber++;
                                    prevTotalDefcount += defCount;
                                    //                                    prevTotalDeclareCount += declareCount;
                                    baseAdd_parseTwo += lengthModule;
                                    
                                }
                            } else {
                                strcpy(&programTexts_parseTwo[k_parseTwo].addType, token);
                                middleState = true;
                            }
                        }
                    }
                    
                    token = strtok(NULL, " ");
                    
                }
            } else {
                continue;
            }
        }
        
    }
    
    return 0;
}

int PrintWarning(){
    
    for (int s = 0; s < i; s++) {
        
        for (int j = 0; j <prevTotalDeclareCount; j++) {
            if (strcmp(&symbolLists[j].symbolDeclare, &symbolDefs[s].symbolName) == 0) {
                 symbolDefs[s].checkUseState = true;
            }
        }
        if (symbolDefs[s].checkUseState == false) {
            printf("Warning: Module %i: %s was defined but never used\n", symbolDefs[s].modulePostion, &symbolDefs[s].symbolName);
        } else {
            symbolDefs[s].checkUseState = false;
        }
    }
    
    
    return 0;
};


int useListState() {
    for(int j = 0; j < prevTotalDeclareCount; j++) {
        if (symbolLists[j].usedState == false) {
            printf("Warning: Module %i: %s appeared in the uselist but was not actually used", symbolLists[j].modulePosition, &symbolLists[j].symbolDeclare);
        }
    }
    return 0;
};



int removeTab() {
    char *pr = token, *pw = token;
    while (*pr) {
        *pw = *pr++;
        pw += (*pw != '\t');
    }
    *pw = '\0';
    
    return 0;
}