#ifndef PATTERN_H
#define PATTERN_H

#include<string>
#include<vector>
using namespace std;
struct Pattern {
    string RE_content;
    string Action;
    //vector< string> Actions;
    int priority; //Smaller the value, higher the priority 
};

#endif