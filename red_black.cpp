#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "node.h"

using namespace std;

/*
 * Red Black Node
 * Inherits: Node.hh
 */
template <class Item>
class RedBlackNode : public Node<Item>
{
public:
	RedBlackNode() : Node<Item>(), color(BLACK) { }
	RedBlackNode(Item value, int color) : Node<Item>(value), color (color) { }
	RedBlackNode(Item value) : Node<Item>(value), color(RED) { }
	RedBlackNode(const RedBlackNode &rhs) : Node<Item>(rhs), color(rhs.color) {}

	unsigned getColor() { return this->color; }
	void setColor(unsigned color) { this->color = color; }

	RedBlackNode<Item>    *getLeftChild() { return (RedBlackNode<Item> *) Node<Item>::getLeftChild(); }
	RedBlackNode<Item>    *getRightChild() { return (RedBlackNode<Item> *) Node<Item>::getRightChild(); }
	RedBlackNode<Item>    *getParent() { return (RedBlackNode<Item> *) Node<Item>::getParent(); }

	static const unsigned RED = 0;
	static const unsigned BLACK = 1;

private:
	unsigned	color:1;
};

/*
 * Red Black Node static declarations
 */
template <class Item>
const unsigned RedBlackNode<Item>::RED;
template <class Item>
const unsigned RedBlackNode<Item>::BLACK;

/*
 * Red Black Tree
 */
template <class Item>
class RedBlackTree
{
public:
	RedBlackTree<Item>();
	~RedBlackTree();

	// Rotations
	void leftRotate(RedBlackNode<Item> *);
	void rightRotate(RedBlackNode<Item> *);

	// Insertions
	RedBlackNode<Item> *insert(Item);
	RedBlackNode<Item> *insert(RedBlackNode<Item> *, RedBlackNode<Item> *);
	RedBlackNode<Item> *insertFixUp(RedBlackNode<Item> *);

	// Deletion
	void remove(Item);
	void remove(RedBlackNode<Item> *);
	void removeFixUp(RedBlackNode<Item> *);
	void Transplant(RedBlackNode<Item> *, RedBlackNode<Item> *);

	// Find
	RedBlackNode<Item> *findNode(RedBlackNode<Item> *, Item);
	RedBlackNode<Item> *findMin(RedBlackNode<Item> *);
	RedBlackNode<Item> *findMax(RedBlackNode<Item> *);

	// Print Tree
	void print(RedBlackNode<Item> *);

	// Get/Set
	RedBlackNode<Item> *getRoot() { return this->root; }
	void setRoot(RedBlackNode<Item> *root)
		{ this->root = root; this->root->setParent(NIL); }

private:
	RedBlackNode<Item>	*root;
	RedBlackNode<Item>	*NIL;
};


/*
 * Red Black Tree Implementation
 */
template <class Item>
RedBlackTree<Item>::RedBlackTree()
{
	this->NIL = new RedBlackNode<Item>();
	this->NIL->setLeftChild(NIL);
	this->NIL->setRightChild(NIL);
	
	this->root = this->NIL;
}

template <class Item>
void RedBlackTree<Item>::leftRotate(RedBlackNode<Item> *x)
{
	RedBlackNode<Item> *y = x->getRightChild();	
	RedBlackNode<Item> *alpha = x->getLeftChild();	
	RedBlackNode<Item> *parent = x->getParent();	
	
	if (y) {
		RedBlackNode<Item> *beta = y->getLeftChild();
		x->setRightChild(beta);
		beta->setParent(x);
	}

	if (NIL != parent) {
		if (x->isLeftChild())
			parent->setLeftChild(y);
		else
			parent->setRightChild(y);

		y->setParent(parent);
	} else {
		setRoot(y);
	}

	y->setLeftChild(x);
	x->setParent(y);
}

template <class Item>
void RedBlackTree<Item>::rightRotate(RedBlackNode<Item> *y)
{
	RedBlackNode<Item> *x = y->getLeftChild();	
	RedBlackNode<Item> *lamda = y->getRightChild();	
	RedBlackNode<Item> *parent = y->getParent();	

	if (x) {
		RedBlackNode<Item> *beta = x->getRightChild();
		y->setLeftChild(beta);
		beta->setParent(y);
	}

	if (NIL != parent) {
		if (y->isLeftChild())
			parent->setLeftChild(x);
		else
			parent->setRightChild(x);

		x->setParent(parent);
	} else {
		setRoot(x);
	}

	x->setRightChild(y);
	y->setParent(x);
}

template <class Item>
RedBlackNode<Item> *
RedBlackTree<Item>::insert(Item add)
{
	RedBlackNode<Item> *node = new RedBlackNode<Item>(add);

	node->setLeftChild(NIL);
	node->setRightChild(NIL);

	if (NIL == getRoot()) {
		setRoot(node);
		getRoot()->setParent(NIL);
		node->setColor(RedBlackNode<Item>::BLACK);
		return node;
	}

	return insert(this->root, node);
}

/*
 * Insertion
 */
