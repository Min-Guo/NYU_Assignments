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
int scanValue;
int idefcount;
int m;
int ideclareCount;
int inumIns;
char *defcount;
char *declareCount;
char *numInstructions;
char tempIns[10];
char tempInsAdds[4];
char *symbolDeclare[16][10];

/* define symbol table */
struct symbolDef
{
    char symbolName[16];
    char *symbolAddress;
};
struct symbolDef symbolDefs[10];

int main() {
    
    file = fopen("/Users/Min/Development/NYU_Assignments/OS/Lab#1/labsamples/input-19", "r");

    while (!feof(file)) {
        ReadDefList(file);
        ReadUseList(file);
        ReadInstructions(file);
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
            if ((scanValue = fscanf(file, "%s", &symbolDefs[i].symbolName))> 0) {
//              fscanf(file, "%s", &symbolDefs[i].symbolName);
                b++;
            } else {
                if ((b != 0) && (b < idefcount)) {
                    printf("TO_MANY_DEF_IN_MODULE");
                    break;
                } else if (b ==0) {
                    printf("SYM_EXPECTED");
                    break;
                }
            }
        
            fscanf(file, "%s", &symbolDefs[i].symbolAddress);
            m = atoi(&symbolDefs[i].symbolAddress);
            if (isalpha(&symbolDefs[i].symbolAddress)) {
                printf("NUM_EXPECTED");
                break;
            } else
            {
                printf("%s ", &symbolDefs[i].symbolName);
                printf("%s ", &symbolDefs[i].symbolAddress);
            }
        }
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
            if ((scanValue = fscanf(file, "%s", &symbolDeclare[16][i])) > 0) {
                
                    printf("%s ", &symbolDeclare[16][i]);
                    a ++;
            }
        }
    
        if (a != 0 && a < ideclareCount) {
            printf("TO_MANY_USE_IN_MODULE");
        }
    }
    printf("\n");
    return 0;
};

int ReadInstructions(FILE *file){
    /* read num-instructions */
    if ((scanValue = fscanf(file, "%s", &numInstructions)) >0 ){
        printf("%s ", &numInstructions);
        inumIns = atoi(&numInstructions);
        for (int i = 0; i < inumIns; i++) {
//            fscanf(file, "%s", &tempIns);
            if ((scanValue = fscanf(file, "%s", &tempIns))> 0) {
                printf("%s ", &tempIns);
                fscanf(file, "%s ", &tempInsAdds);
                printf("%s ", &tempInsAdds);
            } else {
                printf("ADDR_EXPECTED");
                break;
            }
            
        }
    }
    printf("\n");

    return 0;
};

