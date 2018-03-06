#pragma once
#include<iostream>
#include<map>
#include<vector>
#include<fstream>


struct Node
{
private:
	static int maxid;
	int id;
public:
	std::pair<char, int> data;
	Node* left;
	Node* right;
	std::string binCode;
	Node(std::pair<char,int> _data,Node *_left,Node *_right):data(_data),left(_left),right(_right)
	{
		id = maxid;
		maxid++;
	}
	int getid() { return id; } 
};

class HuffmanTree
{
public:
	HuffmanTree(std::map<char, int>&);//constructor for compression(takes frequency map)
	HuffmanTree(std::istream&);//constructor for decompression(takes the file with the tree)
	Node* getRoot()const
	{
		return root;
	}
	~HuffmanTree();
private:
	Node* root;
	std::vector<Node *> nodes;

	void sortSymbols(std::map<char,int>&);//sort the symbols by number of occurences
	void buildTree();//helper function
	Node* buildTree(std::istream&);//helper function
	void printNodesLISP(std::ostream&, Node* root);//save the tree in a file
	void printDotty(std::ostream&, Node* root);//creates a dottyfile with the tree
	void deleteTree(Node*);
	

};

class Coder
{
public:
	Coder() {};
	void compress(const std::string&, const std::string&);
	void printMap();
	void decompress(const std::string&,const std::string&);
	
	~Coder()
	{
		delete tree;
	}

private:
	HuffmanTree *tree;//pointer to a HuffmanTree
	std::map<char,int> freq_map;//frequency map
	std::map<char, std::string>comp_map;//map helping to compress faster
	
	void readFromFile(const std::string&);
	void compressWithSymbols(const std::string&, const std::string&);
	void compressWithNumbers(const std::string&, const std::string&);
	void binarySequence(const std::string&, const std::string&);

	//helper functions
	int BinaryToDec(std::string&);
	std::string charToBinCode(char);
	std::string DecimalToBinary(unsigned long);	
	void createCodes(Node*, std::string);
	bool hasSymbol(const Node*, std::string, char&);
	unsigned long countSymbols();
	void printBinaryCodes();
	void decompressWithSymbols(const std::string&, const std::string&);
	
	//void fillMap(std::map<std::string, char>&);
};