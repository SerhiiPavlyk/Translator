#include "Analizators.h"


DataType Data;
//Returns the number of lexemes
int AnalisisLexems(FILE* FIn)
{
	Lexema* TempLexema;
	int i = 0;		//lexemes counter
	char type[50];
	strcpy(type, "");

	//fill Data
	do
	{
		TempLexema = GetNextLexem(FIn, i);
		strncpy(Data.TableLexems[i].name, TempLexema->name, 49);
		Data.TableLexems[i].value = TempLexema->value;
		Data.TableLexems[i].type = TempLexema->type;
		Data.TableLexems[i].line = TempLexema->line;
		i++;
	} while (TempLexema->type != LEOF);
	return i;
}

// get next Lexeme
Lexema* GetNextLexem(FILE* f, int ii)
{
	char ch, buf[LEX_SIZE];
	bool IsComment = false;
	Lexema* res;
	static int line = 1;
	res = (Lexema*)calloc(1, sizeof(Lexema));
	for (;;)
	{
		ch = getc(f);

		if (ch == '#') //find first symbol to open comentar
		{
			char c;
			c = getc(f);
			if (c == '#') //find second symbol to open  comentar
			{
				strncpy(res->name, "Comentar", 9);
				res->type = LComentar;
				res->value = 0;
				res->line = line;
				for (;;) {
					c = getc(f);
					if (c == '#') //find first symbol to close comentar
					{
						c = getc(f);
						if (c == '\n')
						{
							line++;
						}
						if (c == EOF)
						{
							strncpy(res->name, "EOF", 4);
							res->type = LEOF;
							res->value = 0;
							res->line = line;
						}
						if (c == '#') //find second symbol to close  comentar
						{
							break;
						}
					}
					if (c == '\n')
					{
						line++;
					}
					if (c == EOF)
					{
						strncpy(res->name, "EOF", 4);
						res->type = LEOF;
						res->value = 0;
						res->line = line;
					}
				}
			}
			else
			{
				ungetc(c, f);
			}
		}
		else if (ch == '\n') line++;
		else if (ch == EOF)
		{
			strncpy(res->name, "EOF", 4); //end of file
			res->type = LEOF;
			res->value = 0;
			res->line = line;
			break;
		}
		else if (ch == '(') //opening bracket
		{
			strncpy(res->name, "(", 2);
			res->type = LLBraket;
			res->value = 0;
			res->line = line;
			break;
		}
		else if (ch == ')')//closing bracket
		{
			strncpy(res->name, ")", 2);
			res->type = LRBraket;
			res->value = 0;
			res->line = line;
			break;
		}
		else if (ch == ';') //semicolon
		{
			strncpy(res->name, ";", 2);
			res->type = LSeparator;
			res->value = 0;
			res->line = line;
			break;
		}
		else if (ch == ',') //comma
		{
			strncpy(res->name, ",", 2);
			res->type = LComma;
			res->value = 0;
			res->line = line;
			break;
		}


		else if (ch == '+')
		{
			strncpy(res->name, "+", 2); //addition
			res->type = LAdd;
			res->value = 0;
			res->line = line;
			break;
		}
		else if (ch == '-')
		{
			char c = getc(f);
			if (isdigit(c)) //negative digit
			{
				int i = 2;
				buf[0] = ch;
				buf[1] = c;
				for (; isdigit(ch = getc(f)); ++i)
				{
					buf[i] = ch;
				}
				ungetc(ch, f);
				buf[i] = '\0';
				strncpy(res->name, buf, i);
				res->type = LNumber;
				res->value = atoi(buf);
				res->line = line;
				break;
			}
			else
			{
				strncpy(res->name, "-", 2); //subtraction
				res->type = LSub;
				res->value = 0;
				res->line = line;
				break;
			}
		}
		else if (ch == '*')
		{
			strncpy(res->name, "*", 2); //multiplication
			res->type = LMul;
			res->value = 0;
			res->line = line;
			break;
		}

		else if (ch == '!')
		{
			char c = getc(f);
			if (c == '!')
			{
				strncpy(res->name, "!!", 3); //Logical NOT
				res->type = LNot;
				res->value = 0;
				res->line = line;
				break;
			}
			else if (c == '=')
			{
				strncpy(res->name, "!=", 3); //NOT equal
				res->type = LNe;
				res->value = 0;
				res->line = line;
				break;
			}
			else
			{
				strncpy(res->name, "!", 2);
				res->type = LUnknown;
				res->value = 0;
				res->line = line;
				break;
			}
		}
		else if (ch == '&')
		{
			char c = getc(f);
			if (c == '&')
			{
				strncpy(res->name, "&&", 3); //Logical AND
				res->type = LAnd;
				res->value = 0;
				res->line = line;
				break;
			}
			else
			{
				strncpy(res->name, "&", 2);
				res->type = LUnknown;
				res->value = 0;
				res->line = line;
				break;
			}
		}
		else if (ch == '|')
		{
			char c = getc(f);
			if (c == '|')
			{
				strncpy(res->name, "||", 3); //Logical OR
				res->type = LOr;
				res->value = 0;
				res->line = line;
				break;
			}
			else
			{
				strncpy(res->name, "|", 2);
				res->type = LUnknown;
				res->value = 0;
				res->line = line;
				break;
			}
		}
		else if (ch == '=')
		{
			char c = getc(f);
			if (c == '=')
			{
				strncpy(res->name, "==", 3); // Logical equal
				res->type = LEg;
				res->value = 0;
				res->line = line;
				break;
			}
			else
			{
				strncpy(res->name, "=", 2);
				res->type = LUnknown;
				res->value = 0;
				res->line = line;
				break;
			}
		}
		else if (ch == '<')
		{
			char c = getc(f);
			if (c == '<')
			{
				strncpy(res->name, "<<", 3);// assigment operation
				res->type = LNewValue;
				res->value = 0;
				res->line = line;
				break;
			}
			else
			{
				strncpy(res->name, "<", 2);
				res->type = LUnknown;
				res->value = 0;
				res->line = line;
				break;
			}
		}
		else if (isalpha(ch) || (ch == '_')) // Identificator or keyword
		// '_' for INTEGER32_t 
		{
			int i = 0;
			buf[0] = ch;//first symbol of Identificator or key word
			for (i = 1;; ++i)
			{
				//if next is alphabetic or digit or '_'
				ch = getc(f);
				if ((isdigit(ch) != 0) || (isalpha(ch) != 0) | (ch == '_')) buf[i] = ch;
				else break;
			}
			int j;
			for (j = 0; j < i; j++)
			{
				if (islower(buf[j]))
				{
					break;
				} //check all symbol for low register or digit or _
			}
			ungetc(ch, f);
			buf[i] = '\0';
			strncpy(res->name, buf, i);

			//matching keyword with reserved word
			if (strcmp(buf, "STARTPROGRAM") == 0) //name of program
			{
				res->type = LProgram;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "STARTBLOK")) == 0) //begining of program
			{
				res->type = LBody;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "VARIABLE")) == 0) //keyword for variable declaration
			{
				res->type = LVar;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "INTEGER32_t")) == 0) //data type
			{
				res->type = LVarType;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "ENDBLOK")) == 0) //end of program or for cycle
			{
				res->type = LEnd;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "SCAN")) == 0) //input
			{
				res->type = LInput;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "PRINT")) == 0) //output
			{
				res->type = LOutput;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "FOR")) == 0) //declaration beggining of FOR cycle
			{
				res->type = LFor;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "GE")) == 0) //greate equal
			{
				res->type = LGe;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "LE")) == 0) //less equal
			{
				res->type = LLe;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "DIV")) == 0) //division
			{
				res->type = LDiv;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "IF")) == 0)//declaration beggining of IF 
			{
				res->type = LIf;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "ELSE")) == 0)//declaration beggining of ELSE
			{
				res->type = LElse;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "STARTIF")) == 0)//declaration beggining of IF block 
			{
				res->type = LBodyIf;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "ENDIF")) == 0)//declaration end of IF block 
			{
				res->type = LEndIf;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "MOD")) == 0)//MOD
			{
				res->type = LMod;
				res->line = line;
				break;
			}
			else if ((j <= i) && ((i <= 8)))//name of variables (identificator)
			{
				strncpy(res->name, buf, i);
				res->type = LIdentifier;
				res->value = 1;
				res->line = line;
				break;
			}
			else
			{
				strncpy(res->name, buf, i);
				res->type = LUnknown;
				res->value = 0;
				res->line = line;
				break;
			}
			res->value = 0;
			res->line = line;
		}
		else if (isdigit(ch)) //possitive digit
		{
			int i = 1;
			buf[0] = ch;
			for (; isdigit(ch = getc(f)); ++i)
			{
				buf[i] = ch;
			}
			ungetc(ch, f);
			buf[i] = '\0';
			strncpy(res->name, buf, i);
			res->type = LNumber;
			res->value = atoi(buf);
			res->line = line;
			break;
		}
		else if ((ch != '\n') && (ch != '\t') && (ch != ' '))//something unknown
		{
			char bufer[LEX_SIZE];
			char c;
			int i;
			bufer[0] = ch;
			for (i = 1;; i++)
			{
				c = fgetc(f);
				if ((c == '\n') || (c == '\t') || (c == ' ') || (c == ';'))
				{
					if (c == '\n')
					{
						line++;
						break;
					}
					else break;
				}
				bufer[i] = c;
			}
			ungetc(c, f);
			bufer[i] = '\0';
			strncpy(res->name, bufer, i);
			res->type = LUnknown;
			res->value = 0;
			res->line = line;
			break;
		}
	}
	return res;
}

