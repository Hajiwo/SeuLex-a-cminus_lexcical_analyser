#ifndef METHOD_RE_H
#define METHOD_RE_H

#include"Global_Val.h"

class method_RE{
private:
public:
/*
i:  RE in suffix form
o:  RE in postfix form
*/

void genearte_postfix_RE(Pattern* );

void standardize_RE(Pattern*);

void standardize_quoteSymbols(Pattern*);
};

#endif