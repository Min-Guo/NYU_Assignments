//
//  main.cpp
//  lab3
//
//  Created by Min Guo on 7/11/15.
//  Copyright (c) 2015 Min Guo. All rights reserved.
//

#include <iostream>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <vector>
using namespace std;

string lineBuffer;
char* line;
char* token;


struct instruction {
    int operation;
    bool operationState;
    int virtualPageIndex;
    bool virtualPageState;
};
vector<instruction> Inputs;

int readFile(){
    int i = 0;
    ifstream infile("/Users/Min/Development/NYU_Assignments/OS/Lab#3/lab3_assign/in1K4");
    if(!infile.is_open()){
        cout<<"Failed to open"<<endl;
    }
    else{
        while (!infile.eof()) {
            getline(infile, lineBuffer);
            if (lineBuffer.find("#") != 0) {
                line = new char[lineBuffer.length() + 1];
                strcpy(line, lineBuffer.c_str());
                token = strtok(line, "\n");
                token = strtok(line, " ");
                Inputs.push_back(instruction());
                while (token!= NULL) {
                    if (Inputs[i].operationState == false) {
                        Inputs[i].operation = atoi(token);
                        Inputs[i].operationState = true;
                        cout<<Inputs[i].operation<<" ";
                    }
                    else
                    {
                        Inputs[i].virtualPageIndex = atoi(token);
                        cout<<Inputs[i].virtualPageIndex<< endl;
                    }
                    token = strtok(NULL, " ");
                }
                i++;
            }
        }
    }

    return 0;
};


int main(int argc, const char * argv[]) {
    readFile();
    return 0;
}
