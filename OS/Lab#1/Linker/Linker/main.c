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
        CalculateAbAddress();
    };
    PrintSymbolTable();
    fclose(file);
    return 0;
}

int ReadDefList(FILE *file) {
    /* read definiton number */
    if ((scanValue = fscanf(file, "%s", &defcount))> 0){

        idefcount = atoi(&defcount);
        
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
            }
//            } else
//            {
                
//                absoluteAddress[i+ prevDefcount] = baseAddress + atoi(&symbolDefs[i + prevDefcount].symbolAddress);
//                for (int j = 0; j < i + prevDefcount; j++)
//                {
//
//                    if (strcmp(&symbolDefs[j].symbolName, &symbolDefs[i + prevDefcount].symbolName) == 0) {
//                        absoluteAddress[i+ prevDefcount] = absoluteAddress[j];
//                    }
//                }

//            }
        }
//        prevDefcount  += atoi(&defcount);
    }
    return 0;
} ;

int ReadUseList(FILE *file){
    /* read symbol declaration */
    if ((scanValue = fscanf(file, "%s", &declareCount)) > 0){
        ideclareCount = atoi(&declareCount);
    
        for (int i = 0; i < ideclareCount; i++) {
            if ((scanValue = fscanf(file, "%s", &symbolLists[i + prevDeclareCount].symbolDeclare)) > 0) {
                if (isdigit(&symbolLists[i + prevDeclareCount].symbolDeclare)) {
                    printf("SYM_EXPECTED");
                    exit(0);
                }else{
                
                    a ++;
                }
            }
        }
    
        if (a != 0 && a < ideclareCount) {
            printf("TO_MANY_USE_IN_MODULE");
        }
        prevDeclareCount += atoi(&declareCount);
    }
    
    return 0;
};

int ReadInstructions(FILE *file){
    /* read num-instructions */
    if ((scanValue = fscanf(file, "%s", &numInstructions[numModule-1])) >0 ){

        NumIns[numModule-1] = atoi(&numInstructions[numModule-1]);
        for (int i = 0; i < NumIns[numModule-1]; i++) {
            if ((scanValue = fscanf(file, "%s", &tempIns))> 0) {
                fscanf(file, "%s ", &tempInsAdds);
            } else {
                printf("ADDR_EXPECTED");
                exit(0);
            }
        }
    }
    return 0;
};

/* Check the relative address is valid or not*/
int CheckReAdd(){
    for (int i = prevDefcount ; i < atoi(&defcount); i ++) {
        if (atoi(&symbolDefs[i].symbolAddress) > atoi(&numInstructions[numModule-1])) {
            
            printf("Warning: Module %i : %s to big %i (max=%i) assume zero relative.\n", numModule, &symbolDefs[i].symbolName, atoi(&symbolDefs[i].symbolAddress), (atoi(&numInstructions[numModule-1]) -1));
            strcpy(symbolDefs[i].symbolAddress, "0"); /* the value is printed before reassign, but reassign works. */

        }
    }
    return 0;
};

/* Absolute Address */

int CalculateAbAddress(){
    for (int i = 0; i < idefcount; i ++) {
        absoluteAddress[i+ prevDefcount] = baseAddress + atoi(&symbolDefs[i + prevDefcount].symbolAddress);
//        for (int j = 0; j < i + prevDefcount; j++)
//            {
//        
//                if (strcmp(&symbolDefs[j].symbolName, &symbolDefs[i + prevDefcount].symbolName) == 0) {
//                    absoluteAddress[i+ prevDefcount] = absoluteAddress[j];
//                }
//            }
    }
    prevDefcount  += atoi(&defcount);
    return 0;
};

/* print symbol table*/

int PrintSymbolTable(){
    printf("Symbol Table\n");
    for(int j = 0; j < prevDefcount; j++) {
        for (int k = j + 1; k < prevDefcount; k++) {
            if(strcmp(&symbolDefs[j].symbolName, &symbolDefs[k].symbolName)== 0){
                strcpy(&symbolDefs[k].symbolName, &symbolDefs[k+1].symbolName);
                strcpy(&symbolDefs[k].symbolAddress, &symbolDefs[k+1].symbolAddress);
                absoluteAddress[k]= absoluteAddress[k+1];
                prevDefcount -= 1;
                printf("%s=%i Error: This variable is multiple times defined; first value used\n", &symbolDefs[j].symbolName, absoluteAddress[j]);
            }
        }
        printf("%s=%i\n", &symbolDefs[j].symbolName, absoluteAddress[j]);
    }

    
    return 0;
};

