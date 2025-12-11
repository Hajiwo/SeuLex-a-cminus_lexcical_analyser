#include"Read_Lex.h"
#include"Method_RE.h"
#include"Method_Automata.h"
#include"Test.h"
#include"Global_Val.h"
#include<unordered_set>
using namespace std;

void print_string(string str){
   for(int i(0);i<str.size();i++){
    if(str[i]=='\t') cout<<"\\t";
    else if(str[i]=='\n') cout<<"\\n";
    else if(str[i]=='\r') cout<<"\\r";
    else if(str[i]==' ') cout<<"ws";
    else cout<<str[i];
   }
}

void read_lex::Test_print_info(){
    cout<<"\n>>>Testing print info:\n#part 1(section 1):\n";
    for(auto& pair:RE_MAP){
        cout<<"("<<pair.first<<","<<pair.first.size()<<")  ("<<pair.second<<","<<pair.second.size()<<")"<<endl;
    }
    cout<<"#part 2(section 2):\n";
    for(int i(0);i<Patterns.size();i++){
        cout<<"Pattern "<<Patterns[i].priority<<" Name:(";
        print_string(Patterns[i].RE_content);
        cout<<",size="<<Patterns[i].RE_content.size()<<")\n ACTION: "<<Patterns[i].Action<<"\n\n";
    }
    cout<<"print finish!\n";
}

void read_lex::read_FILE_LEX(string filePath){
    cout<<">>>>Start to read cLex.l file\n";
    // Open the input file
    std::ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        std::cout << "Failed to open the input file." << std::endl;
        exit(0);
    }

    // Open the output file
    ofstream outputfile[2];
    outputfile[0].open(FILENAME_SEC1);
    if (!outputfile[0].is_open()) {
        std::cout << "Failed to create the output file." << std::endl;
        exit(0);
    }
    outputfile[1].open(FILENAME_SEC3);
    if (!outputfile[1].is_open()) {
        std::cout << "Failed to create the output file." << std::endl;
        exit(0);
    }

    string line;
    Pattern* pat;
    int counter = 0;
    int sec = 0;
    while(getline(inputFile,line)){
        
        //cout<<line<<"     sec="<<sec<<endl;
        //section1:
        if(sec == 0){
            int pos = line.find("%{");
            if(pos!=string::npos) {
                sec++;
                
            }
        }else
        if(sec == 1){
            int pos = line.find("%}");
            if(pos!=string::npos) {
                sec++;
                
            }else 
            outputfile[0]<<line<<'\n';
        }
        else if(sec == 2){
            int pos = line.find("%%");
            if(pos!=string::npos) {
                sec++;
               
            }else{
            if(line[0]!=' '&&line[0]!='\t'&&line[0]!='\n'&&line[0]!='\0'){
                string Name;
                string Patt;
                bool tag = 0;
                 for(int i(0);i<line.size();i++){
                    char s = line[i];
                     if(!tag){
                        if(s!=' '&&s!='\n'&&s!='\t'&&s!='\0'){
                            Name.push_back(s);
                            
                        }else{
                            tag = 1;
                        }
                     }else{
                            int j = line[line.size()-1];
                            while(line[i]==' '||line[i]=='\t'||line[i]=='\0'||line[i]=='\n'){
                                i++;
                            }
                // int asciiValue = static_cast<int>(line[i]);
                // cout<<"\nASCLL is "<<asciiValue<<line[i]<<endl;;                            
                            while(line[j]==' '||line[j]=='\t'||line[j]=='\n'||line[j]=='\0'){
                                j--;
                            }
                            Patt = line.substr(i, j-i+1);
                            break;
                     }
                 }
                 RE_MAP.insert(make_pair(Name,Patt));
            }
            }
            }
            else if(sec==3){
            int pos = line.find("%%");
            if(pos!=string::npos) {
                sec++;
               
            }else{
                // int asciiValue = static_cast<int>(line[0]);
                // cout<<"ascll is "<<asciiValue<<line[0];
             if(line[0]!=' '&&line[0]!='\t'&&line[0]!='\n'&&line[0]!='\0'){
                string RE;
                string Act;
                int j=line.size()-1;
                //cout<<"enter: "<<line<<endl;
                while(line[j]!='}'){
                    j--;
                    if(j<0){
                        cout<<"error\n";
                        exit(1);
                    }
                }
                int i = j-1;
                int count = 0; 
                while(1){
                    if(count==0&&line[i]=='{') break;
                    else if (line[i]=='}') count++;
                    else if(line[i]=='{') count--;
                    i--;
                }
                Act = line.substr(i,j-i+1);
                i--;
                while(line[i]==' '||line[i]=='\t'||line[i]=='\0'||line[i]=='\n'){
                    i--;
                    if(i<0){
                        cout<<"error\n";
                        exit(1);
                    }
                }
                RE = line.substr(0,i+1);
                pat= new Pattern;
                pat->RE_content = RE;
                pat->Action = Act;
                pat->priority=counter++;
                Patterns.push_back(*pat);
                //cout<<RE<<" "<<Act<<endl;
            }
            }
            }
            else if(sec==4){
            outputfile[1]<<line<<'\n';                
            }
        }
   

    // Close the input and output files
    inputFile.close();
    outputfile[0].close();
    outputfile[1].close();
    PatternNumber = Patterns.size();
    cout<<"Finish reading cLex.l file\n";
}

