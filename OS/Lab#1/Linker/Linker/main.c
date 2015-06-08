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
struct symbol
{
    char *symbolName;
    char *symbolAddress;
};
struct symbol symbols[10];

int main() {
    FILE *file;
    file = fopen("/Users/Min/Development/NYU_Assignments/OS/Lab#1/labsamples/input-18", "r");
    
    int n;
    int m;
    char *defcount;
    
    //
    /* read definiton number */
    fscanf(file, "%s", &defcount);
    n = atoi(&defcount);
    printf("%s ", &defcount);
    
    /* read symbol defintion */
    for (int i = 0; i < n; i ++) {
        fscanf(file, "%s", &symbols[i].symbolName);
        fscanf(file, "%s", &symbols[i].symbolAddress);
        m = atoi(&symbols[i].symbolAddress);
        if (m == 0) {
            printf("NUM_EXPECTED");
            break;
        } else
        {
            printf("%s ", &symbols[i].symbolName);
            printf("%s ", &symbols[i].symbolAddress);
        }
    }
    
  
    
    fclose(file);
    return 0;
}
