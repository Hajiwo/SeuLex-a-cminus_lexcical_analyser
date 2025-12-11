#ifndef DFA_H
#define DFA_H
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include"Pattern.h"
#include"NFA.h"
#include"Global_Val.h"
using namespace std;


struct DFA_state{
int state_number;
unordered_set< int>subset_of_NFA;
unordered_map< char, int>next_State;
};

struct DFA{
int start_stateNumber;
int Dtran[1000][128]={-1};
int Dsize;
unordered_map< int, Pattern>Accepting_States;
unordered_map< int, DFA_state>states_Map;
};


#endif