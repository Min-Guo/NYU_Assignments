//
//  Scheduler.cpp
//  Lab#2_C++
//
//  Created by Min Guo on 6/21/15.
//  Copyright (c) 2015 Min Guo. All rights reserved.
//

#include <stdio.h>
#include "Scheduler.h"



void Scheduler::put_eventqueue(Process process){
    event_queue.push(process);
}

Process Scheduler::decreasePriority(Process* process){
    return *process;
};

Process Scheduler::checkFirstEvent(){
    Process process = event_queue.top();
    return process;
}

bool Scheduler::expiredEmpty() {
    return true;
}

Process Scheduler::get_eventqueue(){
    Process process = event_queue.top();
    event_queue.pop();
    return process;
}

Process FCFSScheduler::decreasePriority(Process* process){
    return *process;
}

void FCFSScheduler::put_readyqueue(Process process){
    ready_queue.push(process);
}

void FCFSScheduler::switchPointer(){
};

Process FCFSScheduler::get_readyqueue(){
    Process process = ready_queue.top();
    ready_queue.pop();
    return process;
}


bool FCFSScheduler::bothEmpty() {
    if (!event_queue.empty() || !ready_queue.empty()) return false;
    return true;
}


bool FCFSScheduler::isReady(double time) {
    if (event_queue.top().AT <= time) return true;
    return false;
}

bool FCFSScheduler::expiredEmpty() {
    return true;
}

void FCFSScheduler::put_expiredqueue(Process process){
    expired_queue.push(process);
}

bool FCFSScheduler::readyEmpty(){
    if (ready_queue.empty()) return true;
    return false;
}
Process LCFSScheduler::decreasePriority(Process* process){
    return *process;
}

void LCFSScheduler::switchPointer(){
};

void LCFSScheduler::put_readyqueue(Process process){
    ready_queue.push(process);
}


Process LCFSScheduler::get_readyqueue(){
    Process process = ready_queue.top();
    ready_queue.pop();
    return process;
}


bool LCFSScheduler::bothEmpty() {
    if (!event_queue.empty() || !ready_queue.empty()) return false;
    return true;
}


bool LCFSScheduler::isReady(double time) {
    if (event_queue.top().AT <= time) return true;
    return false;
}

void LCFSScheduler::put_expiredqueue(Process process){
    expired_queue.push(process);
}

bool LCFSScheduler::expiredEmpty() {
    return true;
}

bool LCFSScheduler::readyEmpty(){
    if (ready_queue.empty()) return true;
    return false;
}

Process SJFScheduler::decreasePriority(Process* process){
    return *process;
}

void SJFScheduler::switchPointer(){
};


void SJFScheduler::put_readyqueue(Process process){
    ready_queue.push(process);
}


Process SJFScheduler::get_readyqueue(){
    Process process = ready_queue.top();
    ready_queue.pop();
    return process;
}


bool SJFScheduler::bothEmpty() {
    if (!event_queue.empty() || !ready_queue.empty()) return false;
    return true;
}


bool SJFScheduler::isReady(double time) {
    if (event_queue.top().AT <= time) return true;
    return false;
}

bool SJFScheduler::expiredEmpty() {
    return true;
}

bool SJFScheduler::readyEmpty(){
    if (ready_queue.empty()) return true;
    return false;
}

void SJFScheduler::put_expiredqueue(Process process){
    expired_queue.push(process);
}

void PRIOScheduler::put_readyqueue(Process process){
    ready_queue.push(process);
}

void PRIOScheduler::put_expiredqueue(Process process){
    expired_queue.push(process);
}


Process PRIOScheduler::get_readyqueue(){
    Process process = ready_queue.top();
    ready_queue.pop();
    return process;
}


bool PRIOScheduler::bothEmpty() {
    if (!event_queue.empty() || !ready_queue.empty()) return false;
    return true;
}

bool PRIOScheduler::expiredEmpty() {
    if (!expired_queue.empty()) return false;
    return true;
}

Process PRIOScheduler::decreasePriority(Process* process){
    process->priority = process->priority - 1;
    return *process;
}

bool PRIOScheduler::isReady(double time) {
    if (event_queue.top().AT <= time) return true;
    return false;
}

void PRIOScheduler::switchPointer(){
    std::swap(ready_queue, expired_queue);
};

bool PRIOScheduler::readyEmpty(){
    if (ready_queue.empty()) return true;
    return false;
}

bool Scheduler::eventEmpty(){
    if (event_queue.empty()) return true;
    return false;
}



