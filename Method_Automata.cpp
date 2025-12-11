#include"Method_Automata.h"
#include"NFA.h"
#include"DFA.h"
#include"Test.h"
#include"Global_Val.h"
#include <stack>
#include <vector>
#include <iostream>
#include <algorithm>
Test method_test;

using namespace std;


void Test_print_subset(subset* _subset){
  cout<<"subset: {";
  for(auto& ele:*_subset){
    cout<<ele<<" ";
  }
  cout<<"} \n";
}

/*
start_State means the number of start state; (use & because we need to modify the state_Counter)
type is actually the operator or letter to decide how to construct the NFA
*/
NFA* generate_NFA_from_Letter(int& stateCounter, char letter){
  //cout<<"\nEnter generate_NFA_from_Letter: "<<letter<<endl;
  NFA* NFA_tmp = new NFA ;
  NFA_State* Start_state_tmp = new NFA_State ;
  NFA_State* End_state_tmp = new NFA_State ;
  //initilaize states:
  //numbers:
  int s1=Start_state_tmp->number = stateCounter++;
  int s2=End_state_tmp->number = stateCounter++;
  //edge:
  Start_state_tmp->Next_State.insert(make_pair(letter, s2));
  //initialize NFA:
  NFA_tmp->Start_State = s1;
  NFA_tmp->States_Map.insert(make_pair(s1, *Start_state_tmp));
  NFA_tmp->States_Map.insert(make_pair(s2, *End_state_tmp));
  NFA_tmp->End_State_tmp = End_state_tmp->number;
  //--the tmp NFA doesn't have an pattern in the final state map
  //cout<<"--end generate_NFA_from_Letter: "<<letter<<endl;
  return NFA_tmp;
}

