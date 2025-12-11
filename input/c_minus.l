%{
#include<iomanip>
typedef enum
{
	INT = 1, 
	FLOAT,
	
	ID,
	STRUCT,
	IF,
	ELSE,
	RETURN,
	NUM,

	LPAREN,
	RPAREN,
	LCOMMENT,
	RCOMMENT,
	LBRACK,
	RBRACK,

	ASSIGN,
	SEMI,
	COMMA,
	
	MINUS,
	PLUS,
	TIMES,
	OVER,
	MOD,
	EQ,

	CHAR,
	BOOL,
	DOUBLE,
	VOID,

	_NULL,
	TRUE,
	FALSE,
	WHILE,

	PLUSASSIGN,
	MINUSASSIGN,
	TIMESASSIGN,
	OVERASSIGN,
	MODASSIGN,

	LOGICNOT,
	LOGICAND,
	LOGICOR,

	NEQ,
	LT,
	GT,
	LEQ,
	GEQ,

	NL,
	SPACE,
} TokenType;
int LineNum = 1;
int ERROR = -1;
ofstream outputFile("tokenList.l", std::ios::out);
%}

alpha			[A-Za-z]
alphanum		[A-Za-z0-9]
digits          [0-9]+

%%
[" "\t\r]			{ /*ignore white space*/ return SPACE; }
\n			        { LineNum++;return NL; }  

"int"			 { outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Fundamental type\n";return INT; }
"char"			{  outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Fundamental type\n";return CHAR; }
"bool"			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Fundamental type\n";return BOOL; }
"float"			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Fundamental type\n";return FLOAT;}
"double"		{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Fundamental type\n";return DOUBLE;}
"void"			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Fundamental type\n";return VOID;}


"NULL"			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Reserved Constant\n";return _NULL;}
"true"			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Reserved Constant\n";return TRUE;}
"false"			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Reserved Constant\n";return FALSE;} 
"while"			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Flow Control\n";return WHILE;}


"return"		{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Flow Control\n";return RETURN;}
"else"			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Flow Control\n";return ELSE;} 
"if"			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Flow Control\n";return IF;}
  
{alpha}{alphanum}*	        { outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Identifier\n";return ID;}
{digits}("."{digits})?	    { outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Number\n";return NUM;}

","			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Separating Character\n";return COMMA;}
"{"			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Separating Character\n";return LCOMMENT;}
"}"			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Separating Character\n";return RCOMMENT;}
"("			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Separating Character\n";return LPAREN;}
")"			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Separating Character\n";return RPAREN;}
"["			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Separating Character\n";return LBRACK;}
"]"			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Separating Character\n";return RBRACK;}
";"			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Separating Character\n";return SEMI;}

"-"			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Arithmetic Operator\n";return MINUS;    }
"+"			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Arithmetic Operator\n";return PLUS;}
"*"			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Arithmetic Operator\n";return TIMES;}
"/"			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Arithmetic Operator\n";return OVER;}
"%"			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Arithmetic Operator\n";return MOD;}
"<"			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Comparison Operator\n";return NEQ;}
">"			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Comparison Operator\n";return LT;}
"!"			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Logical Operator\n";return LOGICNOT;} 

"="			    { outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Assign Operator\n";return ASSIGN;}
"+="			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Assign Operator\n";return PLUSASSIGN;   }
"-="			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Assign Operator\n";return MINUSASSIGN;}
"*="			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Assign Operator\n";return TIMESASSIGN;}
"/="			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Assign Operator\n";return OVERASSIGN;}
"%="			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Assign Operator\n";return MODASSIGN;}
 

"!="			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Comparison Operator\n";return GEQ;}
"&&"			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Logical Operator\n";return LOGICAND;}
"||"			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Logical Operator\n";return LOGICOR;}

"=="			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Comparison Operator\n";return EQ;}
"<="			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Comparison Operator\n";return GT;}
">="			{ outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Comparison Operator\n";return LEQ;}

    
.			        { outputFile<<"token: "<<setw(15)<<yytext<<" lineNumber: "<<LineNum<<"\t  Type:\t  Error: illegal character \n";  return ERROR;  }
			
%%

int main()
{
	outputFile<<std::left;
	int res;
	ifstream inputfile("cminus.c");
	do{ 
		res = yyLex( inputfile );
	}while(res); //when reached the end of file, yyLex returns 0 to suspend the loop
	cout<<"---Lexical analysis has finished\n";
    outputFile.close();
	return 0;
}
