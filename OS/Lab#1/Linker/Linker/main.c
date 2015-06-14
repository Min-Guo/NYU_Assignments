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
int j = 0;
int k = 0;
/* define symbol table */
struct symbolDef
{
    char symbolName[16];
    char symbolAddress[4];
    int symbolAbsoluteAddress;
};
struct symbolDef symbolDefs[256] = {NULL};

struct symbolList
{
    char symbolDeclare[16];
};
struct symbolList symbolLists[256];

struct programText
{
    char addType;
    int instruction;
};
struct programText programTexts[256];




int main() {

    
    file = fopen("/Users/Min/Development/NYU_Assignments/OS/Lab#1/labsamples/input-1", "r");
//    ParseOne(file);
    ParseTwo(file);

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
            if (strcmp(line_buffer,  "\n") != 0) {
                
                token = strtok(line_buffer, "\n");
                token = strtok(line_buffer, " ");
                while( token!= NULL )
                {
                    if (listType == 0) {
                        if (defOperationNum == -1) {
                            defOperationNum = atoi(token);
                            defCount = defOperationNum;
                            if (defOperationNum > 16) {
                                printf("Parse Error line %i: TO_MANY_DEF_IN_MODULE", lineNum);
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
                                        printf("Parse Error line %i: NUM_EXPECTED", lineNum);
                                        exit(0);
                                    }
                                }
                                
                                //                                    printf("Symbol[%i]Address is % s\n", i,  &symbolDefs[i].symbolAddress);
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
                                printf("Parse Error line %i: TO_MANY_USE_IN_MODULE", lineNum);
                                exit(0);
                            } else if (operationNum == 0) {
                                listType = 2;
                                operationNum = -1;
                            }
                        } else {
                            strcpy(&symbolLists[prevTotalDeclareCount + j].symbolDeclare, token);
                            if (!isalpha(symbolLists[prevTotalDeclareCount + j].symbolDeclare[0])) {
                                printf("Parse Error line %i: SYM_EXPECTED", lineNum);
                                exit(0);
                            } else{
                                //                                    printf("SymbolList[%i] is %s ", j, &symbolLists[prevTotalDeclareCount + j].symbolDeclare);
                                operationNum--;
                                j++;
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
                                printf("Parse Error line %i: TO_MANY_INSTR", lineNum);
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
                                    moduleNumber++;
                                    prevTotalDefcount += defCount;
                                    prevTotalDeclareCount += declareCount;
                                    
                                }
                            } else {
                                strcpy(&programTexts[k].addType, token);
                                //                                    printf("\n");
                                //                                    printf("AddType[%i] is %s ", k, &programTexts[k].addType);
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
    if (feof(file) && (defOperationNum != -1)) {
        printf("Parse Error line %i: SYM_EXPECTED", lineNum);
        exit(0);
    }
    if (feof(file) && (ProOperationNum != -1)) {
        printf("Parse Error line %i: ADDR_EXPECTED", lineNum);
        exit(0);
    }
    PrintSymbolTable();
    return 0;
}







int ParseTwo(FILE* file){
    while (!feof(file)) {
        if(fgets(line_buffer, 512, file)!= NULL) {
            lineNum ++;
            if (strcmp(line_buffer,  "\n") != 0) {
                
                token = strtok(line_buffer, "\n");
                token = strtok(line_buffer, " ");
                while( token!= NULL )
                {
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
                                strcpy(&symbolDefs[i].symbolAddress, token);
                                for (int t =0; t < 4; t++) {
                                    
                                    if (!isdigit(symbolDefs[i].symbolAddress[t]) && (symbolDefs[i].symbolAddress[t]!= NULL)) {
//                                        printf("Parse Error line %i: NUM_EXPECTED", lineNum);
                                        exit(0);
                                    }
                                }
                                
                                //                                    printf("Symbol[%i]Address is % s\n", i,  &symbolDefs[i].symbolAddress);
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
                            strcpy(&symbolLists[prevTotalDeclareCount + j].symbolDeclare, token);
                            if (!isalpha(symbolLists[prevTotalDeclareCount + j].symbolDeclare[0])) {
//                                printf("Parse Error line %i: SYM_EXPECTED", lineNum);
                                exit(0);
                            } else{
                                //                                    printf("SymbolList[%i] is %s ", j, &symbolLists[prevTotalDeclareCount + j].symbolDeclare);
                                operationNum--;
                                j++;
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
//                                printf("Parse Error line %i: TO_MANY_INSTR", lineNum);
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
                                tempIns = atoi(token);
                                int firstDigit;
                                int lastThreeDigit;
                                firstDigit = tempIns/1000;
                                lastThreeDigit = tempIns % 1000;
                                if (strcmp(&programTexts[k].addType, "I") == 0) {
                                    printf("I %i is %s\n",k, &programTexts[k].instruction);
                                } else if (strcmp(&programTexts[k].addType, "A") == 0){
                                    if (lastThreeDigit > 512) {
                                        programTexts[k].instruction = firstDigit * 1000 + 0;
                                        printf("%i  Error: Absolute address exceeds machine size; zero used\n", programTexts[k].instruction);
                                    } else {
                                        printf("%s\n", &programTexts[k].instruction);
                                    }
                                } else if (strcmp(&programTexts[k].addType, "E") == 0) {
                                    if (lastThreeDigit > (declareCount - 1)) {
                                        printf("Error: External address exceeds length of uselist; treated as immediate\n");
                                    } else {
                                        printf("%s\n", &programTexts[k].instruction);
                                    }
                                } else if (strcmp(&programTexts[k].addType, "R") == 0) {
                                    programTexts[k].instruction += baseAddress;
                                    printf("baseaddress is %i  ,R is %i\n", baseAddress, programTexts[k].instruction);
                                }
                                
//                                                                    printf("AddType is %s   Instrution is %s\n", &programTexts[k].addType,  &programTexts[k].instruction);
                                ProOperationNum--;
                                k++;
                                middleState = false;
                                if (ProOperationNum == 0) {
                                    listType = 0;
                                    ProOperationNum = -1;
                                    middleState = false;
                                    moduleNumber++;
                                    prevTotalDefcount += defCount;
                                    prevTotalDeclareCount += declareCount;
                                    
                                }
                            } else {
                                strcpy(&programTexts[k].addType, token);
                                //                                    printf("\n");
                                //                                    printf("AddType[%i] is %s ", k, &programTexts[k].addType);
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
    if (feof(file) && (defOperationNum != -1)) {
//        printf("Parse Error line %i: SYM_EXPECTED", lineNum);
        exit(0);
    }
    if (feof(file) && (ProOperationNum != -1)) {
//        printf("Parse Error line %i: ADDR_EXPECTED", lineNum);
        exit(0);
    }
//    PrintSymbolTable();
    return 0;
}