/*
input: stateCounter, type, NFA1, NFA2(if needed)
output: NFA pointer
*/
NFA* generate_NFA_from_NFA(int& stateCounter, char type, NFA* NFA1, NFA* NFA2=NULL){
  //cout<<"\nenter combine_Two_NFA method\n";

  NFA* NFA_tmp = new NFA ;
  NFA_State* Start_state_tmp = new NFA_State ;
  NFA_State* End_state_tmp = new NFA_State ;

  if(type=='|'){
  //cout<<"type: | \n --start\n";
  //initilaize new states:
  //numbers:
  int s1=Start_state_tmp->number = stateCounter++;
  int s2=End_state_tmp->number = stateCounter++;
  NFA_State& End_state_NFA1 = NFA1->States_Map[NFA1->End_State_tmp];
  NFA_State& End_state_NFA2 = NFA2->States_Map[NFA2->End_State_tmp];
  //edge:
  int &startState_nfa1=NFA1->Start_State;
  int &startState_nfa2=NFA2->Start_State;

  Start_state_tmp->Next_State.insert(make_pair(EPSILON, startState_nfa1));
  Start_state_tmp->Next_State.insert(make_pair(EPSILON, startState_nfa2));
  End_state_NFA1.Next_State.insert(make_pair(EPSILON, s2));
  End_state_NFA2.Next_State.insert(make_pair(EPSILON, s2));
  

  //initialize NFA:
  //start:
  NFA_tmp->Start_State = s1;
  //end:
  NFA_tmp->End_State_tmp = s2;
  
  //add info from NFA1,2 to NFA_tmp(and the two new states):
  NFA_tmp->States_Map.insert(make_pair(s1, *Start_state_tmp));
  NFA_tmp->States_Map.insert(make_pair(s2, *End_state_tmp));
    //merge maps into the final map in NFA_tmp:
  NFA_tmp->States_Map.insert( NFA1->States_Map.begin(), NFA1->States_Map.end());
  NFA_tmp->States_Map.insert( NFA2->States_Map.begin(), NFA2->States_Map.end());
  //--the tmp NFA doesn't have an pattern in the final state map

  }else if(type == '$'){
   
  //note: NFA1 is ahead of NFA2
  //edge:
  int &startState = NFA1->Start_State;
  NFA_State* end_State = &NFA2->States_Map[NFA2->End_State_tmp];
  
  /*
  1. let the end_state of NFA1 be the start_state of NFA2:
     - copy nextState of start_state_NFA2 to that of end_state_NFA1
     - delete the start_state in the search_map of DFA2
  2. merge maps   
  */

 // 1.
  NFA_State& start_NFA2_State = NFA2->States_Map[NFA2->Start_State];
  unordered_multimap< char, int>& map2 = start_NFA2_State.Next_State;
  NFA_State& end_state_NFA1 = NFA1->States_Map[NFA1->End_State_tmp];
  end_state_NFA1.Next_State = map2;
  // NFA1->States_Map[NFA2->End_State_tmp].Next_State.insert(std::make_move_iterator(map2.begin()), std::make_move_iterator(map2.end()));

 //delete the start_state in the search_map of DFA2:
  NFA2->States_Map.erase(NFA2->Start_State);

  //initialize NFA:
  //start:
  NFA_tmp->Start_State = NFA1->Start_State;
  //end:
  NFA_tmp->End_State_tmp = NFA2->End_State_tmp;

    //merge maps into the final map in NFA_tmp:
  NFA_tmp->States_Map.insert( NFA1->States_Map.begin(), NFA1->States_Map.end());
  NFA_tmp->States_Map.insert( NFA2->States_Map.begin(), NFA2->States_Map.end());

  } else if(type == '*'){
    // '*' operator 
    /*
    1. a new start and a new end state
    2. start_state -epsilion-> start_state_of_NFA1
    3. end_state_of_NFA1- epsilon -> end_state 
    4. end_state_of_NFA1 -epsilon-> start_state_of_NFA1
    5. start_state -epsilon-> end_state
    6. insert the two new states into state_map of NFA_tmp
    7. set start state number to the new start_state
    8. set new end_state of NFA_tmp
    9. add states in NFA1_state_map to NFA_tmp_state_map
    */
    //1. 
    int &startNumber = Start_state_tmp->number = stateCounter++;
    int &endNumber = End_state_tmp->number = stateCounter++;
    //2.
    int &startNumber_NFA1 = NFA1->Start_State;
    int &endNumber_NFA1 = NFA1->End_State_tmp;
    NFA_State& startState_NFA1 = NFA1->States_Map[startNumber_NFA1];
    NFA_State& endState_NFA1 = NFA1->States_Map[NFA1->End_State_tmp];
    Start_state_tmp->Next_State.insert(make_pair(EPSILON, startState_NFA1.number));
    //3.
    endState_NFA1.Next_State.insert( make_pair( EPSILON, endNumber));
    //4.
    endState_NFA1.Next_State.insert( make_pair( EPSILON, startNumber_NFA1 ) );
    //5.
     Start_state_tmp->Next_State.insert(make_pair(EPSILON, endNumber));
    //6.
    NFA_tmp->States_Map.insert(make_pair( startNumber, *Start_state_tmp));
    NFA_tmp->States_Map.insert(make_pair( endNumber, *End_state_tmp));
    //7.
    NFA_tmp->Start_State = startNumber;
    //8.
    NFA_tmp->End_State_tmp = End_state_tmp->number;
    //9.
    NFA_tmp->States_Map.insert( NFA1->States_Map.begin(), NFA1->States_Map.end());


  }
  
  
  return NFA_tmp;
}

