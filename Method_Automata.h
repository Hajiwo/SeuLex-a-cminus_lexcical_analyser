#ifndef METHOD_AUTOMATA_H
#define METHOD_AUTOMATA_H

#include"NFA.h"
#include"DFA.h"
#include"Global_Val.h"


// int myDFA[100000][128];
// int sizeDFA;
// int sizeSymbols;
// char mySymbols[128];
/*
input: a string which is an RE in the form of postfix, and an int& to represent stateCounter to help distinguish states in different NFA 
output: a NFA's pointer 
*/
NFA* generate_NFA(Pattern* , int );

class method_mergeNFA{
private:
NFA* final_NFA;

//unordered_set

public:
method_mergeNFA();

/*
add NFA* to list waiting to merge
*/
void add_NFA_to_merge(NFA* );

/*
merge all the NFAs
*/
NFA* merge_NFAs();
};

/*
including methods to:
- generate DFA from NFA;
- optimize DFA;
- create search table for DFA; 
*/
class generate_DFA{
private:
DFA* DFA_p;     
DFA* opt_DFA;
NFA* input_NFA_p;
subset* deadState;
int stateCounter;

unordered_set<subset* >Partion;
unordered_set<char >symbol_Table;
//map<subset* , int>Dstates;
vector<subset*>Dstates; //using vector consumes a lot of space!

public:
//methods for generating DFA from NFA:

generate_DFA();

subset* E_closure(int );

subset* E_closure(subset* ); 

void generate_State(subset* );

subset* Move(char, subset*);

void renew_Dtran(subset*, char, subset*);

int if_in_DFAstates(subset* );

void generate_DFA_from_NFA(NFA* );

DFA* get_DFA();

//methods for optimizing DFA:

DFA* minimize_DFA();

void initialize_Partion();

int Move(int, char);

void merge_DFAstates(); // after partioned all subsets, we need to generate a new DFA

subset* find_group(int );

bool partion_Group(subset*);  //verify if the group can be partioned. If can, return true;

};

void clear_Automata(DFA*);
void clear_Automata(NFA*);

// class optimize_DFA{
//     private:
//     int stateCounter;
//     Partion _partion;
//     DFA* _DFA;
//     DFA* optimized_DFA;
//     unordered_set< char> symbol_Set;

//     public:
//     optimize_DFA(DFA*);
//     void _Optimize_DFA();
//     bool partion_Group(Group* );
//     bool if_in_Partion(Group* );
//     Group* find_Group(int );
//     int Move(int, char);
//     void generate_DFA();
//     DFA* get_DFA();
//     DFA_state* generate_State(Group*, char);
//     char get_group_type(Group* );
// };


#endif