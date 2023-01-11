#pragma once
#include "Data.h"

enum TypeOfLex
{
	LProgram,		// STARTPROGRAM
	LBody,			// STARTBLOK
	LVar,			// VARIABLE
	LVarType,		// INTEGER32_t
	LIdentifier,	// NAMEVAR
	LEnd,			// ENDBLOK

	LInput,			// SCAN
	LOutput,		// PRINT


	LFor,			// FOR

	LIf,			// IF
	LBodyIf,		// BodyIf
	LElse,			// ELSE
	LEndIf,		    // EndIf


	LNewValue,		// <<
	LAdd,			// +
	LSub,			// -
	LMul,			// *
	LDiv,			// DIV
	LMod,			// MOD
	LEg,			// ==
	LNe,			// !=
	LLe,			// LESS EQUAL
	LGe,			// GREATE EQUAL
	LNot,			// !!
	LAnd,			// &&
	LOr,			// ||

	LLBraket,		// (
	LRBraket,		// )
	LNumber,		// NUMBER
	LSeparator,		// ;
	LComma,			// ,
	LEOF,			// END FILE
	LComentar,      // ## 
	LUnknown
};

typedef struct Lexem
{
	char name[LEX_SIZE];
	int value;
	int line;								//a line of code on which a lexeme is present
	TypeOfLex type;
}Lexema;

typedef struct ID
{
	char name[LEX_SIZE];
	int value;
}Identificator;


typedef struct L
{
	Lexema TableLexems[MAX_LEXEMS];			//table of lexemes
	int LexNum;								//index of lexeme

	Identificator IdTable[MAX_IDENT];		//table of identificator
	int IdNum;								//index of identificator

	bool IsPresentInput,					//indication of the presence of operators
		IsPresentOutput,
		IsPresentMod,
		IsPresentAnd,
		IsPresentOr,
		IsPresentNot,
		IsPresentEqu,
		IsPresentGreate,
		IsPresentLess,
		IsPresentDiv;

	int numberErrors;						//number of errors

	std::string InputFileName;				//my language file code name
	char OutputFileName[MAX_OUT_Name];		//asm file code name

	int bufExprPostfixForm[MAX_BUF_SIZE];	//buffer for expression in postfix form
}DataType;

typedef struct Stacks
{
	int st[STACK_SIZE];						//the number of operations in stacks
	int top;								//top of stacks
}StackType;

typedef class stack
{
public:
	StackType S;							//stack
	void Init(StackType* s)					//the number of operations in the stack
	{
		s->top = -1;
	}

	void Push(int i, StackType* s)			//push operation to stack
	{
		if (IsFull(s))
		{
			puts("Stack error (is full)");
			exit(0);
		}
		else
		{
			++s->top;
			s->st[s->top] = i;
		}
	}
	int Pop(StackType* s)					//pop operation from stack
	{
		int i;
		if (IsEmpty(s))
		{
			puts("Stack error (is empty)");
			exit(0);
		}
		else
		{
			i = s->st[s->top];
			--s->top;
		}
		return i;
	}
	bool IsEmpty(StackType* s)				//check if the stack is empty
	{
		if (s->top == -1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool IsFull(StackType* s)				//check if the stack is full
	{
		if (s->top == STACK_SIZE - 1)return true;
		else return false;
	}
}StackT;

extern FILE* ErrorF;

extern DataType Data;