/*
just involve operators:
"|": or
"*"：closure
"$": concatenation
("()": parentheses can be emitted during the process of turning into postfix form )
Thus, the input RE_Post has a relative simple form like Pattern: ab.c|
Here Pattern stands for a RE and its action
*/
NFA* generate_NFA(Pattern* RE_Post, int startNumber){


  //cout<<"start to generate NFA \n";
  //
  NFA* NFA1 = NULL;
  NFA* NFA2 = NULL;
  stack< char>Stack_tmp;
  stack< NFA*>Stack_NFA;
  int state_Counter = startNumber;       
  string RE_string = RE_Post->RE_content;

   //special case: just one symbol!
  if(RE_Post->RE_content.size()==1||RE_Post->RE_content.size()==3&&RE_Post->RE_content[0]=='"'&&RE_Post->RE_content[2]=='"'){
    NFA* nfa;
    if(RE_Post->RE_content.size()==1) {
    nfa = generate_NFA_from_Letter(state_Counter ,RE_Post->RE_content[0]);
    nfa->Final_States.insert(make_pair(nfa->End_State_tmp, *RE_Post));
    }else{
    nfa = generate_NFA_from_Letter(state_Counter ,RE_Post->RE_content[1]);
    nfa->Final_States.insert(make_pair(nfa->End_State_tmp,*RE_Post));
    }

    return nfa;
  }

  /*
   There is a special case to notify:
    eg: a*b*c*d**
   we have to distinguish whether the oprend of the * is a letter or a NFA or not!
   thus we can place a special letter in stack to represent a NFA already generated
   we can use EPSILON as the symbol because no letter is EPSILON since white space has been cleared
  */
  //1.
  for(size_t i(0);i<RE_string.length();i++){
    //2.
    char &s = RE_string[i];

     if(s!='*'&&s!='|'&&s!='$'&&s!='?'&&s!='+'){
      if(s!='"')  //there may be case \".\" 
      Stack_tmp.push(s);
      else{  // regard "." as a symbol not an operator
        i++;
        Stack_tmp.push(RE_string[i]);
        i++;
      }
      continue;

     }
     //3.
     if(s == '*'){
       //.1
        if(Stack_tmp.empty()) {
          cout<<"stack empty error\n";
          exit(0); 
        }
       char &letter = Stack_tmp.top();
       Stack_tmp.pop();
       //.2 .3
       if( letter!=EPSILON ){
         NFA1 = generate_NFA_from_Letter( state_Counter, letter);
         NFA1 = generate_NFA_from_NFA( state_Counter, s, NFA1 );
       }else{
        if(Stack_NFA.empty()) {
          cout<<"stack_NFA empty error\n";
          exit(0); 
        }
         NFA1 = Stack_NFA.top();
         Stack_NFA.pop();
         NFA1 = generate_NFA_from_NFA(state_Counter, s, NFA1 );
       }
       //.4 
       Stack_tmp.push(EPSILON);
       //.5
       Stack_NFA.push(NFA1);
       
       continue;

     }
          if(s == '?'){
       //.1
        if(Stack_tmp.empty()) {
          cout<<"stack empty error\n";
          exit(0); 
        }
       char &letter = Stack_tmp.top();
       Stack_tmp.pop();
       //.2 .3
       if( letter!=EPSILON ){
         NFA1 = generate_NFA_from_Letter( state_Counter, letter);
         NFA* tmp = generate_NFA_from_Letter(state_Counter, EPSILON);
         NFA1 = generate_NFA_from_NFA(state_Counter, '|', NFA1 ,tmp);
         //NFA1 = generate_NFA_from_NFA( state_Counter, s, NFA1 );
       }else{
        if(Stack_NFA.empty()) {
          cout<<"stack_NFA empty error\n";
          exit(0); 
        }
         NFA1 = Stack_NFA.top();
         Stack_NFA.pop();
         NFA* tmp = generate_NFA_from_Letter(state_Counter, EPSILON);
         NFA1 = generate_NFA_from_NFA(state_Counter, '|', NFA1 ,tmp);
       }
       //.4 
       Stack_tmp.push(EPSILON);
       //.5
       Stack_NFA.push(NFA1);
       
       continue;

     }
          if(s == '+'){
       //.1
        if(Stack_tmp.empty()) {
          cout<<"stack empty error\n";
          exit(0); 
        }
       char &letter = Stack_tmp.top();
       Stack_tmp.pop();
       //.2 .3
       if( letter!=EPSILON ){
         NFA1 = generate_NFA_from_Letter( state_Counter, letter);
         NFA1 = generate_NFA_from_NFA( state_Counter, '*', NFA1 );
         NFA* tmp=generate_NFA_from_Letter( state_Counter, letter);
         NFA1 = generate_NFA_from_NFA( state_Counter, '$', NFA1,tmp );
       }else{
        if(Stack_NFA.empty()) {
          cout<<"stack_NFA empty error\n";
          exit(0); 
        }
         NFA1 = Stack_NFA.top();
         Stack_NFA.pop();
         NFA1 = generate_NFA_from_NFA(state_Counter, '*', NFA1 );
         NFA* tmp=generate_NFA_from_Letter( state_Counter, letter);
         NFA1 = generate_NFA_from_NFA( state_Counter, '$', NFA1,tmp );
       }
       //.4 
       Stack_tmp.push(EPSILON);
       //.5
       Stack_NFA.push(NFA1);
       
       continue;

     }
     //4. 
     if(s=='$'||s=='|'){
     //.1
       if(Stack_tmp.empty()) {
          cout<<"stack empty error\n";
          exit(0); 
        }
       char &letter2 = Stack_tmp.top();
       Stack_tmp.pop();

      if(Stack_tmp.empty()) {
          cout<<"stack empty error\n";
          exit(0); 
        }
       char &letter1 = Stack_tmp.top();
       Stack_tmp.pop();
      //.2
      if(letter2 == EPSILON){
        if(Stack_NFA.empty()) {
          cout<<"stack_NFA empty error\n";
          exit(0); 
        }
         NFA2 = Stack_NFA.top();
         Stack_NFA.pop();
      }else{
         NFA2 = generate_NFA_from_Letter( state_Counter, letter2);
      }

      if(letter1 == EPSILON){
        if(Stack_NFA.empty()) {
          cout<<"stack_NFA empty error\n";
          exit(0); 
        }
         NFA1 = Stack_NFA.top();
         Stack_NFA.pop();
      }else{
         NFA1 = generate_NFA_from_Letter( state_Counter, letter1);
      }

      //.3
      NFA1 = generate_NFA_from_NFA( state_Counter, s, NFA1, NFA2);
      //.4 
       Stack_tmp.push(EPSILON);
      //.5
       Stack_NFA.push(NFA1);

       continue;
     }
     
  }

 
  //5.
   if(Stack_tmp.empty()) {
          cout<<"stack empty: error\n";
          exit(0); 
        }
   Stack_tmp.pop();
      if(Stack_NFA.empty()) {
          cout<<"stack_NFA empty: error\n";
          exit(0); 
        }
   NFA1 = Stack_NFA.top();
   Stack_NFA.pop();
  //renew accepting state:
  NFA1->Final_States.insert( make_pair(NFA1->End_State_tmp, *RE_Post));
  if(!Stack_tmp.empty() ) {
          cout<<"final stack not empty: error\n";
          exit(0); 
        }
  if(!Stack_NFA.empty()) {
          cout<<"final stack_NFA not empty: error\n";
          exit(0); 
        }
  cout<<"NFA generated\n";
  //
  return NFA1;
}

