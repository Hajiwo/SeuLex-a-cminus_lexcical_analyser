#ifndef READ_LEX_H
#define READ_LEX_H
#include"Global_Val.h"
#include"Method_RE.h"
#include<fstream>
using namespace std;
class read_lex{
private:
map<string, string> RE_MAP;
vector< Pattern>Patterns;
DFA* myDFA;

public:
void read_FILE_LEX(string );
void standardize_REs();
void Test_print_info();
void generate_DFA_from_LEX();
void generate_yyLEX(string );
};

#endif