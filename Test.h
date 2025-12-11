#ifndef TEST_H
#define TEST_H
#include"NFA.h"
#include"DFA.h"
#include"Test.h"
#include"Method_RE.h"
#include"Pattern.h"
#include"Global_Val.h"
#include"Method_Automata.h"
#include<iostream>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include<unordered_set>
#include<string>

class Test{
    public:
    void test_print_nextStates(NFA_State* );
    void print_NFA_info(NFA* );
    void Test_generate_NFA_from_RE_post(string RE_post);
    void Test_merge_NFAs();
    void Test_generate_DFA();
    void Test_print_DFA(DFA*, string );
    void Test_print_DFA(DFA* );
    void Test_print_NFA(NFA*, string );
    void Test_print_DFA_states(DFA_state*);
    void Test_generateDFA_from_RE_post(string*, int);
};


#endif