template <class Item>
RedBlackNode<Item> *
RedBlackTree<Item>::insert(RedBlackNode<Item> *root, RedBlackNode<Item> *node)
{
	if (NIL == root->getLeftChild() &&
		*node < *root) {
		root->setLeftChild(node);
		node->setParent(root);

		cout << "Root: " << root->getData() << " Left Child: "
			 << node->getData() << endl;

		insertFixUp(node);

		return node;
	} else if (NIL == root->getRightChild() &&
			   *node > *root) {
		root->setRightChild(node);
		node->setParent(root);

		cout << "Root: " << root->getData() << " Right Child: "
			 << node->getData() << endl;

		insertFixUp(node);

		return node;
	}

	if (*node < *root)
		insert(root->getLeftChild(), node);
	else
		insert(root->getRightChild(), node);

}

template <class Item>
RedBlackNode<Item> *
RedBlackTree<Item>::insertFixUp(RedBlackNode<Item> *z)
{
	RedBlackNode<Item> *parent = NULL;
	RedBlackNode<Item> *grand_parent = NULL;

	while (RedBlackNode<Item>::RED == z->getParent()->getColor()) {
		
		parent = z->getParent();
		grand_parent = parent->getParent();

		if (NIL == grand_parent)
			return z;

		if (grand_parent && parent->isLeftChild()) {
			RedBlackNode<Item> *y = grand_parent->getRightChild();

			// Case 1
			if (RedBlackNode<Item>::RED == y->getColor()) {
				cout << "Case 1" << endl;
				z->getParent()->setColor(RedBlackNode<Item>::BLACK);
				y->setColor(RedBlackNode<Item>::BLACK);
				grand_parent->setColor(RedBlackNode<Item>::RED);
				z = grand_parent;
			} else {
				// Case 2
				if (z->isRightChild()) {
					cout << "Case 2" << endl;
					z = parent;
					leftRotate(z);
				}

				// Case 3
				cout << "Case 3" << endl;
				z->getParent()->setColor(RedBlackNode<Item>::BLACK);
				z->getParent()->getParent()->setColor(RedBlackNode<Item>::RED);

				rightRotate(z->getParent()->getParent());
			}
		} else if (grand_parent && parent->isRightChild()) {
			RedBlackNode<Item> *y = grand_parent->getLeftChild();

			// Case 1
			if (RedBlackNode<Item>::RED == y->getColor()) {
				cout << "Case 1" << endl;
				((RedBlackNode<Item> *) z->getParent())->setColor(RedBlackNode<Item>::BLACK);
				y->setColor(RedBlackNode<Item>::BLACK);
				grand_parent->setColor(RedBlackNode<Item>::RED);
				z = grand_parent;
			} else {
				// Case 2
				if (z->isLeftChild()) {
					cout << "Case 2" << endl;
					z = parent;
					rightRotate(z);
				}

				// Case 3
				cout << "Case 3" << endl;
				z->getParent()->setColor(RedBlackNode<Item>::BLACK);
				z->getParent()->getParent()->setColor(RedBlackNode<Item>::RED);

				leftRotate(z->getParent()->getParent());
			}

		}
	}

	this->root->setColor(RedBlackNode<Item>::BLACK);
}

template <class Item>
RedBlackNode<Item> *
RedBlackTree<Item>::findNode(RedBlackNode<Item> *node, Item data)
{
	if (NIL == node)
		return node;

	cout << "findNode: " << node->getData() << endl;

	if (data == node->getData())
		return node;

	if (node->getData() > data)
		return findNode(node->getLeftChild(), data);
	else
		return findNode(node->getRightChild(), data);

}

template <class Item>
RedBlackNode<Item> *
RedBlackTree<Item>::findMin(RedBlackNode<Item> *node)
{
	if (NIL == node)
		return node;

	if (NIL == node->getLeftChild())
		return node;

	return findMin(node->getLeftChild());
}

template <class Item>
RedBlackNode<Item> *
RedBlackTree<Item>::findMax(RedBlackNode<Item> *node)
{
	if (NIL == node)
		return node;

	if (NIL == node->getRightChild())
		return node;

	return findMax(node->getRightChild());
}

template <class Item>
void RedBlackTree<Item>::Transplant(RedBlackNode<Item> *old_node,
									RedBlackNode<Item> *new_node)
{
	RedBlackNode<Item> *parent = old_node->getParent();

	if (old_node == getRoot()) {
		setRoot(new_node);
	} else if (old_node->isLeftChild()) {
		parent->setLeftChild(new_node);	
	} else if (old_node->isRightChild()) {
		parent->setRightChild(new_node);	
	}

	new_node->setParent(parent);
}

/*
 * Deletion
 */
template <class Item>
void RedBlackTree<Item>::remove(Item data)
{
	RedBlackNode<Item> *node = findNode(getRoot(), data);

	if (NIL != node) {
		remove(node);
	}
}

