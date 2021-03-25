#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <windows.h>
#include <random>

#include "MyDict.h"


using std::cout;
using std::cin;
using std::endl;

//secondary function
void draw_tree_hor2(MyDict<int, double>::Node* tree, int depth, char* path, int right, HANDLE hConsole)
{
	const int space = 2;

	// stopping condition
	if (tree == nullptr)
		return;

	// increase spacing
	depth++;

	// start with right node
	draw_tree_hor2(tree->getChild(false), depth, path, 1, hConsole);

	if (depth > 1)
	{
		// set | draw map
		path[depth - 2] = 0;

		if (right)
			path[depth - 2] = 1;
	}

	if (tree->getChild(true))
		path[depth - 1] = 1;


	// print root after spacing
	printf("\n");

	for (int i = 0; i < depth - 1; i++)
	{
		if (i == depth - 2)
			printf("+");
		else if (path[i])
			printf("|");
		else
			printf(" ");

		for (int j = 1; j < space; j++)
			if (i < depth - 2)
				printf(" ");
			else
				printf("-");
	}

	// Если цвет красный, то ставим его красным.
	if (tree->isRed) {
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
	}
	std::cout << tree->m_key << endl;
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	// vertical spacers below
	for (int i = 0; i < depth; i++)
	{
		if (path[i])
			printf("|");
		else
			printf(" ");

		for (int j = 1; j < space; j++)
			printf(" ");
	}

	// go to left node
	draw_tree_hor2(tree->getChild(true), depth, path, 0, hConsole);
}
//primary fuction
void draw_tree_hor(MyDict<int, double>::Node* tree)
{
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// should check if we don't exceed this somehow..
	char path[255] = {};

	//initial depth is 0
	draw_tree_hor2(tree, 0, path, 0, hConsole);
}

void test1()
{
	//static_cast<unsigned int>(time(0))
	srand(200);
	MyDict<int, double> tree;

	std::vector<int> vec(20);
	for (size_t i = 0; i < vec.size(); i++) {
		vec[i] = i;
	}
	std::random_shuffle(vec.begin(), vec.end());

	for (auto& element : vec) {
		cout << element << ' ';
		tree.insert(element);
	}
	cout << endl;

	draw_tree_hor(tree.getRootPtr());
	cout << "\n\n\n\n";

	tree.erase(10);

	draw_tree_hor(tree.getRootPtr());
	cout << "\n\n\n\n";

}

void test2()
{
	MyDict<int, double> tree;

	tree.insert(10, 0.0);
	tree.insert(18, 0.0);
	tree.insert(7, 0.0);
	tree.insert(15, 0.0);
	tree.insert(16, 0.0);
	tree.insert(30, 0.0);
	tree.insert(25, 0.0);
	tree.insert(40, 0.0);
	tree.insert(60, 0.0);
	tree.insert(2, 0.0);
	tree.insert(1, 0.0);
	tree.insert(70, 0.0);



	draw_tree_hor(tree.getRootPtr());
}

void test3()
{
	MyDict<int, double> tree;

	//tree.insert(10, 0.0);
	//tree.insert(18, 0.0);
	//tree.insert(7, 0.0);
	//tree.insert(15, 0.0);
	//tree.insert(16, 0.0);
	//tree.insert(30, 0.0);
	//tree.insert(25, 0.0);
	//tree.insert(40, 0.0);
	//tree.insert(60, 0.0);
	//tree.insert(2, 0.0);
	//tree.insert(1, 0.0);
	//tree.insert(70, 0.0);

	// Много кратное BSTDeletion.
	//tree.insert(100);
	//tree.insert(50);
	//tree.insert(150);
	//tree.insert(125);
	//tree.insert(175);
	//tree.insert(142);

	tree.insert(100);
	tree.insert(25);
	tree.insert(200);
	tree.insert(150);

	draw_tree_hor(tree.getRootPtr());
	cout << "\n\n\n\n";


	tree.erase(150);
	draw_tree_hor(tree.getRootPtr());
	cout << "\n\n\n\n";

	tree.erase(100);
	draw_tree_hor(tree.getRootPtr());
}

// Конструируем деревья самостоятельно.
void test4()
{
	using dict_t = MyDict<int, double>;

	dict_t dict;
	dict.insert(10);
	dict_t::Node* root = dict.getRootPtr();
	dict_t::Node* curNode = new dict_t::Node(root, 5);
	root->setChild(curNode, true);


	curNode = new dict_t::Node(root, 1);
	root->setChild(curNode, false);

	curNode = new dict_t::Node(root, 20);
	root->setChild(curNode, false);

	curNode = new dict_t::Node(root, 20);
	root->setChild(curNode, false);

	curNode = new dict_t::Node(root, 20);
	root->setChild(curNode, false);

	curNode = new dict_t::Node(root, 20);
	root->setChild(curNode, false);
}


