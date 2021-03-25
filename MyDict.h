#pragma once
#include <assert.h>

template <typename keyT, typename valT>
class MyDict
{
public:
	class const_iterator;
	// У словаря нет не констатного итератора.
	using iterator = const_iterator;

	class Node
	{
	public:
		// Красный по умолчанию.
		// Потомки - nullptr.
		Node(Node* parent) : parent(parent)
		{
			m_key = {};
			m_value = {};

			isRed = true;

			left = nullptr;
			right = nullptr;
		}

		// Красный по умолчанию.
		// Потомки - nullptr.
		Node(Node* parent, keyT key, valT value) : Node(parent)
		{
			m_key = key;
			m_value = value;
		}

		Node(const Node& other)
		{
			// Копируем значения.
			m_key = other.m_key;
			m_value = other.m_value;
			isRed = other.isRed;

			// Связи оставляем nullptr.
			parent = nullptr;
			left = nullptr;
			right = nullptr;
		}

		// Позаботиться о связи родителя.
		~Node()
		{
			assert(left == nullptr && right == nullptr);
			if (parent != nullptr) {
				// Если удаляется не корень.
				parent->setChild(nullptr, parent->isLeftChild(this));
			}
			// Если удаляется корень, заботится о связи родителя не нужно.
		}

		bool isKeyLeft(keyT key) const
		{
			return (key < m_key);
		}

		bool isLeftChild(Node* node) const
		{
			assert(node == left || node == right);
			if (node == left) {
				return true;
			}
			
			return false;
		}

		bool isLeft() const
		{
			// Только у корня parent == nullptr.
			// Для корня должно быть возвращено истина.
			if (parent == nullptr) {
				return true;
			}

			return parent->isKeyLeft(m_key);
		}


		Node* getParent()
		{
			return parent;
		}

		const Node* getParent() const
		{
			return parent;
		}

		Node* getChild(bool isLeft)
		{
			return ((isLeft) ? (left) : (right));
		}

		const Node* getChild(bool isLeft) const
		{
			return ((isLeft) ? (left) : (right));
		}

		Node* getSibling()
		{
			return parent->getChild(!isLeft());
		}

		Node* getUncle()
		{
			return parent->getSibling();
		}


		void setChild(Node* newChild, bool isNewChildLeft)
		{
			((isNewChildLeft) ? (left) : (right)) = newChild;
			if (newChild != nullptr) {
				newChild->parent = this;
			}
		}


		void makeRoot(Node*& treeRoot)
		{
			treeRoot = this;
			parent = nullptr;
			isRed = false;
		}


		void assertRelations()
		{
			if (this == nullptr) return;
			if (parent != nullptr) assert(parent->getChild(isLeft()) == this);
			if (left != nullptr) assert(left->parent == this);
			if (right != nullptr) assert(right->parent == this);
		}


		// Ключ.
		keyT m_key;

		// Информация хранящаяся в узле.
		valT m_value;

		// Красный ли узел?
		bool isRed;

	private:

		// Указатель на родителя.
		Node* parent;

		// Указатель левого потомка.
		Node* left;

		// Указатель на правого потомка.
		Node* right;
	};

	// Конструктор по умолчанию.
	// Результат: будет создан пустой словарь.
	MyDict()
	{
		m_root = nullptr;
	}

	// Конструктор копирования.
	MyDict(const MyDict& other)
	{
		// Копируем все характеристики, кроме связей.
		m_root = new Node(*(other.m_root));

		// Обрабатываем левого ребенка.
		recCopy(m_root, other.m_root, true);
		// Обрабатываем правого ребенка.
		recCopy(m_root, other.m_root, false);
	}

	// Оператор присваивания.
	MyDict& operator=(const MyDict& other)
	{
		clear();

		// Копируем все характеристики, кроме связей.
		m_root = new Node(*(other.m_root));

		// Обрабатываем левого ребенка.
		recCopy(m_root, other.m_root, true);
		// Обрабатываем правого ребенка.
		recCopy(m_root, other.m_root, false);

		return *this;
	}

