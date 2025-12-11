#include"Global_Val.h"
#include"Method_RE.h"
#include "Read_Lex.h"
#include<stack>
using namespace std;

/*
we suppose the operators are:
one oprend:  *, +, ? we can actually view them as the same
two oprends:  concatenation(emitted), | 
brackets: ( and )
*/
void method_RE::genearte_postfix_RE(Pattern* _pattern){
   //RE_post->Actions = _pattern->Actions;
   string RE;
   stack<char >rec;
   string str = _pattern->RE_content;
   rec.push('#');
   for(int i(0);i<str.length();i++){
    char s = str[i];

    if(s=='('){
       if(i>0){
                bool add = false;
            if(str[i-1]=='*'||str[i-1]=='+'||str[i-1]=='?') add=1;
            else if (str[i-1]=='"') add=1;
            else if (str[i-1]==')') add=1;
            else if (str[i-1]!='|'&&str[i-1]!='(') add=1;
            if(add){
                while(rec.top()=='*'||rec.top()=='?'||rec.top()=='+'||rec.top()=='$'){
                char t= rec.top();
                RE.push_back(t);
                if(rec.empty()) {cout<<"empty error\n"; exit(0);}
                rec.pop();
            }
              rec.push('$');
            }
        }
         rec.push(s);
    }
    else if(s==')'){
            while(rec.top()!='('){
            char t = rec.top();
            if(rec.empty()) {cout<<"empty error\n"; exit(0);}
            rec.pop();
            RE.push_back(t);
           }
           if(rec.empty()) {cout<<"empty error\n"; exit(0);}
           rec.pop();
    }

    else if(s=='*'||s=='+'||s=='?')
        {
           while(rec.top()=='*'||rec.top()=='?'||rec.top()=='+'){
            if(rec.empty()) {cout<<"empty error\n"; exit(0);}
             rec.pop();
             RE.push_back(s);
           }
           rec.push(s);
        }

       else  if(s=='|')
        {
            while(rec.top()=='*'||rec.top()=='|'||rec.top()=='?'||rec.top()=='+'||rec.top()=='$'){
                char t= rec.top();
                RE.push_back(t);
                if(rec.empty()) {cout<<"empty error\n"; exit(0);}
                rec.pop();
            }
            rec.push(s);
           }
        // else if(if_operator(str[i+1])){
        //    //if the next char is an operator, not add concatenation 
        //    RE.push_back(s);
   //         }
        else if(s=='"') 
        {
            if(i>0){
                bool add = false;
            if(str[i-1]=='*'||str[i-1]=='+'||str[i-1]=='?') add=1;
            else if (str[i-1]=='"') add=1;
            else if (str[i-1]==')') add=1;
            else if (str[i-1]!='|'&&str[i-1]!='(') add=1;
            if(add){
                while(rec.top()=='*'||rec.top()=='?'||rec.top()=='+'||rec.top()=='$'){
                char t= rec.top();
                RE.push_back(t);
                if(rec.empty()) {cout<<"empty error\n"; exit(0);}
                rec.pop();
            }
              rec.push('$');
            }
        }
            RE.push_back(str[i++]);
            RE.push_back(str[i++]);
            RE.push_back(str[i]);
        
        }else{
            if(i>0){
                bool add = false;
            if(str[i-1]=='*'||str[i-1]=='+'||str[i-1]=='?') add=1;
            else if (str[i-1]=='"') add=1;
            else if (str[i-1]==')') add=1;
             else if (str[i-1]!='|'&&str[i-1]!='(') add=1;
            if(add){
                while(rec.top()=='*'||rec.top()=='?'||rec.top()=='+'||rec.top()=='$'){
                char t= rec.top();
                RE.push_back(t);
                if(rec.empty()) {cout<<"empty error\n"; exit(0);}
                rec.pop();
            }
              rec.push('$');
            }
        }
            RE.push_back(s);
        }
//        cout<<"\nRE="<<RE<<endl<<"stack:";
//           stack<char>rec_2;
//    while(!rec.empty()){
//        rec_2.push(rec.top());
//        rec.pop();
//    }
//    while(!rec_2.empty()){
//        rec.push(rec_2.top());
//        rec_2.pop();
//        cout<<rec.top()<<" ";
//    }
//    cout<<endl;
   }
   while(!rec.empty()){
    char s = rec.top();
    if(rec.empty()) {cout<<"empty error\n"; exit(0);}
    rec.pop();
    if(s!='#') RE.push_back(s);
   }
   _pattern->RE_content = RE;
}


