#include"NFA.h"
#include"DFA.h"
#include"Read_Lex.h"
#include"Test.h"
#include"Method_RE.h"
#include"Pattern.h"
#include"Global_Val.h"
#include"Method_Automata.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <unordered_set>

using namespace std;

void Test::print_NFA_info(NFA* p_NFA){
       cout<<"\n\nstart to test NFA:\n";
       cout<<"Start state number:  "<<p_NFA->Start_State<<endl;
       if(p_NFA->End_State_tmp != MULTI_ACCEPTING_STATES)
       cout<<"End state number: "<<p_NFA->End_State_tmp<<endl;
       else{
        for(const auto& pair: p_NFA->Final_States){
           cout<<"-Accepting state "<<pair.first<<endl;
        }
       }
       cout<<"State set: {\n";
       auto state_map = p_NFA->States_Map;
         for (const auto& pair : state_map) {
        std::cout <<"--state: "<< pair.first <<":"<< std::endl;
        NFA_State* state = &p_NFA->States_Map[pair.first];
        test_print_nextStates(state);
    }
    cout<<"}\n";
      
}
    

void Test::Test_generate_NFA_from_RE_post(string RE_post){
       NFA* testNFA;
       Pattern* testRE = new Pattern();
       testRE->RE_content = RE_post;
       int stateCounter=1;
       testNFA = generate_NFA(testRE , stateCounter);
       print_NFA_info(testNFA);
       //cout<<"state_counter = "<<stateCounter<<endl;
    }

void Test::test_print_nextStates(NFA_State* state){
  cout<<"  print out all the next states of state:"<<state->number<<endl;
          auto myMap = state->Next_State;
           for (const auto& pair : myMap) {
            if(pair.first == EPSILON)
           std::cout << "      input "<<"EPSILION" << " --> " << pair.second << std::endl;
            else if(pair.first == '\t')
            std::cout << "      input "<<"\\t"<< " --> " << pair.second << std::endl;
            else if(pair.first == '\n')
            std::cout << "      input "<<"\\n"<< " --> " << pair.second << std::endl;
            else if(pair.first == '\r')
            std::cout << "      input "<<"\\r"<< " --> " << pair.second << std::endl;
            else
            std::cout << "      input "<<pair.first<< " --> " << pair.second << std::endl;
    }
    cout<<"\n";
}

void Test::Test_merge_NFAs(){
  cout<<"testing merge NFAs:\n";
  method_mergeNFA merge_function;
  // configure input:
  int stringNum=5;
  string REs[]={
    "a*b.c|",
    "fhd.|*",
    "kij||m.",
    "o*p|*",
    "ha*p*||c*.*",
  };
  int stateCounter=1;
  for(size_t i(0);i<stringNum;i++){
    cout<<">loop "<<i<<" generate: "<<REs[i]<<endl;
    Pattern* pattern = new Pattern;
    pattern->RE_content = REs[i];
    NFA* nfa;
    nfa = generate_NFA(pattern, stateCounter);
    stateCounter = nfa->End_State_tmp+1;
    merge_function.add_NFA_to_merge(nfa);
  }
  NFA* merged_NFA = merge_function.merge_NFAs();
  print_NFA_info(merged_NFA);
}

void Test::Test_print_DFA(DFA* _DFA){
  cout<<"# start to print DFA info:";
  cout<<"- start state: "<<_DFA->start_stateNumber<<endl;
  cout<<"- accepting states: \n";
  for(auto& pair:_DFA->Accepting_States){
    cout<<"  - state: ["<<pair.first;
    cout<<"] the action is num."<<pair.second.priority;
    cout<<endl;
  }
  cout<<"- all states: {\n";
  for(auto& pair:_DFA->states_Map){
    cout<<"\n ## state: "<<pair.first<<endl;
    cout<<" -subset: { ";
    for(auto& v:_DFA->states_Map[pair.first].subset_of_NFA){
        cout<<v<<" ";
    }
    cout<<"}\n";
    for(auto& pair1:_DFA->states_Map[pair.first].next_State){
      cout<<"input: "<<pair1.first<<" ---> state("<<pair1.second<<")\n";
    }
  }
  cout<<"}\nFinish printing!\n";
}

