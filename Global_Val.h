#ifndef GLOBAL_VAL_H
#define GLOBAL_VAL_H
#include"NFA.h"
#include"DFA.h"
#include"Pattern.h"
#include<iostream>
#include <unordered_map>
#include <algorithm>
#include<vector>
#include<unordered_set>
#include<filesystem>


const int DSIZE = 1000;


/*
used in Automata:
*/
const char EPSILON='\0';         //since there will be no @ in the RE(delete ws in the early stage) so it can represent epsilon
const int MULTI_ACCEPTING_STATES = -1;
const int START_STATE_OF_MERGED_NFA = 0;
const int START_STATE_OF_DFA = 0;
const int NO_NEXT_STATE = -1;
const int DEAD_STATE = -1;
typedef unordered_set< int> subset;
const int NOT_FOUND_IN_DSTATES = -1;

static int PatternNumber;
const int SYMBOLS = 128;
const int MAX_DFA_STATES = 10000;

static string CURRENTPATH = filesystem::current_path();
static string INPUTPATH = CURRENTPATH+"/input/";
static string OUTPUTPATH = CURRENTPATH+"/output/";
static string INTERMEDIATEPATH = CURRENTPATH+"/inter_File/";
static string SRCPATH = CURRENTPATH+"/srcFile/";
static string AUTOMATAINFO = CURRENTPATH+"/Automata_Info/";

const string CLEX_L = INPUTPATH + "c_minus.l";
const string FILENAME_SEC1 = INTERMEDIATEPATH + "SECTION1.l";
const string LEX_YY_C = OUTPUTPATH + "lex.yy.cpp";
const string FILENAME_SEC3 = INTERMEDIATEPATH + "SECTION3.l";
const string FILENAME_OUTPUT = OUTPUTPATH + "cLex_yy.cpp";
const string OPT_DFA_FILE = AUTOMATAINFO +"OPT_DFA.l";
const string DFA_FILE = AUTOMATAINFO +"DFA.l";
const string NFA_FILE = AUTOMATAINFO + "NFA.l";
const string FUCTION_FILE = SRCPATH + "FUCTION.l";
#endif