	// Деструктор.
	// Если в списке хранились указатели, то удаляются только эти указатели, не информация
	// на которую они указывают.
	// Результат: будут удалены все узлы.
	~MyDict()
	{
		clear();
	}

	//Вывести как красно-черное дерево.
	void printAsRBTree()
	{
		Node* tree = m_root;

		HANDLE hConsole;
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		// should check if we don't exceed this somehow..
		char path[255] = {};

		//initial depth is 0
		draw_tree_hor2(tree, 0, path, 0, hConsole);
	}

	// Очистить.
	// Результат: будут удалены все узлы в дереве.
	void clear()
	{
		if (m_root == nullptr) {
			return;
		}

		recClean(m_root);
		m_root = nullptr;
	}
	
	void insert(keyT key, valT value)
	{
		if (m_root == nullptr) {
			m_root = new Node(nullptr, key, value);
			m_root->isRed = false; // Корень должен быть черным.
			return;
		}

		bool isInsertedLeft = false;
		Node* parentNode = searchParent(m_root, key, isInsertedLeft);

		Node* insertingNode = new Node(parentNode, key, value);
		parentNode->setChild(insertingNode, isInsertedLeft);

		balance(insertingNode);
	}

	void erase(keyT key)
	{
		// 3 функции:
		// 1. Находим. (erase)
		// 2. Переходим к узлу без детей. (BSTDeletion)
		// 3. Удаляем его. (erase(Node*))
		Node* targetNode = search(m_root, key);
		if (targetNode == nullptr) return;

		erase(targetNode, key);
	}

	iterator find(keyT key)
	{
		Node* node = search(m_root, key);
		return iterator(node);
	}

	const valT& operator[](keyT key)
	{
		iterator it = find(key);
		assert(it != end());

		return it.node->m_value;
	}

	// Возвращает указатель на узел с m_value. Если nullptr, то значит нет такого узла.
	Node* search(Node* startNode, keyT key)
	{
		// Текущий узел - искомый.
		if (startNode == nullptr || startNode->m_key == key) {
			return startNode;
		}

		return search( startNode->getChild(startNode->isKeyLeft(key)), key);
	}


	class const_iterator
	{
	public:
		const_iterator(Node* node) : node(node)
		{
		}

		const_iterator& operator++()
		{
			// Если есть правый ребенок: ищем приемника.
			// Иначе: поднимаемся по дереву пока подъем не будет осуществлен через левую связь.

			if (node->getChild(false) != nullptr) {
				// Ищем приемника - наименьший элемент в правом поддереве.
				// Начинаем с правого ребенка. Идем влево пока левый узел не станет nullptr.
				for (const Node* curNode = node->getChild(false); ; curNode = curNode->getChild(true)) {

					// Закончили поиск.
					if (curNode->getChild(true) == nullptr) {
						node = curNode;
						return *this;
					}
				}
			}

			for (const Node* curNode = node; ; curNode = curNode->getParent()) {
				// Найден узел, у которого переход к родителю будет через левую связь.
				if (curNode->isLeft()) {
					// Делаем в последний раз переход и закончили. Причем в node запись.
					node = curNode->getParent();
					return *this;
				}
			}
		}

		const_iterator& operator--()
		{
			// Если есть левый ребенок: ищем предшественника.
			// Иначе: поднимаемся по дереву пока подъем не будет осуществлен через правую связь.

			if (node->getChild(false) != nullptr) {
				// Ищем предшественника - наибольший элемент в левом поддереве.
				// Начинаем с левого ребенка. Идем право пока правый узел не станет nullptr.
				for (const Node* curNode = node->getChild(true); ; curNode = curNode->getChild(false)) {

					// Закончили поиск.
					if (curNode->getChild(false) == nullptr) {
						node = curNode;
						return *this;
					}
				}
			}

			for (const Node* curNode = node; ; curNode = curNode->getParent()) {
				// Найден узел, у которого переход к родителю будет через правую связь.
				if ( !(curNode->isLeft()) ) {
					// Делаем в последний раз переход и закончили. Причем в node запись.
					node = curNode->getParent();
					return *this;
				}
			}
		}

