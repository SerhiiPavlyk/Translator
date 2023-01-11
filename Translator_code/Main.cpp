#include "Analizators.h"
#include "CodeGenarator.h"
#include "Errors.h"
#include "Data.h"
#include <process.h>
#include <io.h>
#include <iostream>


FILE* InputFile, * OutputFile;


bool directory_exists(const std::string& directory)
{
	if (!directory.empty())
	{
		if (_access(directory.c_str(), 0) == 0)
		{
			struct stat status;
			stat(directory.c_str(), &status);
			if (status.st_mode & S_IFDIR)
				return true;
		}
	}
	// if any condition fails
	return false;
}

int main(int argc, std::string* argv)
{
	printf("TRANSLATOR:\n\tPavlykLanguage -> ASSEMBLER\n");

	// Checking the number of arguments to be passed from the command line
	if (argc != 2)
	{
		printf("Input file name: ");
		std::cin >> Data.InputFileName;
		//Data.InputFileName = "3.16";
	}
	else
	{
		// Obtaining and formation names of incoming and outgoing files
		Data.InputFileName = argv->c_str();
	}

	if ((InputFile = fopen(Data.InputFileName.c_str(), "r")) == 0)
	{
		printf("Error: Can not open file: %s\n", Data.InputFileName.c_str());
		system("pause");
		exit(1);
	}

	printf("Start translating file: %s\n", Data.InputFileName.c_str());

	int k = Data.InputFileName.size();
	while (k > 0)
	{
		if (Data.InputFileName[k] == '\\')
		{
			k++;
			break;
		}
		k--;
	}
	// 4 = size of ".pavlyk"
	Data.InputFileName.copy(Data.OutputFileName, Data.InputFileName.size() - k - 4, k);
	printf("Output file: %s\n\n\n", std::string(std::string(Data.OutputFileName) + ".asm").c_str());

	// Breaking into lexemes and printing into file
	printf("Breaking into lexemes are starting...\n");
	Data.LexNum = AnalisisLexems(InputFile);
	PrintLexemsInFile();
	printf("Breaking into lexemes completed. There are %d lexemes.\nReport file: lexemes.txt\n\n\n", Data.LexNum);

	// Error checking
	printf("Error checking are starting...\n");
	Data.numberErrors = ErrorChecking();
	printf("Error checking is complete. There is(are) %d errors.\nReport file: errors.txt\n\n\n", Data.numberErrors);

	// If there are no errors, go to a translation stage
	if (Data.numberErrors != 0)
	{
		printf("Translation can not be continued. Errors were found. Please correct the errors and try again.\n");
	}
	else
	{
		if ((OutputFile = fopen(std::string(std::string(Data.OutputFileName) + ".asm").c_str(), "w")) == 0)
		{
			printf("Error: Can't create file: %s\n", std::string(std::string(Data.OutputFileName) + ".asm").c_str());
			system("pause");
			exit(1);
		}


		// Creating assembly code
		printf("Code generation is starting...\n");
		GenerateCode(OutputFile);
		printf("Code generation is finish.\n");
		if (OutputFile != NULL)
			fclose(OutputFile);

		// If masm32 is installed
		if (directory_exists("masm32"))
		{
			system(std::string("masm32\\bin\\ml /c /coff " + std::string(Data.OutputFileName) + ".asm").c_str());
			system(std::string("masm32\\bin\\Link /SUBSYSTEM:WINDOWS " + std::string(Data.OutputFileName) + ".obj").c_str());
			system(std::string("del " + std::string(Data.OutputFileName) + ".obj").c_str());
		}
		else
		{
			printf("WARNING!\n");
			printf("Can't compile asm file, because masm32 doesn't exist.\n");
		}

		printf("\tDONE!\n");
	}
	system("pause");
	return 0;
}