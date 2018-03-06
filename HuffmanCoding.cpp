#include "stdafx.h"
#include "HuffmanCoding.h"
#include <cassert>
#include<string>
#include<bitset>

int Node::maxid = 0;

//constructor for compression
HuffmanTree::HuffmanTree(std::map<char, int>& freq_map)
{
	sortSymbols(freq_map);
	buildTree();
	root = nodes[nodes.size() - 1];

	std::ofstream out("HuffmanTree.txt");
	printNodesLISP(out, root);


	std::ofstream treeViz("treeViz.dot");
	treeViz << "digraph G{";
	printDotty(treeViz, root);
	treeViz << "}";
}

//constructor for decompression
HuffmanTree::HuffmanTree(std::istream& in)
{
	root=buildTree(in);

	//std::ofstream out("checkTree.txt");
	//printNodesLISP(out, root);
}
//creates a sorted vector with pairs,each containing character from the frequency map and its number of occurencies
void HuffmanTree::sortSymbols(std::map<char, int>& freq_map)
{
	//fill the vector
	for (std::map<char, int>::iterator it = freq_map.begin(); it != freq_map.end(); ++it)
	{
		nodes.push_back(new Node(std::pair<char, int>(it->first, it->second), nullptr, nullptr));
	}
	//sort the vector/slow sort
	for (int i = 0; i < nodes.size(); i++)
	{
		for (int j = i + 1; j < nodes.size(); j++)
		{
			if (nodes[i]->data.second > nodes[j]->data.second)
			{
				Node *tmp = nodes[i];
				nodes[i] = nodes[j];
				nodes[j] = tmp;
			}
		}
	}
	/*
	std::cout << std::endl;
	for (int i = 0; i < nodes.size(); i++)
	{
	std::cout << nodes[i]->data.first << " " << nodes[i]->data.second << std::endl;
	}
	*/

}

//builds the tree of Huffman for compression
void HuffmanTree::buildTree()
{
	int length = nodes.size() * 2 - 1;//the number of the elements in the tree which we will build
	int curr = 0;
	if(length<=0)return;
	if (length == 1)
	{
		nodes.push_back(new Node(std::pair<char, int>('~', nodes[0]->data.second), nodes[0], nullptr));
		return;
	}
	while (nodes.size()>1 && nodes.size() < length)
	{
		Node* createFromMin = new Node(std::pair<char, int>('~', nodes[curr]->data.second + nodes[curr + 1]->data.second)
			, nodes[curr], nodes[curr + 1]);//take the two min nodes and create a new node from them
											
		for (int i = 0; i < nodes.size(); i++)//insert the node at the right place
		{
			if (createFromMin->data.second <= nodes[i]->data.second)
			{
				nodes.insert(nodes.begin() + i, createFromMin);
				break;
			}
			else if (i == nodes.size() - 1)
			{
				nodes.push_back(createFromMin);
				break;
			}
		}
		curr += 2;
	}
}

//builds the tree of Huffman for decompression
Node* HuffmanTree::buildTree(std::istream& in)
{
	char nextChar;
	nextChar = in.get();
	assert(nextChar == '(');

	nextChar = in.get();
	assert(nextChar == ' ' || nextChar == ')');

	if (nextChar == ')')
	{
		return nullptr;
	}
	

	nextChar = in.get();
	assert(nextChar == '[');

	std::pair<char,int> rootValue;
	char character;
	int sum;
	character = in.get();

	nextChar = in.get();//reads the '-'
	assert(nextChar == '-');

	in >> sum;

	rootValue.first = character;
	rootValue.second = sum;

	nextChar = in.get();//read the ']'
	assert(nextChar == ']');

	//interval

	nextChar = in.get();
	assert(nextChar == ' ');

	// leftSubTree

	Node *leftSubTree;
	leftSubTree = buildTree(in);

	//interval

	nextChar = in.get();
	assert(nextChar == ' ');

	//RightSubTree

	Node *rightSubTree;
	rightSubTree = buildTree(in);

	//interval

	nextChar = in.get();
	assert(nextChar == ' ');

	//close bracket

	nextChar = in.get();
	assert(nextChar == ')');

	return new Node(rootValue, leftSubTree, rightSubTree);
}
//print huffman tree in 2 ways
void HuffmanTree::printNodesLISP(std::ostream& out, Node* root)
{
	if (root == nullptr)
	{
		out << "()";
		return;
	}

	out << "( "
		<< "[" << root->data.first << "-" << root->data.second << "]"
		<< " ";
	printNodesLISP(out, root->left);
	out << " ";
	printNodesLISP(out, root->right);
	out << " )";
}

