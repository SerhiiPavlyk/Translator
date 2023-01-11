#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include "malloc.h"
#include "string.h"
#include "stdlib.h"
#include "ctype.h"
#include <string>

#define MAX_LEXEMS 1000			//maximum amount of lexemes
#define MAX_IDENT 100			//maximum amount of identifiers 
#define MAX_BUF_SIZE 100		//
#define STACK_SIZE 200			//maximum size of stack
#define MAX_LENGTH_TYPES 20		//maximum size of lexem's category
#define LEX_SIZE 50				//maximum size of lexem names
#define MAX_OUT_Name 50			//maximum size of output file

extern enum TypeOfLex;			//type of lexeme

typedef struct Lexem;			//struct lexeme

typedef struct ID;				//struct identificator

typedef struct L;

typedef struct Stacks;

typedef class stack;
