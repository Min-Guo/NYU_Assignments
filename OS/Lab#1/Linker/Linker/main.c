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

FILE *file;
int a = 0;
int b = 0;
int numModule = 0;
int scanValue;
int idefcount;
int m;
int prevDefcount;
int ideclareCount;
int prevDeclareCount;
int inumIns;
char *defcount;
char *declareCount;
char numInstructions[10][2];
//int inumIns[];
int NumIns[];
char tempIns[10];
char tempInsAdds[4];
char symbolDeclare[16][10];
int baseAddress;
int absoluteAddress[10];

/* define symbol table */
struct symbolDef
{
    char symbolName[16];
    char symbolAddress[4];
    int symbolAbsoluteAddress[4];
};
struct symbolDef symbolDefs[20];

struct symbolList
{
    char symbolDeclare[16];
};
struct symbolList symbolLists[10];

struct module
{
    int moduleNum;
    int lenModule;
};
struct module modules[10];


int main() {
    
    file = fopen("/Users/Min/Development/NYU_Assignments/OS/Lab#1/labsamples/input-19", "r");

    while (!feof(file)) {
        baseAddress = 0;
        numModule ++;
        for(int i = 0; i < numModule; i++){
            baseAddress += NumIns[i];
        }
        ReadDefList(file);
        ReadUseList(file);
        ReadInstructions(file);
        CheckReAdd();
        PrintSymbolTable();
    };
    fclose(file);
    return 0;
}

int ReadDefList(FILE *file) {
    /* read definiton number */
//    fscanf(file, "%s", &defcount);
    if ((scanValue = fscanf(file, "%s", &defcount))> 0){
        
    
//    printf("%i\n", scanValue);
        idefcount = atoi(&defcount);
        printf("%i ", idefcount);

    
    /* read symbol defintion */
    
        for (int i = 0; i < idefcount; i ++) {
            if ((scanValue = fscanf(file, "%s", &symbolDefs[i + prevDefcount].symbolName))> 0) {
                b++;
            } else {
                if ((b != 0) && (b < idefcount)) {
                    printf("TO_MANY_DEF_IN_MODULE");
                    exit(0);
                } else if (b ==0) {
                    printf("SYM_EXPECTED");
                    exit(0);
                }
            }
        
            fscanf(file, "%s", &symbolDefs[i + prevDefcount].symbolAddress);
            if (isalpha(&symbolDefs[i + prevDefcount].symbolAddress)) {
                printf("NUM_EXPECTED");
                exit(0);
            } else
            {
                printf("%s ", &symbolDefs[i + prevDefcount].symbolName);
                printf("%s ", &symbolDefs[i + prevDefcount].symbolAddress);
                
                absoluteAddress[i+ prevDefcount] = baseAddress + atoi(&symbolDefs[i + prevDefcount].symbolAddress);
                printf("\nabsoluteAddress is %i\n", absoluteAddress[i+ prevDefcount]);
//                strcpy(symbolDefs[i + prevDefcount].symbolAbsoluteAddress, 'absoluteAddress');
//                symbolDefs[i + prevDefcount].symbolAbsoluteAddress = absoluteAddress;
            }
        }
        prevDefcount  += atoi(&defcount);
    }
    printf("\n");
    return 0;
} ;

int ReadUseList(FILE *file){
    /* read symbol declaration */
    if ((scanValue = fscanf(file, "%s", &declareCount)) > 0){
        ideclareCount = atoi(&declareCount);
        printf("%s ", &declareCount);
    
        for (int i = 0; i < ideclareCount; i++) {
            if ((scanValue = fscanf(file, "%s", &symbolLists[i + prevDeclareCount].symbolDeclare)) > 0) {
                if (isdigit(&symbolLists[i + prevDeclareCount].symbolDeclare)) {
                    printf("SYM_EXPECTED");
                    exit(0);
                }else{
                
                    printf("%s ", &symbolLists[i + prevDeclareCount].symbolDeclare);
                    a ++;
                }
            }
        }
    
        if (a != 0 && a < ideclareCount) {
            printf("TO_MANY_USE_IN_MODULE");
        }
        prevDeclareCount += atoi(&declareCount);
    }
    
    printf("\n");
    return 0;
};

int ReadInstructions(FILE *file){
    /* read num-instructions */
    if ((scanValue = fscanf(file, "%s", &numInstructions[numModule-1])) >0 ){
//        printf("module lenth is %s\n", &numInstructions[numModule-1]);
//        inumIns[numModule-1] = atoi(&numInstructions[numModule-1]);
        NumIns[numModule-1] = atoi(&numInstructions[numModule-1]);
//        printf("integer numIns is %i\n", NumIns[numModule-1]);
        for (int i = 0; i < NumIns[numModule-1]; i++) {
//            fscanf(file, "%s", &tempIns);
            if ((scanValue = fscanf(file, "%s", &tempIns))> 0) {
                printf("%s ", &tempIns);
                fscanf(file, "%s ", &tempInsAdds);
                printf("%s ", &tempInsAdds);
            } else {
                printf("ADDR_EXPECTED");
                exit(0);
            }
        }
    }
    printf("\n");

    return 0;
};

/* Check the relative address is valid or not*/
int CheckReAdd(){
    for (int i = prevDefcount - (atoi(&defcount)) ; i < prevDefcount; i ++) {
        if (atoi(&symbolDefs[i].symbolAddress) > atoi(&numInstructions[numModule-1])) {
            
            printf("Warning: Module %i : %s to big %i (max=%i) assume zero relative.\n", numModule, &symbolDefs[i].symbolName, atoi(&symbolDefs[i].symbolAddress), (atoi(&numInstructions[numModule-1]) -1));
            strcpy(symbolDefs[i].symbolAddress, "0"); /* the value is printed before reassign, but reassign works. */
            printf("New value is %s\n", &symbolDefs[i].symbolAddress);
        }
    }
    return 0;
};

/* print symbol table*/

int PrintSymbolTable(){
    printf("Symbol Table\n");
    for (int i = 0; i < prevDefcount; i++) {
        printf("%s=%i\n", &symbolDefs[i].symbolName, absoluteAddress[i]);
    }
    
    return 0;
};

