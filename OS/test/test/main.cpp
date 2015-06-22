//
//  main.cpp
//  test
//
//  Created by Min Guo on 6/21/15.
//  Copyright (c) 2015 Min Guo. All rights reserved.
//

#include <iostream>
#include <queue>
#include <iomanip>

using namespace std;

struct Time {
    int h;
    int m;
    int s;
};

class CompareTime {
public:
    bool operator() (Time& t1, Time& t2) {
        if (t1.h > t2.h) return true;
        if (t1.h == t2.h && t1.m > t2.m) return true;
        return false;
    }
};

int main() {
    std::priority_queue<Time, vector<Time>, CompareTime> pq;
    Time t[4] = { {3, 2, 40}, {3, 2, 26}, {5, 16, 13}, {5, 14, 20}};
    for (int i =0; i < 4; i++)
        pq.push(t[i]);
        
    
    while (!pq.empty()) {
        Time t = pq.top();
        cout << setw(3) << t.h << " " << setw(3) << t.m << " " <<
        setw(3) << t.s << endl;
        pq.pop();
    }
    
    return 0;
}
