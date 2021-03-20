#pragma once
#include <assert.h>

template <typename keyT, typename valT>
class MyDict
{
public:
	struct Node;

	// ����������� �� ���������.
	// ���������: ����� ������ ������ �������.
	MyDict()
	{

	}


	// ����������.
	// ���� � ������ ��������� ���������, �� ��������� ������ ��� ���������, �� ����������
	// �� ������� ��� ���������.
	// ���������: ����� ������� ��� ����.
	~MyDict()
	{
		clear();
	}


	// ��������.
	// ���� � ������ ��������� ���������, �� ��������� ������ ��� ���������, �� ����������
	// �� ������� ��� ���������.
	// ���������: ����� ������� ��� ���� � ������, ����� ����������.
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
			m_root->isRed = false; // ������ ������ ���� ������.
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

	// ���������� ��������� �� ���� � m_value. ���� nullptr, �� ������ ��� ������ ����.
	Node* search(Node* startNode, keyT key)
	{
		// ������� ���� - �������.
		if (startNode == nullptr || startNode->m_key == key) {
			return startNode;
		}

		return search(startNode->getChild(startNode->isKeyLeft(key)), key);
	}

	Node* searchParent(Node* startNode, keyT key, bool& isChildLeft)
	{
		isChildLeft = startNode->isKeyLeft(key);
		Node* nextNode = startNode->getChild(isChildLeft);

		// ��������� ���� - �������.
		if (nextNode == nullptr || startNode->m_key == key) {
			return startNode;
		}


		return searchParent(nextNode, key, isChildLeft);
	}


	class Node
	{
	public:
		// ������� �� ���������.
		// ������� - nullptr.
		// m_key = 0
		Node(Node* parent) : parent(parent)
		{
			m_key = {};
			m_value = {};

			isRed = true;

			left = nullptr;
			right = nullptr;
		}

		// ������� �� ���������.
		// ������� - nullptr.
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



		// ����.
		keyT m_key;

		// ���������� ���������� � ����.
		valT m_value;

		// ������� �� ����?
		bool isRed;

	private:

		// ��������� �� ��������.
		Node* parent;

		// ��������� ������ �������.
		Node* left;

		// ��������� �� ������� �������.
		Node* right;
	};

private:

	void balance(Node* inserted)
	{
		assert(inserted != m_root);

		Node* parent = inserted->getParent();

		// 1. ���� ������������ ���� ������, �� ���������.
		if (!(parent->isRed)) {
			return;
		}

		/*======� ������ ������ � ������ ��������, ��� ����������� � �������� �������=====*/

		Node* grandParent = parent->getParent();
		Node* uncle = inserted->getUncle();

		/* 2. ���� ���� �������, �� �������������
			���� � �������� � ������, � ���� � �������, ���� ��� �� ������,
			���� ������ - �� ������ ��� � ���������. */
		if (uncle != nullptr && uncle->isRed) {
			uncle->isRed = false;
			parent->isRed = false;

			// ���� ��� - ������, �� ���������.
			if (grandParent == m_root) {
				return;
			}

			// ����� ������������� ��� � �������� ������������ ��� ���� � ���������.
			grandParent->isRed = true;
			balance(grandParent);
			return;
		}


		// 3. ����� ������ �������� � ��������������.

		const bool wasParentLeft = parent->isLeft();
		const bool wasInsertedLeft = inserted->isLeft();

		// ���� ����� ������� ������ ��������.
		if (wasParentLeft != wasInsertedLeft) {
			rotate(parent, wasParentLeft);

			inserted->isRed = false; // ������� ������ ���������� ����������� �����. ��� ������ ���� ������.
		}
		else {
			parent->isRed = false; // ����� ������� ����� ������� �������. ��� ������ ���� ������.
		}

		// ������ ��������.
		rotate(grandParent, !wasParentLeft);

		// ��������������. ��� ������ ����, ������ ����� �������.
		grandParent->isRed = true;
	}


	// ���������� ����� ������� (parent, left, right).
	void rotate(Node* rotatedNode, bool isLeftRotation)
	{
		Node* parentNode = rotatedNode->getParent(); // �������� ����.
		Node* targetChildNode = rotatedNode->getChild(!isLeftRotation); // ������� ����.
		Node* lostGrandChild = targetChildNode->getChild(isLeftRotation); // ����, ������� ����� "������" ��� ��������.

		// ��������� � ����� �������� � ������.
		if (rotatedNode == m_root) {
			// ����� ������.
			targetChildNode->makeRoot(m_root);
		}
		else {
			// ������ �������� � �������� ������� ����� (�� ����� ����������).
			parentNode->setChild(targetChildNode, rotatedNode->isLeft());
		}

		// ������� ������ �������� ���������� ����.
		targetChildNode->setChild(rotatedNode, isLeftRotation);

		// ��������� ���������� "���������" ����.
		rotatedNode->setChild(lostGrandChild, !isLeftRotation);

		// DEBUG.
		parentNode->assertRelations();
		rotatedNode->assertRelations();
		targetChildNode->assertRelations();

	}

	// ������ ������.
	Node* m_root;
};
