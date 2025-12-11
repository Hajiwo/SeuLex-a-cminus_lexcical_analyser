
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
