#include"Read_Lex.h"
#include"Test.h"
#include <chrono>
using namespace chrono;
int main() {
    
    auto start = high_resolution_clock::now();

    read_lex Lex_reader;   // Open the input file

    string filePath= CLEX_L;
    Lex_reader.read_FILE_LEX(filePath);      //read LEX.l info
    auto end = high_resolution_clock::now();
    duration<double> duration = end - start;
    cout << "Runtime: " << duration.count() << " seconds" << endl;
    start = high_resolution_clock::now();
    

    Lex_reader.standardize_REs();          //REs standardization
    end = high_resolution_clock::now();
    duration = end - start;
    cout << "Runtime: " << duration.count() << " seconds" << endl;
    start = high_resolution_clock::now();

    Lex_reader.generate_DFA_from_LEX();    //DFA generation
    end = high_resolution_clock::now(); 
    duration = end - start;
    cout << "Generating time: " << duration.count() << " seconds" << endl; 

    
    Lex_reader.generate_yyLEX(LEX_YY_C);    //lex.yy.cpp generation
  
}