void Test::Test_print_DFA(DFA* _DFA, string filePath){
  unordered_set<int >acts;
  int rec=0;
  ofstream outputfile(filePath);
  
    outputfile<<"# start to print DFA info:";
  outputfile<<"- start state: "<<_DFA->start_stateNumber<<endl;
  outputfile<<"- accepting states: \n";
  for(auto& pair:_DFA->Accepting_States){
    outputfile<<"  - state: ["<<pair.first;
    outputfile<<"] the action is num."<<pair.second.priority;
    acts.insert(pair.second.priority);
    outputfile<<endl;
  }
  outputfile<<"- all states: {\n";

   for(int i(0);i<_DFA->Dsize;i++){
    outputfile<<"\n ## state: "<<i<<endl;
    for(int j(0);j<128;j++){
      if(_DFA->Dtran[i][j]==-1) continue;
      int dst = _DFA->Dtran[i][j];
          char s= char(j);
          string str;
    if(s=='\n') str="\\n";
    else if(s=='\r') str="\\r";
    else if(s=='\t') str="\\t";
    else if(s==' ') str="ws";
    else str.push_back(s);
      outputfile<<"input: "<<"ASCLL("<<j<<") ---> state("<<dst<<")\n";
    }
   }

  // for(auto& pair:_DFA->states_Map){
  //   outputfile<<"\n ## state: "<<pair.first<<endl;
  //   rec++;
  //   outputfile<<" -subset: { ";
  //   for(auto& v:_DFA->states_Map[pair.first].subset_of_NFA){
  //       outputfile<<v<<" ";
  //   }
  //   outputfile<<"}\n";
  //   for(auto& pair1:_DFA->states_Map[pair.first].next_State){
  //         char s= pair1.first;
  //         string str;
  //   if(s=='\n') str="\\n";
  //   else if(s=='\r') str="\\r";
  //   else if(s=='\t') str="\\t";
  //   else if(s==' ') str="ws";
  //   else str.push_back(s);
  //     outputfile<<"input: "<<str<<" ---> state("<<pair1.second<<")\n";
  //   }
  // }
  int size=acts.size();
  outputfile<<"}\nThere are "<<_DFA->Dsize<<" states in total\n";
  outputfile<<"}\nThere are "<<_DFA->Accepting_States.size()<<" accepting states in total\n";
  outputfile<<"There are "<<acts.size()<<" actions\n";
  for(int i(0);i<PatternNumber;i++){
       if(acts.count(i)==0)
       cout<<"act "<<i<<" is lost!!!\n";
  }
  outputfile<<"Finish printing!\n";

/*
   outputfile<<"----> Print DFA:\nStart state number: "
   <<_DFA->start_stateNumber<<"\nAccepting State Number: \n";
   for(auto& pair: _DFA->Accepting_States){
    outputfile<<"{ "<<pair.first<<" } "<<endl;
   }
   outputfile<<"all the states: {\n";
   for(auto& pair:_DFA->states_Map){
    outputfile<<"#state: "<<pair.first<<endl;
    outputfile<<"subset:{";
    for(auto& val:pair.second.subset_of_NFA){
      outputfile<<val<<" ";
    }outputfile<<"}\n";
    for(auto& _pair:pair.second.next_State){
       string str;
    char s= _pair.first;
    if(s=='\n') str="\\n";
    else if(s=='\r') str="\\r";
    else if(s=='\t') str="\\t";
    else if(s==' ') str="ws";
    else str.push_back(s);
    outputfile<<"input: "<<str<<" --------> "<<_pair.second<<endl;
     // cout<<"---input: "<<_pair.first<<"---->"<<" "<<_pair.second<<endl;
    }
   }
   outputfile<<"} \n-------print finish\n";
   */
   outputfile.close();
}
/*
testing string:
string test_string="ab|*aa.bb.|.ab|*."
*/
// void Test::Test_generate_DFA(){
//     generate_DFA m_DFA;
//     NFA* merged_NFA;
//     cout<<"testing generate DFA:\n -->firstly merge NFAs:\n";
//   method_mergeNFA merge_function;
//   // configure input:
//   string REs[]={
//     //"ab$*a*b*|$ba$*$"
//     //"ab|*aa$bb$|$ab|*$"
//     //"01$10$|*01$10$|$"
//     //"ab|*a$b$"
//     "a?", "b+", "c*"
//   };
//   int stringNum=3;
//   int stateCounter=1;
//   for(size_t i(0);i<stringNum;i++){
//     cout<<">loop "<<i<<" generate: "<<REs[i]<<endl;
//     Pattern* pattern = new Pattern;
//     pattern->RE_content = REs[i];
//     NFA* nfa;
//     nfa = generate_NFA(pattern, stateCounter);
//     stateCounter = nfa->End_State_tmp+1;
//     print_NFA_info(nfa);
//     merge_function.add_NFA_to_merge(nfa);
  
//   }
//    merged_NFA = merge_function.merge_NFAs();
  

//   //merged_NFA = merge_function.merge_NFAs();
//   print_NFA_info(merged_NFA);
//   m_DFA.generate_DFA_from_NFA(merged_NFA);

//   Test_print_DFA(m_DFA.get_DFA(),DFA_FILE);
//   optimize_DFA optDFA(m_DFA.get_DFA());
//    DFA* opt_DFA = optDFA.get_DFA();
//   Test_print_DFA(opt_DFA, OPT_DFA_FILE);
// }

