#include "CodeGenarator.h"

bool IsOperation(TypeOfLex t)
{
	bool r;
	r = ((t == LAdd) || (t == LSub) || (t == LMul) || (t == LDiv) || (t == LMod) || (t == LNot) ||
		(t == LAnd) || (t == LOr) || (t == LEg) || (t == LNe) || (t == LLe) || (t == LGe));
	return r;
}
int IsExpression(int i, FILE* ef)
{
	int nom, error = 0;
	nom = i;
	//check if expression begins from identifier, number or '(' 
	if ((Data.TableLexems[nom].type != LLBraket) && (Data.TableLexems[nom].type != LIdentifier) && (Data.TableLexems[nom].type != LNumber))
	{
		fprintf(ef, "line %d: \tExpression must begin from identifier, number or '('  !\n", Data.TableLexems[nom].line);
		error++;
	}
	for (; (Data.TableLexems[nom].type != LSeparator); nom++)
	{
		//check if after ')' there are ')', operation or ';'
		if (Data.TableLexems[nom].type == LRBraket)
			if (!(IsOperation(Data.TableLexems[nom + 1].type)) && (Data.TableLexems[nom + 1].type != LSeparator) && (Data.TableLexems[nom + 1].type != LRBraket))
			{
				fprintf(ef, "line %d: \tAfter ')' must be ')', operation or ';'!\n", Data.TableLexems[nom].line);
				error++;
			}
		//check if after '(' there are '(' or identifier
		if (Data.TableLexems[nom].type == LLBraket)
		{
			if ((Data.TableLexems[nom + 1].type != LIdentifier) && (Data.TableLexems[nom + 1].type != LLBraket) &&
				(Data.TableLexems[nom + 1].type != LNumber) && (Data.TableLexems[nom + 1].type != LNot))
			{
				fprintf(ef, "line %d: \tAfter '(' must be '(' or identifier!\n", Data.TableLexems[nom].line);
				error++;
			}
		}//check if after operation there are '(' or identifier

		if (IsOperation(Data.TableLexems[nom].type) && Data.TableLexems[nom].type != LNot)
		{
			if ((Data.TableLexems[nom + 1].type != LIdentifier) && (Data.TableLexems[nom + 1].type != LLBraket) && (Data.TableLexems[nom + 1].type != LNumber) && (Data.TableLexems[nom + 1].type != LNot))
			{
				fprintf(ef, "line %d: \tAfter operation must be '(' or idetifier!\n", Data.TableLexems[nom].line);
				error++;
			}
		}
		//check if after identifier there are  ')' or ';' or operation!
		if ((Data.TableLexems[nom].type == LIdentifier) || (Data.TableLexems[nom].type == LNumber))
		{
			if (!(IsOperation(Data.TableLexems[nom + 1].type)) && (Data.TableLexems[nom + 1].type != LRBraket) && (Data.TableLexems[nom + 1].type != LSeparator))
			{
				fprintf(ef, "line %d: \tAfter identifier must be ')' or ';' or operation!\n", Data.TableLexems[nom].line);
				error++;
			}
		}
	}
	return error;
}

int Balans(int nom, TypeOfLex ends, TypeOfLex ltStart, TypeOfLex ltFinish)
{
	StackT ss;
	int j = 0, i;
	i = nom;
	ss.Init(&ss.S);
	for (; Data.TableLexems[i].type != ends; i++)
	{
		if (Data.TableLexems[i].type == ltStart)
		{
			ss.Push(i, &ss.S);
		}
		if (Data.TableLexems[i].type == ltFinish)
		{
			if (ss.IsEmpty(&ss.S))
			{
				j = 1;	//  Too much ')';
				break;
			}
			else
			{
				ss.Pop(&ss.S);
			}
		}
	}
	if (!(ss.IsEmpty(&ss.S)))
	{
		j = 2;	// ')' expected;
	}
	return j;
}

