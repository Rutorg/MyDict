#pragma once
#include <assert.h>

template <typename keyT, typename valT>
class MyDict
{
public:
	struct Node;

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


	void erase(int m_value)
	{

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
		Node(Node* parent, keyT key, valT value) : Node(parent)
		{
			m_key = key;
			m_value = value;
		}

		bool isKeyLeft(keyT key)
		{
			return (key < m_key);
		}

		bool isLeft()
		{
			return parent->isKeyLeft(m_key);
		}


		Node* getParent()
		{
			return parent;
		}

		Node* getChild(bool isLeft)
		{
			return ( (isLeft) ? (left) : (right) );
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
			( (isNewChildLeft) ? (left) : (right) ) = newChild;
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

private:

	void balance(Node* inserted)
	{
		assert(inserted != m_root);

		Node* parent = inserted->getParent();

		// 1. Если родительский узел черный, то закончили.
		if ( !(parent->isRed) ) {
			return;
		}

		/*======В данный момент и дальше известно, что вставленный и родитель красные=====*/

		Node* grandParent = parent->getParent();
		Node* uncle = inserted->getUncle();
		
		/* 2. Если дядя красный, то перекрашиваем
			дядю и родителя в черный, а деда в красный, если дед не корень,
			если корень - не красим его и закончили. */
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
