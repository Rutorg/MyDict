#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <windows.h>
#include <random>

#include "MyDict.h"


using std::cout;
using std::cin;
using std::endl;


int _print_t(MyDict<int, double>::Node* tree, int is_left, int offset, int depth, char s[20][255])
{
	char b[20];
	int width = 5;

	if (!tree) return 0;

	// Если цвет красный, то ставим его красным.
	if (tree->isRed) {
		sprintf(b, "(%03d)", tree->m_key);
	}
	else {
		sprintf(b, "(%03d)", tree->m_key);
	}
	

	int left = _print_t(tree->getChild(true), 1, offset, depth + 1, s);
	int right = _print_t(tree->getChild(false), 0, offset + left + width, depth + 1, s);

#ifdef COMPACT
	for (int i = 0; i < width; i++)
		s[depth][offset + left + i] = b[i];

	if (depth && is_left) {

		for (int i = 0; i < width + right; i++)
			s[depth - 1][offset + left + width / 2 + i] = '-';

		s[depth - 1][offset + left + width / 2] = '.';

	}
	else if (depth && !is_left) {

		for (int i = 0; i < left + width; i++)
			s[depth - 1][offset - width / 2 + i] = '-';

		s[depth - 1][offset + left + width / 2] = '.';
	}
#else
	for (int i = 0; i < width; i++)
		s[2 * depth][offset + left + i] = b[i];

	if (depth && is_left) {

		for (int i = 0; i < width + right; i++)
			s[2 * depth - 1][offset + left + width / 2 + i] = '-';

		s[2 * depth - 1][offset + left + width / 2] = '+';
		s[2 * depth - 1][offset + left + width + right + width / 2] = '+';

	}
	else if (depth && !is_left) {

		for (int i = 0; i < left + width; i++)
			s[2 * depth - 1][offset - width / 2 + i] = '-';

		s[2 * depth - 1][offset + left + width / 2] = '+';
		s[2 * depth - 1][offset - width / 2 - 1] = '+';
	}
#endif

	return left + width + right;
}

void print_t(MyDict<int, double>::Node* tree)
{
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	char s[20][255];
	for (int i = 0; i < 20; i++)
		sprintf(s[i], "%80s", " ");

	_print_t(tree, 0, 0, 0, s);

	//SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
	for (int i = 0; i < 20; i++)
		printf("%s\n", s[i]);

}


void test1()
{
	srand(static_cast<unsigned int>(time(0)));
	MyDict<int, double> tree;

	std::vector<int> vec(20);
	for (size_t i = 0; i < vec.size(); i++) {
		vec[i] = i;
	}
	std::random_shuffle(vec.begin(), vec.end());

	for (auto& element : vec) {
		cout << element << ' ';
		tree.insert(element, 0.0);
	}
	cout << endl;

	print_t(tree.getRootPtr());
	cout << endl;
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



	print_t(tree.getRootPtr());
}


int main()
{
	//test1();
	test2();

	return 0;
}