		bool operator==(const_iterator other)
		{
			return (node == other.node);
		}

		bool operator!=(const_iterator other)
		{
			return !(*this == other);
		}

		std::pair<keyT, valT> operator*()
		{
			return std::pair<keyT, valT>(node->m_key, node->m_value);
		}

	private:
		friend class MyDict;
		const Node* node;
	};

	//class iterator
	//{
	//public:
	//	iterator(Node* node) : node(node)
	//	{
	//	}
	//
	//	iterator& operator++()
	//	{
	//		// Если есть правый ребенок: ищем приемника.
	//		// Иначе: поднимаемся по дереву пока подъем не будет осуществлен через левую связь.
	//
	//		if (node->getChild(false) != nullptr) {
	//			// Ищем приемника - наименьший элемент в правом поддереве.
	//			// Начинаем с правого ребенка. Идем влево пока левый узел не станет nullptr.
	//			for (Node* curNode = node->getChild(false); ; curNode = curNode->getChild(true)) {
	//
	//				// Закончили поиск.
	//				if (curNode->getChild(true) == nullptr) {
	//					node = curNode;
	//					return *this;
	//				}
	//			}
	//		}
	//
	//		for (Node* curNode = node; ; curNode = curNode->getParent()) {
	//			// Найден узел, у которого переход к родителю будет через левую связь.
	//			if (curNode->isLeft()) {
	//				// Делаем в последний раз переход и закончили. Причем в node запись.
	//				node = curNode->getParent();
	//				return *this;
	//			}
	//		}
	//	}
	//
	//	bool operator==(iterator other)
	//	{
	//		return (node == other.node);
	//	}
	//
	//	bool operator!=(iterator other)
	//	{
	//		return !(*this == other);
	//	}
	//
	//	Node* operator*()
	//	{
	//		return node;
	//	}
	//
	//private:
	//
	//	Node* node;
	//};

	iterator begin()
	{
		if (m_root == nullptr)
		{
			iterator it(nullptr);
			return it;
		}

		// Двигаемся влево пока левый ребенок не nullptr.
		for (Node* curNode = m_root; ; curNode = curNode->getChild(true)) {
			// Закончили поиск.
			if (curNode->getChild(true) == nullptr) {
				iterator it(curNode);
				return it;
			}
		}
	}


	iterator end()
	{
		iterator it(nullptr);
		return it;
	}

private:

	//Для printAsRBTree.
	void draw_tree_hor2(MyDict<keyT, valT>::Node* tree, int depth, char* path, int right, HANDLE hConsole)
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
		std::cout << tree->m_key << std::endl;
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

	void recClean(Node* root) {
		if (root->getChild(true) != nullptr) recClean(root->getChild(true));
		if (root->getChild(false) != nullptr) recClean(root->getChild(false));
		delete root;
	}

	void recCopy(Node* parent, const Node* otherParent, bool isLeftChild)
	{
		// ==== Копируем ребенка. ====
		const Node* otherChild = otherParent->getChild(isLeftChild);

		// Если ребенок nullptr, то закончили.
		if (otherChild == nullptr) return;

		// Копируем все характеристики, кроме связей.
		Node* child = new Node(*otherChild);
		parent->setChild(child, isLeftChild);
		// ==================================

		// Обрабатываем левого ребенка. 
		recCopy(child, otherChild, true);

		// Обрабатываем правого ребенка.
		recCopy(child, otherChild, false);
	}

	void erase(Node* nodeToDelete, const keyT& deletedKey)
	{
		// Необходим переход к узлу без детей.

		nodeToDelete = BSTDeletion(nodeToDelete);

		//========= В данный момент узел должен быть конечным. =========

		// 1. Удаляемый узел красный, просто убираем его.
		if (nodeToDelete->isRed == true) {
			delete nodeToDelete;
			return;
		}

		// 2. Удаляемый узел черный. Теперь он DB.
		balanceDB(nodeToDelete, deletedKey);

	}

