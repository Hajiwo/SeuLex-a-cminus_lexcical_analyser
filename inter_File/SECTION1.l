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
