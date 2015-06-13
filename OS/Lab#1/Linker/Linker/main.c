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
char ch;
int a = 0;
int b = 0;
int numModule = 0;
int scanValue;
int idefcount;
int m;

int ideclareCount;
int prevDeclareCount;
int inumIns;

//int inumIns[];
int NumIns[512];
char tempIns[10];
char tempInsAdds[4];
//char symbolDeclare[16][10];
int baseAddress = 0;
int absoluteAddress[10];
int prevNumIns = 0;

char line_buffer[512];
char *token;
char tokenBuffer[256][2];
int lineNum = 0;
int listType = 0;
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



/* define symbol table */
struct symbolDef
{
    char symbolName[16];
    char symbolAddress[4];
    int symbolAbsoluteAddress[4];
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
struct programText programTexts[];


struct module
{
    int moduleNum;
    int lenModule;
};
struct module modules[10];

int main() {
    int i = 0;
    int j = 0;
    int k = 0;
    
    file = fopen("/Users/Min/Development/NYU_Assignments/OS/Lab#1/labsamples/input-15", "r");

    while (!feof(file)) {
        if(fgets(line_buffer, 512, file)!= NULL) {
            lineNum ++;
            if (strcmp(line_buffer,  "\n") != 0) {
                
                token = strtok(line_buffer, "\n");
                token = strtok(line_buffer, " ");
                while( token!= NULL )
                    {
                        if (listType == 0) {
                            if (operationNum == -1) {
                                operationNum = atoi(token);
                                defCount = operationNum;
                                if (operationNum > 16) {
                                    printf("Parse Error line %i: TO_MANY_DEF_IN_MODULE", lineNum);
                                    exit(0);
//                                printf("OperationNum is %i\n", operationNum);
                                } else if (operationNum == 0) {
                                    listType = 1;
                                    operationNum = -1;
                                    middleState = false;
                                }
                        
                            } else {
                                if (middleState) {
                                    strcpy(&symbolDefs[i].symbolAddress, token);
                                    printf("Symbol[%i]Address is % s\n", i,  &symbolDefs[i].symbolAddress);
                                    operationNum-- ;
                                    middleState = false;
                                    i++;
                                    if (operationNum == 0) {
                                        listType = 1;
                                        operationNum = -1;
                                        middleState = false;
                                    }
                                } else {
                                    strcpy(&symbolDefs[i].symbolName, token);
                                    printf("Symbol[%i]Name is %s ", i,  &symbolDefs[i].symbolName);
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
//                                    if (!isalpha(symbolLists[prevTotalDeclareCount + j].symbolDeclare[0])) {
//                                        printf("Parse Error line %i: SYM_EXPECTED", lineNum);
//                                        exit(0);
//                                    } else{
                                    printf("SymbolList[%i] is %s ", j, &symbolLists[prevTotalDeclareCount + j].symbolDeclare);
                                    operationNum--;
                                    j++;
                                    if (operationNum == 0) {
                                        listType = 2;
                                        operationNum = -1;
                                    }
                                }

                            
                        } else if (listType == 2) {
                            if (operationNum == -1) {
                                operationNum = atoi(token);
                                lengthModule = operationNum;
                                baseAddress += operationNum;
                                if (operationNum == 0) {
                                    listType = 0;
                                    operationNum = -1;
                                    middleState = false;
                                }
                            } else {
                                if (middleState) {
                                    strcpy(&programTexts[k].instruction, token);
                                    printf("Instrution[%i] is %s\n", k, &programTexts[k].instruction);
                                    operationNum--;
                                    k++;
                                    middleState = false;
                                    if (operationNum == 0) {
                                        listType = 0;
                                        operationNum = -1;
                                        middleState = false;
                                        moduleNumber++;
                                        CheckReAdd();
                                        prevTotalDefcount += defCount;
                                        prevTotalDeclareCount += declareCount;
                                        
                                    }
                                } else {
                                    strcpy(&programTexts[k].addType, token);
                                    printf("\n");
                                    printf("AddType[%i] is %s ", k, &programTexts[k].addType);
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

//int ReadDefList(FILE *file) {
//    int tokenNum = 0;
//    fgets(line_buffer, 512, file);
//    lineNum ++;
//    printf("%s", &line_buffer);
//    token = strtok(line_buffer, "\n");
//    token = strtok(line_buffer, " ");
//    idefcount = atoi(token);
//    
//    if (idefcount > 16) {
//        printf("TO_MANY_DEF_IN_MODULE");
//        exit(0);
//    }
//    else {
//        for (int i = 0; i < idefcount; i ++) {
//            token = strtok(NULL, " ");
//            if (token != NULL)
//            {
////                token = strtok(NULL, " ");
//                strcpy(&symbolDefs[i + prevDefcount].symbolName, token);
//                printf("symbolname is %s\n", &symbolDefs[i + prevDefcount].symbolName);
//                token = strtok(NULL, " ");
//                if (token != NULL) {
//                    strcpy(&symbolDefs[i + prevDefcount].symbolAddress, token);
//                    printf("symbolAdd is %s", &symbolDefs[i + prevDefcount].symbolAddress);
//                    for (int j =0; j < 4; j++) {
//                        if (!isdigit(symbolDefs[i + prevDefcount].symbolAddress[j]) && (symbolDefs[i + prevDefcount].symbolAddress[j]!= NULL)) {
//                            printf("Parse Error line %i: NUM_EXPECTED", lineNum);
//                            exit(0);
//                        }
//                    }
//                } else {
//                    fgets(line_buffer, 512, file);
//                    lineNum ++;
//                    token = strtok(line_buffer, "\n");
//                    token = strtok(NULL, " ");
//                    if (token != NULL)
//                }
//            } else {
//                printf("Parse Error line %i: SYM_EXPECTED", lineNum);
//                exit(0);
// 
//            }
//        }
//    }

//
//            for (int i = 0; i < idefcount; i ++) {
//                if ((scanValue = fscanf(file, "%s", &symbolDefs[i + prevDefcount].symbolName))> 0) {
//                    fscanf(file, "%s", &symbolDefs[i + prevDefcount].symbolAddress);
//                    for (int j =0; j < 4; j++) {
//                        if (!isdigit(symbolDefs[i + prevDefcount].symbolAddress[j]) && (symbolDefs[i + prevDefcount].symbolAddress[j]!= NULL)) {
//                            printf("NUM_EXPECTED");
//                            exit(0);
//                        }
//                    }
//                } else {
//                    printf("SYM_EXPECTED");
//                    exit(0);
//                    }
//                }
//            }
//        }
//    
//    return 0;
//} ;
//
//int ReadUseList(FILE *file){
//    /* read symbol declaration */
//    if ((scanValue = fscanf(file, "%s", &declareCount)) > 0){
//        ideclareCount = atoi(&declareCount);
//        if (ideclareCount > 16) {
//            printf("TO_MANY_USE_IN_MODULE");
//            exit(0);
//        } else {
//            for (int i = 0; i < ideclareCount; i++) {
//                if ((scanValue = fscanf(file, "%s", &symbolLists[i + prevDeclareCount].symbolDeclare)) > 0) {
//                    if (!isalpha(symbolLists[i + prevDeclareCount].symbolDeclare[0])) {
//                        printf("SYM_EXPECTED");
//                        exit(0);
//                    }
//                }
//            }
//        }
//    
//        prevDeclareCount += atoi(&declareCount);
//    }
//    
//    return 0;
//};
//
//int ReadInstructions(FILE *file){
//    /* read num-instructions */
//    if ((scanValue = fscanf(file, "%s", &numInstructions[numModule-1])) >0 ){
//
//        NumIns[numModule-1] = atoi(&numInstructions[numModule-1]);
//        prevNumIns += atoi(&numInstructions[numModule-1]);
//        if (prevNumIns >512) {
//            printf("TO_MANY_INSTR\n");
//            exit(0);
//        } else {
//            for (int i = 0; i < NumIns[numModule-1]; i++) {
//                if ((scanValue = fscanf(file, "%s", &tempIns))> 0) {
//                    fscanf(file, "%s ", &tempInsAdds);
//                } else {
//                    printf("ADDR_EXPECTED");
//                    exit(0);
//                }
//            }
//        }
//    }
//    return 0;
//};
//
/* Check the relative address is valid or not*/

//
///* Absolute Address */
//
//int CalculateAbAddress(){
//    for (int i = 0; i < idefcount; i ++) {
//        absoluteAddress[i+ prevDefcount] = baseAddress + atoi(&symbolDefs[i + prevDefcount].symbolAddress);
//    }
//    prevDefcount  += atoi(&defcount);
//    return 0;
//};
//
///* print symbol table*/
//
//int PrintSymbolTable(){
//    printf("Symbol Table\n");
//    for(int j = 0; j < prevDefcount; j++) {
//        for (int k = j + 1; k < prevDefcount; k++) {
//            if(strcmp(&symbolDefs[j].symbolName, &symbolDefs[k].symbolName)== 0){
//                strcpy(&symbolDefs[k].symbolName, &symbolDefs[k+1].symbolName);
//                strcpy(&symbolDefs[k].symbolAddress, &symbolDefs[k+1].symbolAddress);
//                absoluteAddress[k]= absoluteAddress[k+1];
//                prevDefcount -= 1;
//                printf("%s=%i Error: This variable is multiple times defined; first value used\n", &symbolDefs[j].symbolName, absoluteAddress[j]);
//            }
//        }
//        printf("%s=%i\n", &symbolDefs[j].symbolName, absoluteAddress[j]);
//    }
//
//    
//    return 0;
//};
//