method_mergeNFA::method_mergeNFA(){
  final_NFA = new NFA;
  NFA_State* start_state = new NFA_State;
  final_NFA->Start_State = START_STATE_OF_MERGED_NFA;
  final_NFA->End_State_tmp = MULTI_ACCEPTING_STATES;
  final_NFA->States_Map.insert( make_pair(START_STATE_OF_MERGED_NFA ,*start_state));
}

void method_mergeNFA::add_NFA_to_merge(NFA* p_NFA){
  final_NFA->States_Map.insert( p_NFA->States_Map.begin(),  p_NFA->States_Map.end());
  final_NFA->Final_States.insert( p_NFA->Final_States.begin(), p_NFA->Final_States.end());
  final_NFA->States_Map[0].Next_State.insert(make_pair( EPSILON, p_NFA->Start_State));
  cout<<"....> merged to final_NFA \n";
}

/*
let state 0 be the start state
*/
NFA* method_mergeNFA::merge_NFAs(){
  return final_NFA;
}

//DFA: start with state number 0
generate_DFA::generate_DFA(){
  stateCounter = START_STATE_OF_DFA;
  DFA_p = new DFA;
  //DFA_p->Dtran = DFA_p->Dtran;
  opt_DFA = new DFA;
  //opt_DFA_p->Dtran = optDFA_p->Dtran;
  for(int i(0);i<DSIZE;i++){
    for(int j(0);j<128;j++){
    DFA_p->Dtran[i][j]=-1;
    opt_DFA->Dtran[i][j]=-1;
    }
  }
}

