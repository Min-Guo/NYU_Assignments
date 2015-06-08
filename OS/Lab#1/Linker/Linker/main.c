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


int main() {
    FILE *file;
    file = fopen("/Users/Min/Development/NYU_Assignments/OS/Lab#1/labsamples/input-2", "r");
    
    int n;
    int m;
    char *defcount;
    //    char symbol[n];
    char symbolName[16];
    char symbol;
    char *sAddress;
    
    //
    /* read definiton number */
    fscanf(file, "%s", &defcount);
    n = atoi(&defcount);
    printf("%s ", &defcount);
    
    /* read symbol defintion */
    for (int i = 0; i < n; i ++) {
        fscanf(file, "%s", &symbolName);
        fscanf(file, "%s", &sAddress);
        m = atoi(&sAddress);
        if (m == 0) {
            printf("NUM_EXPECTED");
        } else
        {
            printf("%s ", &symbolName);
            printf("%s ", &sAddress);
        }
    }
    
    //    do
    //    {
    //        c = fgetc(file);
    //        if(isdigit(c)){
    //            defcount = c - '0';
    //            printf("%i", defcount);
    //
    //        }
    //        else {
    //            printf("Error!");
    //        }
    //    }
    //    while (c != EOF);
    
    /*read defintion number */
    //    do {
    //        c = fgetc(file);
    //        if ((c >= '0') && (c <= '9')) {
    ////            int j = 0;
    //            defcount = c - '0';
    ////            j++;
    //            printf("%i ", defcount);
    //        }
    //    }
    ////        else {
    ////            printf("Number expected!");}
    ////        }
    //        while (!isspace(c)); /* read one more character */
    //
    
    ////
    ///* read symbol */
    //    for (int i = 0; i < defcount; i ++) {
    //        do {
    //            c = fgetc(file);
    //            if (isalpha(c)) {
    //                symbolName = &c;
    //            } else {
    //                printf("Symbol expected!");
    //            }
    //        } while (!isspace(c));
    //
    //        do {
    //            c = fgetc(file);
    //            if (isnumber(c)) {
    //                int temp = c -'0';
    //                sAddress = &temp;
    //            }
    //        } while (!isspace(c));
    //    
    //    }
    
    
    fclose(file);
    return 0;
}
