#pragma once
#include "Data.cpp"
bool IsOperation(TypeOfLex);							//check for operation 
int IsExpression(int, FILE*);							//check for expression 
int Balans(int, TypeOfLex, TypeOfLex, TypeOfLex);   //check for balans ()
int ErrorChecking();									//check for error