void HuffmanTree::printDotty(std::ostream& out, Node* root)
{
	if (!root)
	{
		return;
	}
	out << root->getid()
		<< "[label=\""
		<< root->data.first << " " << root->data.second
		<< "\"];"
		<< std::endl;

	if (root->left != nullptr)
	{
		out << root->getid()
			<< "->"
			<< root->left->getid()
			<< std::endl;
	}

	if (root->right != nullptr)
	{
		out << root->getid()
			<< "->"
			<< root->right->getid()
			<< ";"
			<< std::endl;
	}
	printDotty(out, root->left);
	printDotty(out, root->right);
}


HuffmanTree::~HuffmanTree()
{
	//deleteTree(root);
}
void HuffmanTree::deleteTree(Node* root)
{
	if (!root)return;
	deleteTree(root->left);
	deleteTree(root->right);

	delete root;
	root = nullptr;
}
//create binary code for each character
void Coder::createCodes(Node* root, std::string code)
{
	if (!root)return;
	if (!root->left && !root->right)
	{
		comp_map.insert(std::pair<char, std::string>(root->data.first, code));
		return;
	}
	createCodes(root->left, code + '0');
	createCodes(root->right, code + '1');
}

//reads the text from file and fills the frequency map
void Coder::readFromFile(const std::string& fileinName)
{
	std::ifstream filein(fileinName,std::ios::binary);
	char curr=filein.get();
	while(true)
	{
		if (freq_map.count(curr) > 0)
		{
			freq_map[curr] += 1;
		}
		else
		{
			freq_map.insert(std::pair<char, int>(curr, 1));
		}
		curr = filein.get();
		if (filein.eof())break;
		
	}
	std::cout << countSymbols() << " characters in the file\n";
	filein.close();

	tree = new HuffmanTree(freq_map);
	createCodes(tree->getRoot(), "");


	for (std::map<char, std::string>::iterator it = comp_map.begin(); it != comp_map.end(); it++)
	{
		std::cout << it->first << "-" << it->second << "\n";
	}

}
void Coder::printMap()
{
	std::cout << "Frequency table"<<std::endl;
	for (std::map<char, int>::iterator it = freq_map.begin(); it != freq_map.end(); ++it)
	{
		std::cout << it->first << " " << it->second <<" times"<< std::endl;
	}
}

//compression 
void Coder::compressWithSymbols(const std::string& fileinName, const std::string& fileoutName)
{
	std::ifstream filein(fileinName);
	std::ofstream fileout(fileoutName);

	unsigned long sizeOfCompressed = 0;
	char curr=filein.get();
	std::string binarySeq;
	while(true)
	{
		binarySeq+=comp_map[curr];//takes the binary code of the current character
		if (binarySeq.size() >= 8)
		{
			std::string tmp(binarySeq, 0, 8);//create a byte from the sequence of bits 
			fileout << (char)BinaryToDec(tmp);
			sizeOfCompressed++;
			binarySeq.erase(0, 8);
			
		}
		curr = filein.get();
		if (filein.eof())break;
	} 
	//compress Rest of the binary Sequence
	if (binarySeq.size() > 0)
	{
		while (binarySeq.length() < 8)binarySeq += '0';
		
		fileout << (char)BinaryToDec(binarySeq);
		sizeOfCompressed++;
	}
	std::cout << sizeOfCompressed<<" characters in the compressed file \n";
	double stepen = ((double)sizeOfCompressed*100 / tree->getRoot()->data.second);
	std::cout << "Compressed size / Original file size  = " << stepen << "%\n";
}

//returns the number of symbols in the start file
unsigned long Coder::countSymbols()
{
	unsigned long countSymbols = 0;
	for (std::map<char, int>::iterator it = freq_map.begin(); it != freq_map.end();++it)
	{
		countSymbols += it->second;
	}
	return countSymbols;
}