subset* generate_DFA::E_closure(int stateNumber){
  // cout<<"the E_closure of "<<stateNumber<<" is ";
  //cout<<"test E_closure: state-"<<stateNumber<<endl;
  stack<int >unmarked;
  unmarked.push(stateNumber);
  subset* res = new subset;
  while(!unmarked.empty()){
    int s = unmarked.top();
    unmarked.pop();
    res->insert(s);
    for(auto& pair:input_NFA_p->States_Map[s].Next_State){
      if(pair.first==EPSILON) {
           if(res->find(pair.second)==res->end()){
            res->insert(pair.second);
            unmarked.push(pair.second);
           }
      }
    }
  }
  // cout<<"the E_closure of "<<stateNumber<<" is ";
   //Test_print_subset(res);
 return res;

}

subset* generate_DFA::E_closure(subset* input_set_p){
  //cout<<"\n\n------E_closure of set: ";
  //Test_print_subset(input_set_p);
  subset* _set = new subset;
     for(const auto& stateNumber: *input_set_p){
     // cout<<"E_closure: "<<stateNumber<<endl;
        subset* set_tmp = E_closure(stateNumber);
        _set->insert(set_tmp->begin(), set_tmp->end() );
     }
     //cout<<"res: "; Test_print_subset(_set);
     return _set ;
}

void generate_DFA::generate_State( subset* _set){
  int snum = stateCounter++;
  Dstates.push_back(_set);
  DFA_p->Dsize++;
  Pattern* pat = new Pattern;
  pat->priority=-1;
  for(auto& val: *_set){
    if(input_NFA_p->Final_States.count(val)>0){
      if(pat->priority==-1){
        pat->Action = input_NFA_p->Final_States[val].Action;
        pat->priority = input_NFA_p->Final_States[val].priority;
      }else{
        if(pat->priority>input_NFA_p->Final_States[val].priority){
          //cout<<"pri "<<input_NFA_p->Final_States[val].priority<<" <"<<pat->priority<<endl;
          pat->Action = input_NFA_p->Final_States[val].Action;
          pat->priority = input_NFA_p->Final_States[val].priority;
        }
      }
    }
  }
  if(pat->priority!=-1){
    DFA_p->Accepting_States.insert(make_pair(snum,*pat));
  }
}

/*
return -1 when not found
return the DFA_stateNumber when found;
*/
int generate_DFA::if_in_DFAstates(subset* _set){
     for(int i(0);i<Dstates.size();i++){
      if(*Dstates[i]==*_set) return i;
     }
     return NOT_FOUND_IN_DSTATES;
}

subset* generate_DFA::Move(char s, subset* T){
  subset* res = new subset;
  for(auto& state:*T){
      for(auto& pair:input_NFA_p->States_Map[state].Next_State){
            if(pair.first == s){
              res->insert(pair.second);
            }
      }
  }
  if(res->size()==0) return NULL;
  return res;
}

