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

/* define symbol table */
struct symbolDef
{
    char symbolName[16];
    char *symbolAddress;
};
struct symbolDef symbolDefs[10];

int main() {
    FILE *file;
    file = fopen("/Users/Min/Development/NYU_Assignments/OS/Lab#1/labsamples/input-1", "r");
    int a = 0;
    int b = 0;
    int idefcount;
    int m;
    int ideclareCount;
    int inumIns;
    char *defcount;
    char *declareCount;
    char *numInstructions;
    char *tempIns;
    char *tempInsAdds;
    char symbolDeclare[16][10];
    //
    /* read definiton number */
    fscanf(file, "%s", &defcount);
    idefcount = atoi(&defcount);
    printf("%s ", &defcount);
    
    /* read symbol defintion */
      
    for (int i = 0; i < idefcount; i ++) {
        if (fscanf(file, "%s", &symbolDefs[i].symbolName) != EOF) {
            printf("%s", &symbolDefs[i].symbolName);
            b++;
        } else {
            if ((b != 0) && (b < idefcount)) {
                printf("TO_MANY_DEF_IN_MODULE");
                break;
            } else if (b ==0) {
                printf("SYM_EXPECTED");
              break;
            }}
//        printf("%s ", &symbolDefs[i].symbolAddress);
        fscanf(file, "%s", &symbolDefs[i].symbolAddress);
        m = atoi(&symbolDefs[i].symbolAddress);
        if (m == 0) {
            printf("NUM_EXPECTED");
            break;
        } else
        {
            printf("%s ", &symbolDefs[i].symbolName);
            printf("%s ", &symbolDefs[i].symbolAddress);
        }
    }
    
    /* read symbol declaration */
    
    fscanf(file, "%s", &declareCount);
    ideclareCount = atoi(&declareCount);
    printf("\n%s ", &declareCount);

    for (int i = 0; i < ideclareCount; i++) {
        if (fscanf(file, "%s", &symbolDeclare[16][i]) != EOF) {
        printf("%s ", &symbolDeclare[16][i]);
            a ++;
        }
    }
    
        if (a < ideclareCount) {
            printf("TO_MANY_USE_IN_MODULE");
        }
    
    /* read num-instructions */
    
    fscanf(file, "%s", &numInstructions);
    printf("\n%s", &numInstructions);
    inumIns = atoi(&numInstructions);
    for (int i = 0; i < inumIns; i++) {
            fscanf(file, "%s", &tempIns);
            fscanf(file, "%s", &tempInsAdds);
        }
    fclose(file);
    return 0;
}

