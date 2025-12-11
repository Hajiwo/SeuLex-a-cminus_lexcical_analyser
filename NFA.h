#ifndef NFA_H
#define NFA_H
#include"Pattern.h"
#include <unordered_map>
#include <map>
//NFA_state:
struct NFA_State{
    int number;                                         //The number of this state
    std::unordered_multimap< char,  int>Next_State;     //move(a,s)
};


//NFA:
struct NFA{
    int Start_State;                                //let it be 0 in default
    int End_State_tmp;                              //temporary end state during construction process 
    std::map< int, Pattern>Final_States;            //Final states that have the related patterns
    std::unordered_map< int, NFA_State>States_Map;  //The Map to record all the states in the NFA
};

#endif