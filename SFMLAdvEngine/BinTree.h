#ifndef _ARBIN_H_
#define _ARBIN_H_

#include <iostream>

#include <queue>
#include <limits>

#include "BinTreeException.h"

#include "NodeB.h"

#include "FunctorBase.h"

//#include <iterator>

template <typename T>
class BinTree {
	typedef NodeB<T>* PointerNodeB;
public:

	class Iterator {      // iterator class nested into BinTree
		friend class BinTree<T>;
	public:
		const T& observe() const throw(TreePositionException) {
			if (NodePointer == nullptr)
				throw TreePositionException();
			return NodePointer->getObj();
		}
		bool EmptyTree() const { return NodePointer == nullptr; }
		bool operator!=(const Iterator& i) const { return (NodePointer != i.NodePointer); }
		bool operator==(const Iterator& i) const { return (NodePointer == i.NodePointer); }
		int height() const;
		PointerNodeB getNodePtr() const { return NodePointer; }
	private:
		PointerNodeB NodePointer;
		Iterator(PointerNodeB pointer) { NodePointer = pointer; }
	};

	//constructors

	BinTree();
	BinTree(const BinTree& a);
	BinTree(const T& object);
	BinTree(const T& object, const BinTree& bintreeleft, const BinTree& bintreeright);
	BinTree(const PointerNodeB nodepointer);

	//destructor
	~BinTree();

	BinTree& operator= (const BinTree& bintree) {
		PointerNodeB nodepointer = clone(bintree.getRootPointer());
		BinTree bt(bintree.getRootPointer());
		return bt;
	};

	Iterator getRoot() const { return root; };

	Iterator SubRight(const Iterator& r) const throw(TreePositionException) {
		if (r.NodePointer == nullptr)
			throw TreePositionException();
		return Iterator(r.NodePointer->getRight());
	};

	Iterator SubLeft(const Iterator& r) const throw(TreePositionException) {
		if (r.NodePointer == nullptr)
			throw TreePositionException();
		return Iterator(r.NodePointer->getLeft());
	};

	bool isEmpty() const;

	int height() const { return getRoot().height(); };

	void insert(const T& object);
	//void insert(const T& object, PointerNodeB& t);

	const T& getLeftObj() const {
		return root->getLeft()->getObj();
	}

	PointerNodeB getLeftPointer() const {
		return root->getLeft();
	}

	const T& getRightObj() const {
		return root->getRight()->getObj();
	}

	PointerNodeB getRightPointer() const {
		return root->getRight();
	}

	PointerNodeB getRootPointer() const {
		return root;
	}

	const T& getRootObj() const {
		return root->getObj();
	}

	void setRoot(const T& object) {
		root = new NodeB(object);
	}

protected:
	PointerNodeB root;
	PointerNodeB clone(const PointerNodeB NPointer) {
		if (NPointer == nullptr)
			return nullptr;
		else
			return (new NodeB<T>(NPointer->getObj(), clone(NPointer->getLeft()), clone(NPointer->getRight())));
	};

	void remove(PointerNodeB NPointer) {
		if (NPointer == nullptr)
			return;
		else {
			if (NPointer->getLeft() == nullptr && NPointer->getRight() == nullptr) {
				delete NPointer;
				NPointer == nullptr;
			}
			else if (NPointer->getLeft() != nullptr && NPointer->getRight() != nullptr) {
				remove(NPointer->getLeft());
				remove(NPointer->getRight());
			}
			else if (NPointer->getLeft() != nullptr && NPointer->getRight() == nullptr)
				remove(NPointer->getLeft());
			else if (NPointer->getLeft() == nullptr && NPointer->getRight() != nullptr)
				remove(NPointer->getRight());
			else {
				//PointerNode oldNode = NPointer;
				//NPointer = (NPointer->getLeft() != nullptr) ? NPointer->getLeft() : NPointer->getRight();
				//delete oldNode;
			}
		}
	};
};

//ostream& operator<<(ostream& s, const BinTree& c); //class friend function

//constructors

template <typename T>
BinTree<T>::BinTree() {
	root = nullptr;
}

template <typename T>
BinTree<T>::BinTree(const BinTree& a) {
	if (nullptr != a.root) {
		this->root = clone(a.root);
	}
	else {
		root = nullptr;
	}
}

template< typename T>
BinTree<T>::BinTree(const T& object) {
	root = new NodeB(object);
	/*root->obj = object;
	root->left = nullptrptr;
	root->right = nullptrptr;*/
}

template <typename T>
BinTree<T>::BinTree(const PointerNodeB nodepointer) {
	root = nodepointer;
}

template <typename T>
BinTree<T>::BinTree(const T& object, const BinTree& bintreeleft, const BinTree& bintreeright) {
	root = new NodeB<T>(object, clone(bintreeleft.root), clone(bintreeright.root));
}

//destructor

template <typename T>
BinTree<T>::~BinTree() {
	remove(root);
}

/*template <class T>
const T& max(const T& a, const T& b) {
	return (a < b) ? b : a;     // or: return comp(a,b)?b:a; for version (2)
}*/

template <typename T>
int BinTree<T>::Iterator::height() const {
	if (EmptyTree())
		return 0;
	else
		return 1 + max((Iterator(NodePointer->getLeft()).height()), Iterator(NodePointer->getRight()).height());
}

template <typename T>
bool BinTree<T>::isEmpty() const {
	if (getLeftPointer() == nullptr && getRightPointer() == nullptr)
		return true;
	else
		return false;
};

template< typename T>
void BinTree<T>::insert(const T& object) {
	std::queue<typename BinTree<T>::Iterator> queu;
	typename BinTree<T>::Iterator ic = root;
	queu.push(ic);
	PointerNodeB aux = new NodeB<T>(object);

	//Do level order traversal until we find an empty place
	while (!queu.empty()) {
		ic = queu.front();
		PointerNodeB aux2(ic.NodePointer);
		queu.pop();

		if (aux2->getLeft() == nullptr) {
			aux2->setLeft(aux);
			break;
		}
		else
			queu.push(aux2->getLeft());

		if (aux2->getRight() == nullptr) {
			aux2->setRight(aux);
			break;
		}
		else
			queu.push(aux2->getRight());

		//aux2 = nullptr;
		//delete aux2;
	}
	//aux = nullptr;
	//delete aux;

	//insert(object, root);
}

/*template< typename T>
void BinTree<T>::insert(const T& object, PointerNodeB& NodePointer) {
	if (NodePointer == nullptr)
		NodePointer = new NodeB(object);
	else if (object < NodePointer->getObj)
		insert(object, NodePointer->getLeft);
	else if (object > NodePointer->getObj)
		insert(object, NodePointer->getRight);
	else  //duplicate data is not inserted
	{
	}
}*/

#endif