void generate_DFA::renew_Dtran(subset* T, char s, subset* U){
  int t=-1,u=-1;
  t = if_in_DFAstates(T);
  u = if_in_DFAstates(U);
  if(t==-1&&u==-1){
    cout<<"error from Dtran\n";
    exit(0);
  }
  int is = int(s);
  //cout<<"src:"<<t<<" --"<<is<<"-->"<<"dst="<<u<<endl;
  DFA_p->Dtran[t][is] = u;
}

void generate_DFA::generate_DFA_from_NFA(NFA* _NFA ){
    cout<<"\n >>>>>> start to generate DFA\n";
    input_NFA_p = _NFA;
    DFA_p->Dsize = 0;
    //initialize symbol table:
    for(auto& pair:input_NFA_p->States_Map){
      for(auto& pair1:input_NFA_p->States_Map[pair.first].Next_State){
        if(pair1.first!=EPSILON) symbol_Table.insert(pair1.first);
      }
    }
    stack<subset*>unmarked;
    subset* _set = E_closure(input_NFA_p->Start_State);
    generate_State(_set);
    //Test_print_subset(_set);
    DFA_p->start_stateNumber = START_STATE_OF_DFA;
    unmarked.push(_set);
    int sf=0;
    while(!unmarked.empty()){
      subset* T = unmarked.top();
      unmarked.pop();
     // cout<<"--->> unmarked set: ";
     // Test_print_subset(T);
      for(auto& s:symbol_Table){
       // cout<<"read symbol: "<<s<<endl;
        subset* U = Move(s,T);
        if(U!=NULL) {
          U = E_closure(U);
       // cout<<"get set: ";
       
        if(if_in_DFAstates(U)==NOT_FOUND_IN_DSTATES){
          //cout<<". " ;
            generate_State(U);
            unmarked.push(U);
        }

          renew_Dtran(T,s,U);
      
      }
      }
      //cout<<"finish a loop\n\n";
    }
    //clear_Automata(input_NFA_p);
    //verify:
    subset cset;
    for(auto& pair:Dstates){
      cset.insert(pair->begin(), pair->end());
    }
    if(cset.size()!=input_NFA_p->States_Map.size()){
      cout<<"error! DFA has missed some states of NFA\n";
      exit(0);
    }
    cout<<"->DFA from NFA generated!\n"; 
}


DFA* generate_DFA::get_DFA(){

  return DFA_p;
}

DFA* generate_DFA::minimize_DFA(){
  cout<<"->start to minimize DFA\n";
  initialize_Partion();
  cout<<">initialization done\n";
  bool Pass;
  do{
      Pass = false;
      for(auto& G:Partion){
         if(partion_Group(G)) {
          Pass = true;
          break;
         }
      }
  }while(Pass);
  cout<<">Partion finish!\n";
  merge_DFAstates();
  cout<<"--DFA minimized!\n";
  return opt_DFA;
}

