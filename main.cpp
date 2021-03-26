#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <windows.h>
#include <random>

#include "MyDict.h"
#include <map>


using std::cout;
using std::cin;
using std::endl;

// Прошлые тесты.

/*void test1()
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
		tree.insert(element, 0.0);
	}
	cout << endl;

	printAsRBTree(tree.getRootPtr());
	cout << "\n\n\n\n";

	tree.erase(10);

	printAsRBTree(tree.getRootPtr());
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



	printAsRBTree(tree.getRootPtr());
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

	tree.insert(100, 0.0);
	tree.insert(25, 0.0);
	tree.insert(200, 0.0);
	tree.insert(150, 0.0);

	printAsRBTree(tree.getRootPtr());
	cout << "\n\n\n\n";


	tree.erase(150);
	printAsRBTree(tree.getRootPtr());
	cout << "\n\n\n\n";

	tree.erase(100);
	printAsRBTree(tree.getRootPtr());
}

// Конструируем деревья самостоятельно.
void test4()
{
	using dict_t = MyDict<int, double>;

	dict_t dict;
	dict.insert(10, 0.0);
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
		tree.insert(element, 0.0);
	}
	cout << seed << "\n\n";
	printAsRBTree(tree.getRootPtr());
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
		tree.insert(element, 0.0);
	}

	printAsRBTree(tree.getRootPtr());
	cout << "\n\n\n\n";

	tree.erase(13);

	printAsRBTree(tree.getRootPtr());
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

	printAsRBTree(dict1.getRootPtr());
	cout << "\n\n\n\n";

	//for (auto it = dict1.begin(); it != dict1.end(); ++it) {
	//	cout << (*it)->m_key << ' ';
	//}
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

	printAsRBTree(dict1.getRootPtr());
	cout << "\n\n\n\n";

	std::vector<int> values;
	//for (auto it = dict1.begin(); it != dict1.end(); ++it) {
	//	values.push_back((*it)->m_key);
	//	cout << (*it)->m_key << ' ';
	//}
	cout << "\n\n\n\n";

	for (auto& element : values) {
		cout << "Deleted: " << element << "\n\n";
		dict1.erase(element);
		printAsRBTree(dict1.getRootPtr());
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
		dict1.insert(element, 0.0);
	}

	printAsRBTree(dict1.getRootPtr());
	cout << "\n\n\n\n";

	std::vector<int> values;
	//for (auto it = dict1.begin(); it != dict1.end(); ++it) {
	//	values.push_back((*it)->m_key);
	//	cout << (*it)->m_key << ' ';
	//}
	cout << "\n\n\n\n";

	for (auto& element : values) {
		cout << "Deleted: " << element << "\n\n";
		dict1.erase(element);
		printAsRBTree(dict1.getRootPtr());
		cout << "\n\n";
	}

	cout << endl;
}

void iteratorTestmm()
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

	printAsRBTree(dict1.getRootPtr());
	cout << "\n\n\n\n";

}*/

// Тестирование insert.
void test10()
{
	MyDict<std::string, std::string> engRusDict;
	engRusDict.insert("why", "почему");
	engRusDict.insert("pair", "пара");
	engRusDict.insert("pen", "ручка");
	engRusDict.insert("keyboard", "клавиатура");
	engRusDict.insert("mouse", "мышь");
	engRusDict.insert("letter", "буква");
	engRusDict.insert("sentence", "предложение");
	engRusDict.insert("camera", "камера");
	engRusDict.insert("tree", "дерево");
	engRusDict.insert("road", "дорога");
	engRusDict.insert("human", "человек");

	for (auto elem : engRusDict) {
		cout << elem.first << " : " << elem.second << endl;
	}
	cout << "\n\n";

	engRusDict.printAsRBTree();
}

// Тестирования удаления.
void testDel()
{
	MyDict<std::string, std::string> engRusDict;
	engRusDict.insert("why", "почему");
	engRusDict.insert("pair", "пара");
	engRusDict.insert("pen", "ручка");
	engRusDict.insert("keyboard", "клавиатура");
	engRusDict.insert("mouse", "мышь");
	engRusDict.insert("letter", "буква");
	engRusDict.insert("sentence", "предложение");
	engRusDict.insert("camera", "камера");
	engRusDict.insert("tree", "дерево");
	engRusDict.insert("road", "дорога");
	engRusDict.insert("human", "человек");

	engRusDict.erase("tree");

	for (auto elem : engRusDict) {
		cout << elem.first << " : " << elem.second << endl;
	}
	cout << "\n\n";

	engRusDict.printAsRBTree();
	cout << "\n\n";

	engRusDict.erase("sentence");
	engRusDict.printAsRBTree();
}

// Тестирование оператора доступа по индексу.
void test12()
{
	MyDict<std::string, std::string> dict;
	dict.insert("apple", "яблоко");
	dict.insert("fruit", "фрукт");
	dict.insert("orange", "апельсин");

	std::string word = "apple";
	cout << word << " = " << dict[word] << endl;
}

// Тестирование конструктора копирования и clear.
void test13()
{
	MyDict<int, double> dict;

	dict.insert(10, 0.0);
	dict.insert(18, 0.0);
	dict.insert(7, 0.0);
	dict.insert(15, 0.0);
	dict.insert(16, 0.0);
	dict.insert(30, 0.0);
	dict.insert(25, 0.0);
	dict.insert(40, 0.0);
	dict.insert(60, 0.0);
	dict.insert(2, 0.0);
	dict.insert(1, 0.0);
	dict.insert(70, 0.0);

	cout << "dict:" << endl;
	dict.printAsRBTree();
	cout << "\n\n";

	MyDict<int, double> dictCopy = dict;
	dictCopy.insert(10, 0);
	dictCopy.insert(200, 0);
	dictCopy.insert(-100, 0);
	cout << "dictCopy:" << endl;
	dictCopy.printAsRBTree();
	cout << "\n\n";

	dict.clear();
	cout << "dict:" << endl;
	dict.printAsRBTree();
	cout << "\n\n";

	cout << "dictCopy:" << endl;
	dictCopy.printAsRBTree();
	cout << "\n\n";
}

// Тестирование оператора присваивания.
void test14()
{
	MyDict<int, double> dict;

	dict.insert(10, 0.0);
	dict.insert(18, 0.0);
	dict.insert(7, 0.0);
	dict.insert(15, 0.0);
	dict.insert(16, 0.0);
	dict.insert(30, 0.0);
	dict.insert(25, 0.0);
	dict.insert(40, 0.0);
	dict.insert(60, 0.0);
	dict.insert(2, 0.0);
	dict.insert(1, 0.0);
	dict.insert(70, 0.0);

	cout << "dict:" << endl;
	dict.printAsRBTree();
	cout << "\n\n";

	MyDict<int, double> dictCopy;
	dictCopy.insert(10, 0);
	dictCopy.insert(200, 0);
	dictCopy.insert(-100, 0);
	cout << "dictCopy(not yet):" << endl;
	dictCopy.printAsRBTree();
	cout << "\n\n";

	dictCopy = dict;

	dict.clear();
	cout << "dict:" << endl;
	dict.printAsRBTree();
	cout << "\n\n";

	cout << "dictCopy:" << endl;
	dictCopy.printAsRBTree();
	cout << "\n\n";
}



int main()
{
	/*//test1();
	//test2();
	//test3();
	//showTrees();

	//showTree(1000);

	//test5(1000);

	//iteratorTest();

	//deleteDebug();
	//deleteDebug1(1000);

	//iteratorTestmm();*/


	//test10();
	testDel();
	//test12();
	//test13();
	//test14();

	return 0;
}
