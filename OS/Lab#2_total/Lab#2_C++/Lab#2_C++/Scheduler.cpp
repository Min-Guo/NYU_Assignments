//
//  Scheduler.cpp
//  Lab#2_C++
//
//  Created by Min Guo on 6/21/15.
//  Copyright (c) 2015 Min Guo. All rights reserved.
//

#include <stdio.h>
#include "Scheduler.h"




void Scheduler::put_event(Process process){
    pq.push(process);
}

Process Scheduler::get_event(){
    Process process = pq.top();
    pq.pop();
    return process;
}