void generate_DFA::merge_DFAstates(){
   for(auto& _group:Partion){
    if(_group->empty()) {
      cout<<"Error, empty group\n"; 
      exit(0);
    }
   }
    for(auto& _group:Partion){
         if( partion_Group(_group) ) {
          cout<<"Error, partion incomplete\n";
          exit(0);
         }
    }
     //erase dead states:
     Partion.erase(deadState);
     subset verifyset;
     for(auto& _group:Partion){
       verifyset.insert(_group->begin(), _group->end());
     }
     if(verifyset.size()!=DFA_p->Dsize){
      cout<<"Error! opt_DFA lacks states\n"; exit(0);
     }
     opt_DFA->Dsize = 0;
     unordered_map<subset*, int>set_To_num;
     unordered_map<int, subset*>num_To_set;
     //vector<subset*>optDstates;// rec and optDstates are used for convenience to search optDFA state by subset or number
     int i = 1;
     bool tag=false;
     for(auto& _group: Partion)
     {
       opt_DFA->Dsize++;
      if(!tag && find_group(START_STATE_OF_DFA)==_group){
        //Test_print_subset(_group);
          num_To_set.insert(make_pair(0,_group));
          set_To_num.insert(make_pair(_group,0));
          tag = true;
                for(auto& ele:*_group){
        if(DFA_p->Accepting_States.count(ele)>0){
             opt_DFA->Accepting_States.insert(make_pair(0, DFA_p->Accepting_States[ele]));
             break;
        }
       }
          continue;
      }else{
          num_To_set.insert(make_pair(i,_group));
          set_To_num.insert(make_pair(_group,i));
                for(auto& ele:*_group){
        if(DFA_p->Accepting_States.count(ele)>0){
             opt_DFA->Accepting_States.insert(make_pair(i, DFA_p->Accepting_States[ele]));
             break;
        }
       }
       i++;
      }

     }

     for(auto pair: set_To_num){
            //int opt_state = pair.second;
            if(pair.first->empty()) {
              cout<<"empty set error!\0";
              exit(0);
            }
            int reps = *(pair.first->begin());
            for(int i(0);i<128;i++){
             int dst = DFA_p->Dtran[reps][i];
             if(dst!=-1) {
             int j = set_To_num[find_group(dst)];
             opt_DFA->Dtran[pair.second][i] = j;
             }
            }  
     }
     cout<<"Transition info constructed!\n";
}

bool generate_DFA::partion_Group(subset* G){
  for(auto& s:symbol_Table){
     bool tag = false;
     subset* tmp = NULL;
     unordered_multimap<subset*, int>rec;  //map:rec is used to record the corresponding states that have the same next equivalent states set via symbol s
    for(auto& t:*G){
      subset* dst = find_group(Move(t,s));
      rec.insert(make_pair(dst, t));
      if(tmp==NULL) tmp = dst; //check if the group has to be partioned or not;
      else if(tmp!=dst) tag = true;
    }
   

    // if the group has to be partioned further:
    if(tag) {
      unordered_map<subset*, subset*>newSet;
      for(auto& pair:rec){
        if(newSet.count(pair.first)==0){
            subset* set1 = new subset;
            set1->insert(pair.second);
            newSet.insert(make_pair(pair.first, set1));
        }else{
          newSet[pair.first]->insert(pair.second);
        }
      }
      for(auto& pair:newSet){
        Partion.insert(pair.second);
      }
      Partion.erase(G);
      return true;
    }
  }
  return false;
}


void generate_DFA::initialize_Partion(){
    opt_DFA->start_stateNumber = 0;
    subset* S = NULL;
    map<int, subset*>rec; //final states with diffrent token pattern have different set
    for(auto& pair:DFA_p->Accepting_States){
      if(rec.count(pair.second.priority)>0) //we divide different final states by its priority
      {
            rec[pair.second.priority]->insert(pair.first);
      }else{
         subset* tmpSet = new subset;
         tmpSet->insert(pair.first);
         rec.insert(make_pair(pair.second.priority, tmpSet));
      }
    }
    for(int i(0);i<DFA_p->Dsize;i++){
          if(DFA_p->Accepting_States.count(i)==0){
            if(S==NULL) S= new subset;
               S->insert(i);
          }
    }
    deadState = new subset;  //technically necessary
    deadState->insert(DEAD_STATE);
    if(S!=NULL)
    Partion.insert(S);
    Partion.insert(deadState);
    for(auto& pair:rec)
    {      
        Partion.insert(pair.second);
    }
}

subset* generate_DFA::find_group(int stateNumber){
     subset* res = NULL;
     for(auto &val:Partion){
      if(val->count(stateNumber)>0) return val;
     }
     if(res==NULL){
      cout<<"Error from subset* generate_DFA::find_group(int stateNumber)\n";
      exit(0);
     }
     return res;
}

int generate_DFA::Move(int stateNum, char s){
      if(stateNum == DEAD_STATE) return DEAD_STATE;
      int is=int(s);
      int dst = DFA_p->Dtran[stateNum][is];
      return dst;
}