void showTree(int seed)
{
	srand(seed);
	MyDict<int, double> tree;

	std::vector<int> vec(20);
	for (size_t i = 0; i < vec.size(); i++) {
		vec[i] = i;
	}
	std::random_shuffle(vec.begin(), vec.end());

	for (auto& element : vec) {
		tree.insert(element);
	}
	cout << seed << "\n\n";
	draw_tree_hor(tree.getRootPtr());
	cout << "\n\n";
}

void showTrees()
{
	const int bias = 1000;
	for (int seed = 0; seed < 5; seed++) {
		showTree(seed + bias);
		cout << endl;
	}
}

void test5(int seed)
{
	//static_cast<unsigned int>(time(0))
	srand(seed);
	MyDict<int, double> tree;

	std::vector<int> vec(20);
	for (size_t i = 0; i < vec.size(); i++) {
		vec[i] = i;
	}
	std::random_shuffle(vec.begin(), vec.end());

	for (auto& element : vec) {
		tree.insert(element);
	}

	draw_tree_hor(tree.getRootPtr());
	cout << "\n\n\n\n";

	tree.erase(13);

	draw_tree_hor(tree.getRootPtr());
	cout << "\n\n\n\n";

}

void iteratorTest()
{
	MyDict<int, double> dict1;
	dict1.insert(10, 0.0);
	dict1.insert(18, 0.0);
	dict1.insert(7, 0.0);
	dict1.insert(15, 0.0);
	dict1.insert(16, 0.0);
	dict1.insert(30, 0.0);
	dict1.insert(25, 0.0);
	dict1.insert(40, 0.0);
	dict1.insert(60, 0.0);
	dict1.insert(2, 0.0);
	dict1.insert(1, 0.0);
	dict1.insert(70, 0.0);

	draw_tree_hor(dict1.getRootPtr());
	cout << "\n\n\n\n";

	for (auto it = dict1.begin(); it != dict1.end(); ++it) {
		cout << (*it)->m_key << ' ';
	}
	cout << endl;

}

void deleteDebug()
{
	using dict_t = MyDict<int, double>;
	dict_t dict1;
	dict1.insert(10, 0.0);
	dict1.insert(18, 0.0);
	dict1.insert(7, 0.0);
	dict1.insert(15, 0.0);
	dict1.insert(16, 0.0);
	dict1.insert(30, 0.0);
	dict1.insert(25, 0.0);
	dict1.insert(40, 0.0);
	dict1.insert(60, 0.0);
	dict1.insert(2, 0.0);
	dict1.insert(1, 0.0);
	dict1.insert(70, 0.0);

	draw_tree_hor(dict1.getRootPtr());
	cout << "\n\n\n\n";

	std::vector<int> values;
	for (auto it = dict1.begin(); it != dict1.end(); ++it) {
		values.push_back((*it)->m_key);
		cout << (*it)->m_key << ' ';
	}
	cout << "\n\n\n\n";

	for (auto& element : values) {
		cout << "Deleted: " << element << "\n\n";
		dict1.erase(element);
		draw_tree_hor(dict1.getRootPtr());
		cout << "\n\n";
	}

	cout << endl;
}

void deleteDebug1(int seed)
{
	srand(seed);
	using dict_t = MyDict<int, double>;
	dict_t dict1;
	std::vector<int> vec(20);
	for (size_t i = 0; i < vec.size(); i++) {
		vec[i] = i;
	}
	std::random_shuffle(vec.begin(), vec.end());

	for (auto& element : vec) {
		dict1.insert(element);
	}

	draw_tree_hor(dict1.getRootPtr());
	cout << "\n\n\n\n";

	std::vector<int> values;
	for (auto it = dict1.begin(); it != dict1.end(); ++it) {
		values.push_back((*it)->m_key);
		cout << (*it)->m_key << ' ';
	}
	cout << "\n\n\n\n";

	for (auto& element : values) {
		cout << "Deleted: " << element << "\n\n";
		dict1.erase(element);
		draw_tree_hor(dict1.getRootPtr());
		cout << "\n\n";
	}

	cout << endl;
}

int main()
{
	//test1();
	//test2();
	//test3();
	//showTrees();

	//showTree(1000);

	//test5(1000);

	//iteratorTest();

	//deleteDebug();
	//deleteDebug1(1000);

	return 0;
}