	Node* searchParent(Node* startNode, keyT key, bool& isChildLeft)
	{
		isChildLeft = startNode->isKeyLeft(key);
		Node* nextNode = startNode->getChild(isChildLeft);

		// Следующий узел - искомый.
		if (nextNode == nullptr || startNode->m_key == key) {
			return startNode;
		}


		return searchParent(nextNode, key, isChildLeft);
	}

	void balance(Node* inserted)
	{
		assert(inserted != m_root);

		Node* parent = inserted->getParent();

		// 1. Если родительский узел черный, то закончили.
		if ( !(parent->isRed) ) {
			return;
		}

		// ====== В данный момент вставленный и родитель красные. =====

		Node* grandParent = parent->getParent();
		Node* uncle = inserted->getUncle();
		
		// 2. Если дядя красный, то перекрашиваем
		//	дядю и родителя в черный, а деда в красный, если дед не корень,
		//	если корень - не красим его и закончили.
		if (uncle != nullptr && uncle->isRed) {
			uncle->isRed = false;
			parent->isRed = false;

			// если дед - корень, то закончили.
			if (grandParent == m_root) {
				return;
			}

			// иначе перекрашиваем его и проводим балансировку для него и закончили.
			grandParent->isRed = true;
			balance(grandParent);
			return;
		}


		// 3. Нужно делать вращения и перекрашивания.

		const bool wasParentLeft = parent->isLeft();
		const bool wasInsertedLeft = inserted->isLeft();

		// если нужно сделать первое вращение.
		if (wasParentLeft != wasInsertedLeft) {
			rotate(parent, wasParentLeft);

			inserted->isRed = false; // Средним звеном становится вставленное звено. Оно должно быть черным.
		}
		else {
			parent->isRed = false; // Иначе среднее звено остатся средним. Оно должно быть черным.
		}

		// второе вращение.
		rotate(grandParent, !wasParentLeft);

		// Перекрашивание. Дед уходит вниз, должен стать красным.
		grandParent->isRed = true;
	}

	Node* BSTDeletion(Node* nodeToDelete) 
	{
		// Переход к узлу без детей.
		// Никаких удалений, только замены.

		const bool haveLeft = (nodeToDelete->getChild(true) != nullptr);
		const bool haveRight = (nodeToDelete->getChild(false) != nullptr);

		// 1. Узел не имеет детей.
		if (!haveLeft && !haveRight) {
			return nodeToDelete;
		}


		// 2. У узла два ребенка.
		if (haveLeft && haveRight) {

			// Ищем приемника - наименьший элемент в правом поддереве.
			// Начинаем с правого ребенка. Идем влево пока левый узел не станет nullptr.
			for (Node* curNode = nodeToDelete->getChild(false); ; curNode = curNode->getChild(true)) {

				// Закончили поиск.
				if (curNode->getChild(true) == nullptr) {
					// Заменяем удаляемый узел на узел приемник.
					nodeToDelete->m_key = curNode->m_key;
					nodeToDelete->m_value = curNode->m_value;

					// Теперь мы удаляем узел приемник.
					return BSTDeletion(curNode);
				}
			}
		}

		// 3. Узел имеет одного ребенка. Причем haveLeft показывает какой.

		// Заменяем удаляемый узел на единственного ребенка.
		Node* children = nodeToDelete->getChild(haveLeft);
		nodeToDelete->m_key = children->m_key;
		nodeToDelete->m_value = children->m_value;

		return BSTDeletion(children);
	}