//decompression
void Coder::decompressWithSymbols(const std::string& fileinName, const std::string& fileoutName)
{
	//std::map<std::string, char> decomp_map;//use map for faster decompression
	//fillMap(decomp_map);//use the table of the huffmantree to fill the map

	unsigned long counter = 0;
	unsigned long length = tree->getRoot()->data.second;


	std::ifstream filein(fileinName,std::ios::binary);
	std::ofstream fileout(fileoutName);
	
	std::string totalBins;
	char curr = filein.get();
	while (true)
	{
		totalBins += charToBinCode(curr);
		curr = filein.get();
		if (filein.eof())break;
	}
	Node *start = tree->getRoot();
	std::cout << length<<"\n";
	while (counter<length && !totalBins.empty())
	{
		if (totalBins[0] == '0')
		{
			start = start->left;
			totalBins.erase(0, 1);
		}
		else
		{
			start = start->right;
			totalBins.erase(0, 1);
		}
		if (start && !start->left && !start->right)
		{
			fileout << start->data.first;
			start = tree->getRoot();
			counter++;
		}
	}
	std::cout << counter <<" characters created\n";
	/*std::string singleBinCode;
	char curr=filein.get();
	while (true)
	{
		totalBins += charToBinCode(curr);
		
		while (totalBins.size() > 0)
		{
			singleBinCode += totalBins[0];
			totalBins.erase(0, 1);
			char character;
			if (hasSymbol(tree->getRoot(),singleBinCode,character) && counter<length)
			{
					fileout << character;
					singleBinCode = "";
					counter++;
			}
		}
		curr = filein.get();
		if (filein.eof())break;
	}
	std::cout << std::endl<<counter <<" "<<singleBinCode<<std::endl;
	*/
}

bool Coder::hasSymbol(const Node* root, std::string binCode, char &c)
{
	if (root->left==nullptr && root->right==nullptr)
	{
		c = root->data.first;
		return true;
	}
	if (binCode.empty())
	{
		return false;
	}
		
	if (binCode[0] == '0')
	{
		binCode.erase(0, 1);
		return hasSymbol(root->left, binCode, c);
	}
	else
	{
		binCode.erase(0, 1);
		return hasSymbol(root->right, binCode, c);
	}
}

//compression with numbers
void Coder::compressWithNumbers(const std::string& fileinName, const std::string& fileoutName)
{
	std::ifstream filein(fileinName);
	std::ofstream fileout(fileoutName);
	char curr;
	std::string binSeq="";
	curr = filein.get();
	while (!filein.eof())
	{
		//std::cout << comp_map[curr];
		binSeq += comp_map[curr];
		if (binSeq.length() >= 8)
		{
			
			std::string tmp(binSeq, 0, 8);
			fileout <<BinaryToDec(tmp)<<" ";
			binSeq.erase(0, 8);
		}
		curr = filein.get();
	}
	if (binSeq.size() > 0)
	{
		while (binSeq.size() < 8)binSeq += '0';
		fileout << BinaryToDec(binSeq);//!!!
	}
}


//helper functions
std::string Coder::DecimalToBinary(unsigned long num)
{
	std::string result = "";
	while (num != 0)
	{
		if (num % 2 == 0)result = '0' + result;
		else result = '1' + result;
		num /= 2;
	}
	while (result.size() < 8)
	{
		result = '0'+result;
	}
	return result;
}
int Coder::BinaryToDec(std::string& str)
{
	int result = 0;
	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] == '1')result += pow(2, str.size() - i-1);
	}
	return result;
}
std::string Coder::charToBinCode(char curr)
{
	std::bitset<8> toBinCode(curr);
	std::string result = toBinCode.to_string();
	return result;
}

//output the binary code for each symbol in a file
void Coder::binarySequence(const std::string& fileinName, const std::string&fileoutName)
{
	std::ifstream filein(fileinName);
	std::ofstream fileout(fileoutName);

	char curr = filein.get();
	while (!filein.eof())
	{
		fileout << comp_map[curr];
		curr = filein.get();
	}
}


/*
void Coder::fillMap(std::map<std::string, char>& map)
{
	std::ifstream table("table.txt");
	std::string line;
	char character = table.get();
	getline(table, line);
	while (true)
	{
		line.erase(0, 1);
		map.insert(std::pair<std::string, char>(line, character));
		character=table.get();
		getline(table, line);
		if (table.eof())break;
	}

	for (std::map<std::string, char>::iterator it = map.begin(); it != map.end(); it++)
	{
		std::cout << std::endl << it->first << " " << it->second;
	}
}
*/


void Coder::compress(const std::string& fileinName, const std::string& fileoutName)
{
	readFromFile(fileinName);
	printMap();
	compressWithSymbols(fileinName, fileoutName);
	compressWithNumbers(fileinName, "withNumbers.txt");
	binarySequence(fileinName, "withBinary.txt");


}

void Coder::decompress(const std::string& fileinName, const std::string& fileoutName)
{
	std::ifstream fileWithTree("HuffmanTree.txt");
	tree = new HuffmanTree(fileWithTree);
	assert(tree->getRoot());
	decompressWithSymbols(fileinName, fileoutName);
	//delete tree;
}