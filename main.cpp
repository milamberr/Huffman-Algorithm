#include "stdafx.h"
#include "HuffmanCoding.h"
#include<iostream>
#include<string>
#include<time.h>

void run(char c, const std::string& input, const std::string& output)
{
	Coder letscode;
	if (c == 'c')
	{
		clock_t t = clock();

		letscode.compress(input, output);

		t = clock() - t;
		printf("Compression completed for %f \n\n\n", (float)t / CLOCKS_PER_SEC);
	}
	if (c == 'd')
	{
		clock_t t = clock();

		letscode.decompress(input, output);

		t = clock() - t;
		printf("Decompression completed for %f \n\n\n", (float)t / CLOCKS_PER_SEC);
	}
}


void userInterface()
{
	std::string inputFileName;
	std::string outputFileName;
	std::string option;
	char c;
	printf("No checkings if the files are correct,please enter correct files \n"
		"Menu \nChoose option: \" c \" for compression or \" d \" for decompression \n");
	std::cin >> c;
	std::cin.ignore();
	printf("Now enter filenames for input and output\nCommand for input is: i fname\nCommand for output is: o fname\n");
	printf("See project folder for the names of the files\n(To Fix)Not working with empty file\n");
	while(inputFileName.empty() || outputFileName.empty())
	{
		
		std::getline(std::cin, option);
		if (option[0] == 'i')
		{
			option.erase(0, 2);
			inputFileName = option;
		}
		else if (option[0] == 'o')
		{
			option.erase(0, 2);
			outputFileName = option;
		}
		if (inputFileName.empty() || outputFileName.empty())
		{
			printf("One or more filenames are not defined. \n");
		}
	}
	std::cout << "\n\n";
	//run the program
	run(c, inputFileName, outputFileName);
	
}


void test()
{
	Coder letscode;

	clock_t t = clock();
	letscode.compress("toCompress.txt", "toDecompress.txt");
	letscode.decompress("toDecompress.txt", "result.txt");

	t = clock() - t;

	std::cout << "\n"<<(float)t / CLOCKS_PER_SEC;
}
int main()
{
	userInterface();
	//test();
	system("pause");
	return 0;
}