template <class Item>
void RedBlackTree<Item>::remove(RedBlackNode<Item> *z)
{
	unsigned y_original_color = 0;
	RedBlackNode<Item> *y = NULL;
	RedBlackNode<Item> *x = NULL;

	if (NULL == z ||
		NIL == z) {
		return;
	}

	cout << "Remove Node" << endl;

	y = z;
	y_original_color = z->getColor();

	if (NIL == z->getLeftChild()) {

		x = z->getRightChild();
		Transplant(z, y->getRightChild());
	} else if (NIL == z->getRightChild()) {

		x = z->getLeftChild();
		Transplant(z, y->getLeftChild());
	} else {

		y = findMin(z);
		y_original_color = y->getColor();
		x = y->getRightChild();

		if (y->getParent() == z) {
			x->setParent(y);
		} else {
			Transplant(y, y->getRightChild());
			y->setRightChild(y->getRightChild());
			(y->getRightChild())->setParent(y);
		}

		Transplant(z, y);
		y->setLeftChild(z->getLeftChild());
		(z->getLeftChild())->setParent(y);
		y->setColor(z->getColor());

	}

	if (y_original_color == RedBlackNode<Item>::BLACK)
		removeFixUp(x);
}

template <class Item>
void RedBlackTree<Item>::removeFixUp(RedBlackNode<Item> *x)
{
	RedBlackNode<Item> *w = NULL;

	cout << "FixUp" << endl;

	while (x != this->root &&
		   x->getColor() == RedBlackNode<Item>::BLACK) {

		if (x->isLeftChild()) {
			w = x->getParent()->getRightChild();

			if (w->getColor() == RedBlackNode<Item>::RED) {
				w->setColor(RedBlackNode<Item>::BLACK);
				x->getParent()->setColor(RedBlackNode<Item>::RED);
				leftRotate(x->getParent());
				w = x->getParent()->getRightChild();
			}

			if (w->getColor() == RedBlackNode<Item>::BLACK &&
				w->getLeftChild()->getColor() == RedBlackNode<Item>::BLACK &&
				w->getRightChild()->getColor() == RedBlackNode<Item>::BLACK) {

				w->setColor(RedBlackNode<Item>::RED);
				x = x->getParent();

			} else {

				if (w->getColor() == RedBlackNode<Item>::BLACK &&
				 w->getLeftChild()->getColor() == RedBlackNode<Item>::RED &&
				 w->getRightChild()->getColor() == RedBlackNode<Item>::BLACK) {

					w->setColor(RedBlackNode<Item>::RED);
					w->getLeftChild()->setColor(RedBlackNode<Item>::BLACK);
					rightRotate(w);
					w = x->getParent()->getRightChild();
				}

				w->setColor(x->getParent()->getColor());
				w->getRightChild()->setColor(RedBlackNode<Item>::BLACK);	
				x->getParent()->setColor(RedBlackNode<Item>::BLACK);
				leftRotate(x->getParent());
				setRoot(x);
			}
		} else {
			w = x->getParent()->getLeftChild();

			if (w->getColor() == RedBlackNode<Item>::RED) {
				w->setColor(RedBlackNode<Item>::BLACK);
				x->getParent()->setColor(RedBlackNode<Item>::RED);
				rightRotate(x->getParent());
				w = x->getParent()->getLeftChild();
			}

			if (w->getColor() == RedBlackNode<Item>::BLACK &&
				w->getLeftChild()->getColor() == RedBlackNode<Item>::BLACK &&
				w->getRightChild()->getColor() == RedBlackNode<Item>::BLACK) {

				w->setColor(RedBlackNode<Item>::RED);
				x = x->getParent();

			} else {

				if (w->getColor() == RedBlackNode<Item>::BLACK &&
				 w->getRightChild()->getColor() == RedBlackNode<Item>::RED &&
				 w->getLeftChild()->getColor() == RedBlackNode<Item>::BLACK) {

					w->setColor(RedBlackNode<Item>::RED);
					w->getRightChild()->setColor(RedBlackNode<Item>::BLACK);
					leftRotate(w);
					w = x->getParent()->getLeftChild();
				}

				w->setColor(x->getParent()->getColor());
				w->getLeftChild()->setColor(RedBlackNode<Item>::BLACK);	
				x->getParent()->setColor(RedBlackNode<Item>::BLACK);
				rightRotate(x->getParent());
				setRoot(x);
			}
		}	
	}

	x->setColor(RedBlackNode<Item>::BLACK);
}

template <class Item>
void RedBlackTree<Item>::print(RedBlackNode<Item> *root)
{
	if (NIL == root)
		return;

	print(root->getLeftChild());
	cout << "Data: " << root->getData() << " " << endl;
	cout << "Color: " << root->getColor() << " " << endl;
	print(root->getRightChild());
}

int
main()
{
	RedBlackTree<int> *tree = new RedBlackTree<int>();

	tree->insert(4);
	tree->insert(7);
	tree->insert(12);
	tree->insert(15);
	tree->insert(3);
	tree->insert(5);
	tree->insert(14);
	tree->insert(18);
	tree->insert(16);
	tree->insert(17);

	//tree->remove(5);

	cout << "Root is: " << tree->getRoot()->getData() << endl;
	tree->print(tree->getRoot());

	return 0;
}