void PrintLexemsInFile()//print table of lexemes to file
{
	FILE* outTokensFiles;
	char type[MAX_LENGTH_TYPES];
	int i;
	outTokensFiles = fopen("lexems.txt", "w"); //opening file for writing
	//writing lexemes and analysis to file
	fputs("Table of lexemes!\n\n", outTokensFiles);
	fprintf(outTokensFiles, "--------------------------------------------------------------------\n");
	fprintf(outTokensFiles, "|%5s \t%s \t%12s %20s %10s \n", "Line", "¹", "Name", "Type", "Value|");
	fprintf(outTokensFiles, "--------------------------------------------------------------------\n");
	for (i = 0; i < Data.LexNum; ++i)
	{
		switch (Data.TableLexems[i].type)
		{
		case LProgram: strncpy(type, "Program", 8); break;
		case LBody: strncpy(type, "Body", 5); break;
		case LVar: strncpy(type, "Var", 5); break;
		case LVarType: strncpy(type, "INTEGER32_t", 7); break;
		case LIdentifier: strncpy(type, "Identifier", 11); break;
		case LEnd: strncpy(type, "End", 4); break;
		case LInput: strncpy(type, "Input", 4); break;
		case LOutput: strncpy(type, "Ouput", 4); break;
		case LFor: strncpy(type, "For", 3); break;
		case LIf: strncpy(type, "If", 3); break;
		case LBodyIf: strncpy(type, "BodyIf", 7); break; ;;;
		case LElse: strncpy(type, "Else", 5); break;
		case LEndIf: strncpy(type, "EndIf", 6); break;
		case LNewValue: strncpy(type, "NewValue", 9); break;
		case LAdd: strncpy(type, "Add", 4); break;
		case LSub: strncpy(type, "Sub", 4); break;
		case LMul: strncpy(type, "Mul", 4); break;
		case LDiv: strncpy(type, "Div", 4); break;
		case LMod: strncpy(type, "Mod", 4); break;
		case LEg: strncpy(type, "Equal", 4); break;
		case LNe: strncpy(type, "NotEqu", 7); break;
		case LLe: strncpy(type, "Less", 5); break;
		case LGe: strncpy(type, "Greate", 7); break;
		case LNot: strncpy(type, "Not", 4); break;
		case LAnd: strncpy(type, "And", 4); break;
		case LOr: strncpy(type, "Or", 3); break;
		case LLBraket: strncpy(type, "LeftBraket", 11); break;
		case LRBraket: strncpy(type, "RightBraket", 12); break;
		case LNumber: strncpy(type, "Number", 7); break;
		case LSeparator: strncpy(type, "Separator", 10); break;
		case LComma: strncpy(type, "Comma", 6); break;
		case LEOF: strncpy(type, "EndOfFile", 10); break;
		case LComentar: strncpy(type, "Comentar", 9); break;
		case LUnknown: strncpy(type, "Unknown", 8); break;
		}
		fprintf(outTokensFiles, "%5d \t%d \t%12s %20s %10d \n", Data.TableLexems[i].line, i + 1, Data.TableLexems[i].name, type, Data.TableLexems[i].value);
	}
	fclose(outTokensFiles);
}