int ErrorChecking()
{
	int ifNumb = 0;
	int label = 0;
	int i = 0, j = 1, temp = 0, ValNum = 0;
	FILE* ef;
	int while_num = 0, STARTBLOK_ENDBLOK_num = 0;
	int Err_num = 0;

	ef = fopen("errors.txt", "w");
	fputs("Error list:\n\n\n", ef);

	//check if the first word in the program is "STARTPROGRAM"
	if (Data.TableLexems[0].type != LProgram)
	{
		Err_num++;
		fprintf(ef, "%d - line %d: \t'STARTPROGRAM' expected! (program must begin from the keyword 'STARTPROGRAM')\n", Err_num, Data.TableLexems[0].line);
	}


	//check if the second word in the program or in for cycle is "STARTBLOK"
	if (Data.TableLexems[1].type != LBody)
	{
		Err_num++;
		fprintf(ef, "%d - line %d: \t'STARTBLOK' expected!\n", Err_num, Data.TableLexems[1].line);
	}
	//check if the last word in the program or in for cyclei s "ENDBLOK"
	if (Data.TableLexems[Data.LexNum - 2].type != LEnd)
	{
		Err_num++;
		fprintf(ef, "line %d: \t'ENDBLOK' expected!\n", Data.TableLexems[1].line);
	}
	//check if the file has end
	if (Data.TableLexems[Data.LexNum - 1].type != LEOF)
	{
		Err_num++;
		fprintf(ef, "line %d: \tEnd of file expected!\n", Data.TableLexems[0].line);
	}
	//check if the third word in the programe is "VARIABLE"
	if (Data.TableLexems[2].type != LVar)
	{
		Err_num++;
		fprintf(ef, "line %d: \t'VARIABLE' expected!\n", Data.TableLexems[i].line);
	}

	//check if the next word after VARIABLE in the program is INTEGER32_t
	if (Data.TableLexems[3].type != LVarType)
	{
		Err_num++;
		fprintf(ef, "%d line %d: \t'INTEGER32_t' expected!\n", Err_num, Data.TableLexems[3].line);
	}
	if (Data.TableLexems[3].type == LVarType)
	{
		i = 4;
		//check if after INTEGER32_t there are identifiers
		if (Data.TableLexems[i].type != LIdentifier)
		{
			Err_num++;
			fprintf(ef, "%d - line %d: \tIdentifier expected!\n", Err_num, Data.TableLexems[i].line);
		}
		else
		{
			do
			{
				//check if after identifiers is ','
				if ((Data.TableLexems[i].type == LIdentifier) && (Data.TableLexems[i + 1].type == LComma))
				{
					strcpy(Data.IdTable[Data.IdNum].name, Data.TableLexems[i].name);
					for (int i = 0; i < Data.IdNum; i++)
					{
						if (strcmp(Data.IdTable[i].name, Data.IdTable[Data.IdNum].name) == 0)
						{
							Err_num++;
							fprintf(ef, "%d - line %d: \tThe identifier must be specified only once!\n", Err_num, Data.TableLexems[Data.IdNum].line);
						}
					}

					Data.IdNum++;
					i = i + 2;
				}
			} while ((Data.TableLexems[i].type == LIdentifier) && (Data.TableLexems[i + 1].type == LComma));
			//check if after identifiers is ';'
			if ((Data.TableLexems[i].type == LIdentifier) && (Data.TableLexems[i + 1].type == LSeparator))
			{
				strcpy(Data.IdTable[Data.IdNum].name, Data.TableLexems[i].name);
				for (int i = 0; i < Data.IdNum; i++)
				{
					if (strcmp(Data.IdTable[i].name, Data.IdTable[Data.IdNum].name) == 0)
					{
						Err_num++;
						fprintf(ef, "%d - line %d: \tThe identifier must be specified only once!\n", Err_num, Data.TableLexems[Data.IdNum].line);
					}
				}
				Data.IdNum++;
				i = i + 2;
				goto label1;
			}
			if (Data.TableLexems[i].type != LSeparator)
			{
				if (Data.TableLexems[i].type == LComma)
				{
					Err_num++;
					fprintf(ef, "line %d: \tToo much comma!\n", Data.TableLexems[i].line);
				}
				else
				{
					Err_num++;
					fprintf(ef, "line %d: \t';' expected!\n", Data.TableLexems[i].line);
				}
			}
			else
			{
				Err_num++;
				fprintf(ef, "line %d: \tToo much commas or identifier expected!\n", Data.TableLexems[i].line);
			}
		}
		i++;
	label1:;
	}

	switch (Balans(i, LEOF, LIf, LBodyIf))
	{
	case 1:
		Err_num++;
		fprintf(ef, "line %d: \tToo much 'IF STARTIF'!\n", Data.TableLexems[j].line);
		break;
	default:
		break;
	}
	switch (Balans(i, LEOF, LIf, LEndIf))
	{
	case 1:
		Err_num++;
		fprintf(ef, "line %d: \tToo much 'IF ENDIF'!\n", Data.TableLexems[j].line);
		break;
	case 2:
		Err_num++;
		fprintf(ef, "line %d: \t'If ENDIF' expected!\n", Data.TableLexems[j].line);
		break;
	default:
		break;
	}

	for (j = 0; j < Data.LexNum; j++)
	{
		int s = 0, e = 0;
		if (Data.TableLexems[j].type == LBody)
		{
			s++;
		}
		else if (Data.TableLexems[j].type == LEnd)
		{
			e++;
		}
		if (s > 1)
		{
			Err_num++;
			fprintf(ef, "line %d: \tToo much 'STARTBLOK'!\n", Data.TableLexems[j].line);
		}
		if (e > 1)
		{
			Err_num++;
			fprintf(ef, "line %d: \tToo much 'ENDBLOK'!\n", Data.TableLexems[j].line);
		}
	}

	for (j = 0;; j++)
	{
		if (Data.TableLexems[j].type == LUnknown)        //Search for unknown words (not identifiers)
		{
			Err_num++;
			fprintf(ef, "line %d: \tUnknown identifier!\t-->\t%s\n", Data.TableLexems[j].line, Data.TableLexems[j].name);
		}
		if ((Data.TableLexems[j].type == LIdentifier) && (j > i))
		{
			bool available = 0;
			for (int i = 0; i < Data.IdNum; i++)
			{
				if (strcmp(Data.IdTable[i].name, Data.TableLexems[j].name) == 0)
				{
					available = 1;
				}
			}
			if (!available)
			{
				Err_num++;
				fprintf(ef, "line %d: \tUnknown identifier!\t-->\t%s\n", Data.TableLexems[j].line, Data.TableLexems[j].name);
			}
		}
		if (Data.TableLexems[j].type == LNewValue)
		{
			int buf;
			if (Data.TableLexems[j - 1].type == LIdentifier)
			{
				buf = IsExpression((j + 1), ef);
			}
			else
			{
				buf = 1;
			}
			Err_num = Err_num + buf;
		}

		if (Data.TableLexems[j].type == LInput)
		{

			if (Data.TableLexems[j + 1].type != LIdentifier)
			{
				Err_num++;
				fprintf(ef, "line %d: \tIdentifier expected!\n", Data.TableLexems[j + 2].line);
			}
			if (Data.TableLexems[j + 2].type != LSeparator)
			{
				Err_num++;
				fprintf(ef, "line %d: \t';' expected!\n", Data.TableLexems[j + 4].line);
			}
		}

		if (Data.TableLexems[j].type == LFor)				//check keyword FOR
		{
			if (Data.TableLexems[j + 1].type != LLBraket)
			{
				Err_num++;
				fprintf(ef, "line %d: \t'(' expected after 'FOR'!\n", Data.TableLexems[j + 1].line);
			}
			else if (Data.TableLexems[j + 2].type != LIdentifier)
			{
				Err_num++;
				fprintf(ef, "line %d: \tIdentifier expected after 'FOR'!\n", Data.TableLexems[j + 1].line);
			}
			else if (Data.TableLexems[j + 3].type != LNewValue)
			{
				Err_num++;
				fprintf(ef, "line %d: \t'->' expected after identifier!\n", Data.TableLexems[j + 2].line);
			}
			else if (IsExpression(j + 4, ef) != 0)
			{
				Err_num++;
				fprintf(ef, "line %d: \tIdentifier or number expected after '->'!\n", Data.TableLexems[j + 3].line);
			}
			while ((Data.TableLexems[j].type != LSeparator) && (Data.TableLexems[j].type != LEOF)) j++;

			if (Data.TableLexems[j + 1].type != LIdentifier)
			{
				Err_num++;
				fprintf(ef, "line %d: \tIdentifier expected after 'FOR'!\n", Data.TableLexems[j + 1].line);
			}
			else if ((Data.TableLexems[j + 2].type != LLe) && (Data.TableLexems[j + 2].type != LGe))
			{
				Err_num++;
				fprintf(ef, "line %d: \t'LE' or 'GE' expected after identifier!\n", Data.TableLexems[j + 2].line);
			}
			else if (IsExpression(j + 3, ef) != 0)
			{
				Err_num++;
				fprintf(ef, "line %d: \tIdentifier or number expected after '->'!\n", Data.TableLexems[j + 5].line);
			}
			j++;
			while ((Data.TableLexems[j].type != LSeparator) && (Data.TableLexems[j].type != LEOF)) j++;
			if (Data.TableLexems[j + 1].type != LIdentifier)
			{
				Err_num++;
				fprintf(ef, "line %d: \t Identifier expected!\n", Data.TableLexems[j + 3].line);
			}
			else if (Data.TableLexems[j + 2].type != LNewValue)
			{
				Err_num++;
				fprintf(ef, "line %d: \t '->' expected after identifier!\n", Data.TableLexems[j + 3].line);
			}
			else if (IsExpression(j + 3, ef) != 0)
			{
				Err_num++;
				fprintf(ef, "line %d: \t number or identifier expected!\n", Data.TableLexems[j + 3].line);
			}
			j++;
			while ((Data.TableLexems[j].type != LSeparator) && (Data.TableLexems[j].type != LEOF)) j++;
			if (Data.TableLexems[j + 2].type != LBody)
			{
				Err_num++;
				fprintf(ef, "line %d: \t'STARTBLOK' expected!\n", Data.TableLexems[j + 6].line);
			}
		}
		if (Data.TableLexems[j].type == LEOF) break;
	}
	if (Err_num == 0) fprintf(ef, "No errors found.");
	return Err_num;
}