/* non-standard RE has extensive operators, case:
1. [A-Za-z]
2. [0-9]
3. [A-Za-z0-9]
*/
void method_RE::standardize_RE(Pattern* _pat){
    string str = _pat->RE_content;
    //cout<<str<<endl;
    bool finish = false;

    //erase []
    bool tag = 1;
    while(tag){
        int i = str.find('[');
        if(i==string::npos) break;
        while(i>0&&str[i+1]=='"'&&str[i-1]=='"'){
            i = str.find('[',i+2);
            if(i==string::npos) break;
        }
        int j = str.find(']', i+1);
        if(j==string::npos){cout<<"error in void method_RE::standardize_RE(Pattern* _pat)--error RE"; exit(0);}
                        string strand = str.substr(i+1,j-i-1);
                        //cout<<"strand="<<strand<<endl;
                        string pattern = "A-Z";
                        string replacement = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
                        size_t startPos = strand.find(pattern);
                        if(startPos!=string::npos){
                        strand.erase(startPos, pattern.length());
                        strand.insert(startPos, replacement);
                        }
                                pattern = "a-z";
                                replacement = "abcdefghijklmnopqrstuvwxyz";
                                startPos = strand.find(pattern);
                       if(startPos!=string::npos){
                        strand.erase(startPos, pattern.length());
                        strand.insert(startPos, replacement);
                        }
                                pattern = "0-9";
                                replacement = "0123456789";
                                startPos = strand.find(pattern);
                       if(startPos!=string::npos){
                        strand.erase(startPos, pattern.length());
                        strand.insert(startPos, replacement);
                        }
            
                    string ts;
                            ts.push_back('(');
                            for(int i(0);i<strand.size();i++){
                                if(strand[i]=='"'){
                                     ts.push_back(strand[i++]);
                                     ts.push_back(strand[i++]);
                                }
                                   ts.push_back(strand[i]);
                                   if(i < strand.size()-1){
                                    ts.push_back('|');
                                   }
                                
                            }
                            ts.push_back(')');
                            str.erase(i, j-i+1);
                            str.insert(i, ts);
    }


_pat->RE_content = str;
return;

}

void method_RE::standardize_quoteSymbols(Pattern* pat){
        //for "aaa" we have to make it: "a""a""a"
        string str = pat->RE_content;
    int k(0),l;
    while(1){
        if(k==str.size()) break;
        if(str[k]!='"') {
            k++;
            continue;
        }else{
            l = str.find('"',k+1);
            if(l-k==2) {
                k = l+1;
                continue;
            }else{
                string sub = str.substr(k,l-k+1);
                string sub1; 
                for(int m(1);m<sub.size()-1;m++){                    
                        sub1.push_back('"');
                        sub1.push_back(sub[m]);
                        sub1.push_back('"');                   
                }
                str.replace(k,l-k+1,sub1);
                k=0;
                continue;               
            }
        }
    }
     pat->RE_content = str;
}


// int main(){
//     read_lex Lex_reader;
//     // Open the input file
//     string filePath= CLEX_L;
//     Lex_reader.read_FILE_LEX(filePath);
//     Lex_reader.Test_print_info();

//        method_RE mRE;
//        string s = "((s|k\"m\")ab)c\".\"";
//        Pattern* pat = new Pattern;
//        pat->RE_content = s;
//       cout<<"input: "<<s<<endl;
//       //cout<<"Test: "<<pat->RE_content<<endl;
//       //pat->RE_content = "";
//       mRE.genearte_postfix_RE(pat);
//       cout<<"Test: "<<pat->RE_content<<endl;
//       Lex_reader.standardize_REs();
//       Lex_reader.Test_print_info();
// }
