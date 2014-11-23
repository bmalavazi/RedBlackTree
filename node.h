#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

template <class Item>
class Node
{
public:
	Node();
	~Node();
	Node(int);
	Node(int, Node *);
	Node(int, Node *, Node *, Node *);
	Node(const Node &);

	Node	*getLeftChild() { return this->left; }
	Node	*getRightChild() { return this->right; }
	Node	*getParent() { return this->parent; }
	Item	getData() { return this->data; }

	void 	setLeftChild(Node *left) { this->left = left; }
	void 	setRightChild(Node *right) { this->right = right; }
	void	setParent(Node *parent) { this->parent = parent; }
	void	setData(Item data) { this->data = data; }

	bool	operator==(const Node &rhs);
	bool	operator<(const Node &rhs);
	bool	operator>(const Node &rhs);
	Node	&operator=(const Node &rhs);

	bool	isLeftChild();
	bool	isRightChild();

	void	print();
private:
	Item	data;
	Node	*left;
	Node	*right;
	Node	*parent;
};

template <class Item>
Node<Item>::Node()
{
	this->left = this->right = this->parent = NULL;
}

template <class Item>
Node<Item>::~Node()
{
	if (this->isLeftChild())
		parent->setLeftChild(NULL);
	else if (this->isRightChild())
		parent->setRightChild(NULL);
}

template <class Item>
Node<Item>::Node(int item)
{
	this->data = item;
	this->left = this->right = this->parent = NULL;
}

template <class Item>
Node<Item>::Node(int item, Node *parent)
{
	this->data = item;
	this->left = this->right = NULL;
	this->parent = parent;
}

template <class Item>
Node<Item>::Node(int item, Node *left, Node *right, Node *parent)
{
	this->data = item;
	this->left = left;
	this->right = right;
	this->parent = parent;
}

template <class Item>
Node<Item>::Node(const Node &rhs)
{
	this->data = rhs.data;
	this->left = rhs.left;
	this->right = rhs.right;
	this->parent = rhs.parent;
}

template <class Item>
void
Node<Item>::print()
{
	cout << "Data: " << this->data << endl;
	if (this->parent)
		cout << "Parent: " << this->parent->data << endl;
	if (this->left)
		cout << "Left: " << this->left->data << endl;
	if (this->right)
		cout << "Right: " << this->right->data << endl;
}

template <class Item>
bool
Node<Item>::isLeftChild()
{
	if (NULL == parent ||
		NULL == parent->left)
		return false;

	return (*(parent->left) == *this);
}

template <class Item>
bool
Node<Item>::isRightChild()
{
	if (NULL == parent ||
		NULL == parent->right)
		return false;

	return (*(parent->right) == *this);
}

template <class Item>
bool
Node<Item>::operator==(const Node &rhs)
{
	return (this->data == rhs.data) ? true : false;
}

template <class Item>
Node<Item>&
Node<Item>::operator=(const Node &rhs)
{
	Node<Item> *add = new Node(rhs);
	return *add;
}

template <class Item>
bool
Node<Item>::operator<(const Node &rhs)
{
	return (this->data < rhs.data) ? true : false;
}

template <class Item>
bool
Node<Item>::operator>(const Node &rhs)
{
	return (this->data > rhs.data) ? true : false;
}