	void balanceDB(Node* DB, const keyT& deletedKey) 
	{
		// 2.1. DB является корнем. Просто забываем, что это DB.
		if (DB == m_root) {
			// Если удаляемый ключ - корень и он стал DB, значит удаляется 
			// корень - единственный оставшийся элемент.
			if (DB->m_key == deletedKey) {
				delete DB;
				m_root = nullptr;
			}
			return;
		}

		Node* parent = DB->getParent();
		Node* sibling = DB->getSibling();

		// 2.2. Брат узла черный и дети брата тоже черные или nullptr.
		if ((sibling->isRed == false) &&
			((sibling->getChild(true) == nullptr) || (sibling->getChild(true)->isRed == false)) &&
			((sibling->getChild(false) == nullptr) || (sibling->getChild(false)->isRed == false))
			)
		{
			// Удаляем узел, который был DB.
			// Или забываем что это DB, если это не удаляемый ключ.
			if (DB->m_key == deletedKey) {
				delete DB;
			}
			DB = nullptr;

			if (parent->isRed == false) {
				// Если родитель был черным, то он становится DB.
				DB = parent;
			}
			else {
				// Если родитель был красным, то он становится черным.
				parent->isRed = false;
			}

			// Брат становится красным.
			sibling->isRed = true;

			// Если родитель был красным, то закончили.
			if (DB == nullptr) return;

			// Иначе балансируем новый DB - родителя.
			return balanceDB(DB, deletedKey);
		}


		// 2.3. Брат узла красный.
		if (sibling->isRed == true) {
			// Брат становится черным.
			sibling->isRed = false;
			// Родитель становится красным.
			parent->isRed = true;

			// Вращаем родителя в сторону DB.
			rotate(parent, DB->isLeft());
			// Проводим балансировку DB снова.
			return balanceDB(DB, deletedKey);
		}


		// 2.4. Брат узла черный, но ближний ребенок красный, а дальний ребенок черный.
		bool wasCase4 = false;
		Node* closeSiblingChild = sibling->getChild(parent->isLeftChild(DB));
		Node* farSiblingChild = sibling->getChild( !(parent->isLeftChild(DB)) );
		if ( (sibling->isRed == false) &&
			( (closeSiblingChild != nullptr) && (closeSiblingChild->isRed == true) ) &&
			( (farSiblingChild == nullptr) || (farSiblingChild->isRed == false) )
			)
		{
			wasCase4 = true;
			// Ближний ребенок становится черным.
			closeSiblingChild->isRed = false;
			// Брат становится красным.
			sibling->isRed = true;

			// Вращение в сторону от DB.
			rotate(sibling, !(parent->isLeftChild(DB)) );
		}


		if (wasCase4) {
			sibling = DB->getSibling();
			farSiblingChild = sibling->getChild( !(parent->isLeftChild(DB)) );
		}

		// 2.5. Брат узла черный, но дальний ребенок красный.
		if ((sibling->isRed == false) && 
			( (farSiblingChild != nullptr) && (farSiblingChild->isRed == true) )
			)
		{
			// Родитель и брат меняются цветами.
			std::swap(parent->isRed, sibling->isRed);
			// Вращаем родителя в сторону DB.
			rotate(parent, parent->isLeftChild(DB));
			// DB отдает черный цвет дальнему ребенку, который красный.
			farSiblingChild->isRed = false;

			// Удаляем узел, который был DB.
			// Или забываем что это DB, если это не удаляемый ключ.
			if (DB->m_key == deletedKey) {
				delete DB;
			}
		}
	}

	// Занимается всеми связями (parent, left, right).
	void rotate(Node* rotatedNode, bool isLeftRotation) 
	{
		Node* parentNode = rotatedNode->getParent(); // Родитель узла.
		Node* targetChildNode = rotatedNode->getChild(!isLeftRotation); // Ребенок узла.
		Node* lostGrandChild = targetChildNode->getChild(isLeftRotation); // Узел, который будет "утерян" при повороте.

		// Заботимся о связи родителя с внуком.
		if (rotatedNode == m_root) {
			// Новый корень.
			targetChildNode->makeRoot(m_root);
		}
		else {
			// Ставим родителю в качестве ребенка внука (на место вращаемого).
			parentNode->setChild(targetChildNode, rotatedNode->isLeft());
		}

		// Ребенок теперь родитель вращаемого узла.
		targetChildNode->setChild(rotatedNode, isLeftRotation);

		// Назначаем вращаемому "утерянный" узел.
		rotatedNode->setChild(lostGrandChild, !isLeftRotation);

		// DEBUG.
		parentNode->assertRelations();
		rotatedNode->assertRelations();
		targetChildNode->assertRelations();
	}

	// Корень дерева.
	Node* m_root;
};