void read_lex::standardize_REs(){
           std::string printableChars;
    for (int c = 33; c <= 126; c++) {
        if (c != 32 && c != 10) {  // Exclude space (32) and newline (10)
            string str ;
            str.push_back('"');
            str.push_back(c);
            str.push_back('"');
            str.push_back('|');
            printableChars += str;
        }
    }
    printableChars.pop_back();
    string anyChar = printableChars;
    bool tag = 0;
    method_RE method;
    for(int i(0);i<Patterns.size();i++){
        //cout<<"pattern: "<<i<<endl;
        //after reading cLex, \t is regraded as \ and t, so we have to modify this(also other with \)
        //for . meaning any character except \n
        while(1){
            int m = Patterns[i].RE_content.find('\\');
            if(m==string::npos) break;
            if(Patterns[i].RE_content[m+1]!='"'){
                if(Patterns[i].RE_content[m+1]=='t')
                  Patterns[i].RE_content.replace(m,2,"\t");
                  if(Patterns[i].RE_content[m+1]=='r')
                  Patterns[i].RE_content.replace(m,2,"\r");
                  if(Patterns[i].RE_content[m+1]=='n')
                  Patterns[i].RE_content.replace(m,2,"\n");
            }

        }
        //
        for(auto& pair:RE_MAP){
            while(1){
             string s;
             s.push_back('{');
             s+=pair.first;
             s.push_back('}');
             int k = Patterns[i].RE_content.find(s);
             if(k==string::npos) break;
              Patterns[i].RE_content.erase(k, s.size());
              Patterns[i].RE_content.insert(k, pair.second);
            }
        }
        int m = Patterns[i].RE_content.find('[');
        int l = Patterns[i].RE_content.find(']');
        if(m!=string::npos && l!=string::npos) tag=1;
        if(tag){
        method.standardize_RE(&Patterns[i]);
        tag=0;
        }
        method.standardize_quoteSymbols(&Patterns[i]);
        if(Patterns[i].RE_content==".") {Patterns[i].RE_content=anyChar;cout<<"tag\n";}
        method.genearte_postfix_RE(&Patterns[i]);
    }

    cout<<"----RE standardization finish!\n";
    
}

 void read_lex::generate_DFA_from_LEX(){
    Test test;
    generate_DFA m_DFA;  //used to generate DFA
    NFA* merged_NFA;     //merged NFA
    cout<<">Start to generate DFA:\n -->firstly generate and merge NFAs:\n";
    method_mergeNFA merge_function;
    int stringNum = Patterns.size();
    int stateCounter = 1;
  // configure input:
  for(size_t i(0);i<stringNum;i++){
    cout<<"\n>generate NFA number "<<i<<endl;
    NFA* nfa;
    nfa = generate_NFA(&Patterns[i], stateCounter);
    //test.print_NFA_info(nfa);
    stateCounter = nfa->End_State_tmp+1;
    merge_function.add_NFA_to_merge(nfa);

  }
   merged_NFA = merge_function.merge_NFAs();

  //merged_NFA = merge_function.merge_NFAs();
  //test.print_NFA_info(merged_NFA);
  test.Test_print_NFA(merged_NFA, NFA_FILE);
  m_DFA.generate_DFA_from_NFA(merged_NFA);
  test.Test_print_DFA(m_DFA.get_DFA(),DFA_FILE);
    //myDFA = m_DFA.get_DFA();
    DFA* opt_DFA = m_DFA.minimize_DFA();
   test.Test_print_DFA(opt_DFA, OPT_DFA_FILE);
   myDFA = opt_DFA;
  cout<<"\n\n -- cLex.l file analysis finish!\n";

  

 }

void read_lex::generate_yyLEX(string filePath){
     ifstream inputfile(FILENAME_SEC1);
     ofstream outputfile(filePath);
     //add DFA info:
     int size=myDFA->Dsize;

     outputfile<<"#include<string>\n"<<
     "#include<iostream>\n"<<
     "#include<vector>\n"<<
     "#include<fstream>\n"<<
     "#include<unordered_set>\n"<<
     "using namespace std;\n"<<
     "string yytext;\n"<<
     "char p=0;\n"<<
     "int lastState;\n"<<
     "int presentState;\n"<<
     "int Dtran["<<size<<"][128]={";
     for(int i(0);i<size;i++)
     for(int j(0);j<128;j++){
        outputfile<<myDFA->Dtran[i][j];
        if(!(i==size-1&&j==127)) outputfile<<" ,";
     }
     outputfile<<"};\nint v[]={";
     bool start=true;
     for(auto&pair: myDFA->Accepting_States) 
   {
       if(start) {
        outputfile<<pair.first;
        start=false;
       }else 
       outputfile<<", "<<pair.first;
   }
     outputfile<<"};\n";
     outputfile<<"\nunordered_set<int>action(v, v+"<<myDFA->Accepting_States.size()<<");\n";
     string read;

     while (std::getline(inputfile, read)) {
    outputfile << read << std::endl;
     }
     inputfile.close();

     inputfile.open(FUCTION_FILE);
     while (std::getline(inputfile, read)) {
    outputfile << read << std::endl;
     }
     inputfile.close();

     //add actions:
     string line = "switch(act){\n";
     unordered_set<int>counter;
     outputfile<<line;
     for(auto&pair: myDFA->Accepting_States){
        
         outputfile<<"  case "<<pair.first<<":\n"<<pair.second.Action<<endl;
         counter.insert(pair.second.priority);
        
     }
     outputfile<<"}\nreturn 0;}\n";
     //outputfile<<"//"<<"there are "<<counter.size()<<" cases in total\n";
    
    
     inputfile.open(FILENAME_SEC3);
    while (std::getline(inputfile, read)) {
    outputfile << read << std::endl;
     }
     inputfile.close();
     outputfile.close();

}