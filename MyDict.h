#pragma once
#include <assert.h>

template <typename keyT, typename valT>
class MyDict
{
public:
	class Node
	{
	public:
		// Красный по умолчанию.
		// Потомки - nullptr.
		// m_key = 0
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
		Node(Node* parent, keyT key, valT value=0.0) : Node(parent)
		{
			m_key = key;
			m_value = value;
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
			// Для корня должно быть возвращено истина.
			// Только у корня parent == nullptr.
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


	// Деструктор.
	// Если в списке хранились указатели, то удаляются только эти указатели, не информация
	// на которую они указывают.
	// Результат: будут удалены все узлы.
	~MyDict()
	{
		clear();
	}


	// Очистить.
	// Если в списке хранились указатели, то удаляются только эти указатели, не информация
	// на которую они указывают.
	// Результат: будут удалены все узлы в списке, кроме барьерного.
	void clear()
	{

	}
	

	Node* getRootPtr()
	{
		return m_root;
	}


	void insert(keyT key, valT value=0.0)
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

	// 3 функции:
	// 1. Находим. (erase)
	// 2. Переходим к узлу без детей. (BSTDeletion)
	// 3. Удаляем его. (erase(Node*))
	void erase(keyT key)
	{
		Node* targetNode = search(m_root, key);
		if (targetNode == nullptr) return;

		erase(targetNode, key);
	}


	void erase(Node* nodeToDelete, const keyT& deletedKey)
	{
		// Необходим переход к узлу без детей.

		nodeToDelete = BSTDeletion(nodeToDelete);

		//========= В данный момент узел должен быть конечным. =========

		// 1. Удаляемый узел красный, просто убираем его.
		if (nodeToDelete->isRed == true) {
			Node* parent = nodeToDelete->getParent();
			delete nodeToDelete;
			return;
		}

		// 2. Удаляемый узел черный. Теперь он DB.
		balanceDB(nodeToDelete, deletedKey);

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

		bool operator==(const_iterator other)
		{
			return (node == other.node);
		}

		bool operator!=(const_iterator other)
		{
			return !(*this == other);
		}

		const Node* operator*()
		{
			return node;
		}

	private:

		const Node* node;
	};

	class iterator
	{
	public:
		iterator(Node* node) : node(node)
		{
		}

		iterator& operator++()
		{
			// Если есть правый ребенок: ищем приемника.
			// Иначе: поднимаемся по дереву пока подъем не будет осуществлен через левую связь.

			if (node->getChild(false) != nullptr) {
				// Ищем приемника - наименьший элемент в правом поддереве.
				// Начинаем с правого ребенка. Идем влево пока левый узел не станет nullptr.
				for (Node* curNode = node->getChild(false); ; curNode = curNode->getChild(true)) {

					// Закончили поиск.
					if (curNode->getChild(true) == nullptr) {
						node = curNode;
						return *this;
					}
				}
			}

			for (Node* curNode = node; ; curNode = curNode->getParent()) {
				// Найден узел, у которого переход к родителю будет через левую связь.
				if (curNode->isLeft()) {
					// Делаем в последний раз переход и закончили. Причем в node запись.
					node = curNode->getParent();
					return *this;
				}
			}
		}

		bool operator==(iterator other)
		{
			return (node == other.node);
		}

		bool operator!=(iterator other)
		{
			return !(*this == other);
		}

		Node* operator*()
		{
			return node;
		}

	private:

		Node* node;
	};

	// У словаря нет не констатного итератора.
	//using iterator = const_iterator;


	iterator begin()
	{
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


	// Корень дерева. isLeft() должная возвращать истину
	// для корректной работы итератора.
	Node* m_root;
};