void Test::Test_print_NFA(NFA* _NFA, string filePath){
  unordered_set<int >acts;
  int rec=0;
  ofstream outputfile(filePath);
  
    outputfile<<"# start to print NFA info:";
  outputfile<<"- start state: "<<_NFA->Start_State<<endl;
  outputfile<<"- accepting states: \n";
  for(auto& pair:_NFA->Final_States){
    outputfile<<"  - state: ["<<pair.first;
    outputfile<<"] the action is num."<<pair.second.priority;
    acts.insert(pair.second.priority);
    outputfile<<endl;
  }
  outputfile<<"- all states: {\n";
  for(auto& pair:_NFA->States_Map){
    outputfile<<"\n ## state: "<<pair.first<<endl;rec++;
    for(auto& pair1:_NFA->States_Map[pair.first].Next_State){
          char s= pair1.first;
          string str;
    if(s=='\n') str="\\n";
    else if(s=='\r') str="\\r";
    else if(s=='\t') str="\\t";
    else if(s==' ') str="ws";
    else if(s==EPSILON) str="EPSILON";
    else str.push_back(s);
      outputfile<<"input: "<<str<<" ---> state("<<pair1.second<<")\n";
      
    }
  }
  int size=acts.size();
  outputfile<<"}\nThere are "<<rec<<" states in total\n";
  outputfile<<"There are "<<size<<" actions\n";
  for(int i(0);i<PatternNumber;i++){
       if(acts.count(i)==0)
       cout<<"act "<<i<<" is lost!!!\n";
  }
  outputfile<<"Finish printing!\n";
  outputfile.close();
}

void Test::Test_print_DFA_states(DFA_state* _state){
  cout<<"\n--start to print DFA_state: "<<_state->state_number<<endl;
  // cout<<"include set: { ";
  // for(auto& ele:_state->subset_of_NFA){
  //   cout<<"("<<ele<<") ";
  // }
  // cout<<"}\n";
  cout<<"Next state: \n";
  for(auto& pair:_state->next_State){
    string str;
    char s= pair.first;
    if(s=='\n') str="\\n";
    else if(s=='\r') str="\\r";
    else if(s=='\t') str="\\t";
    else str.push_back(s);
    cout<<"input: "<<str<<"-------->>"<<pair.second<<endl;
  }
  cout<<"\n -------end print DFA_state\n";
}

void Test::Test_generateDFA_from_RE_post(string* REs, int num){
    generate_DFA m_DFA;
    NFA* merged_NFA;
    cout<<"testing generate DFA:\n -->firstly merge NFAs:\n";
  method_mergeNFA merge_function;
  // configure input:
  int stringNum=num;
  int stateCounter=1;
  for(size_t i(0);i<stringNum;i++){
    cout<<">loop "<<i<<" generate: "<<REs[i]<<endl;
    Pattern* pattern = new Pattern;
    pattern->RE_content = REs[i];
    pattern->priority = i;
    NFA* nfa;
    nfa = generate_NFA(pattern, stateCounter);
    stateCounter = nfa->End_State_tmp+1;
    //print_NFA_info(nfa);
    merge_function.add_NFA_to_merge(nfa);
  
  }
   merged_NFA = merge_function.merge_NFAs();
  

  //merged_NFA = merge_function.merge_NFAs();
  print_NFA_info(merged_NFA);
  m_DFA.generate_DFA_from_NFA(merged_NFA);
  Test_print_DFA(m_DFA.get_DFA());
  Test_print_DFA(m_DFA.get_DFA(),DFA_FILE);
  DFA* optDFA = m_DFA.minimize_DFA();
  Test_print_DFA(optDFA);
  Test_print_DFA(optDFA, OPT_DFA_FILE);
}

   
// void Test::Test_print_subset(subset* _subset){
//   cout<<"Print the subset: {";
//   for(auto& ele:*_subset){
//     cout<<"("<<ele<<") ";
//   }
//   cout<<"}Finish\n";
// }

//   int main(){
//   Test _test;
// // //    _test.Test_generate_NFA_from_RE_post("\"a\"");}
// // //    _test.Test_generate_NFA_from_RE_post("a+");
// // //    _test.Test_generate_NFA_from_RE_post("ma+?|");
// // //   // _test.Test_generate_NFA_from_RE_post("ab|*");
// // //   // _test.Test_generate_NFA_from_RE_post("ab|*a.");
// // //  //_test.Test_generate_NFA_from_RE_post("a*");
// // //   //_test.Test_generate_NFA_from_RE_post("a*b*.");
// // //  //_test.Test_generate_NFA_from_RE_post("a*b.");
// // //   //_test.Test_generate_NFA_from_RE_post("a*b.c|");
// // //   //_test.Test_merge_NFAs();
//     _test.Test_generate_DFA();
//   }