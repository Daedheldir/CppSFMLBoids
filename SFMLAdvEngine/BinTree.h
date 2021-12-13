#ifndef _ARBIN_H_
#define _ARBIN_H_


#include <iostream>

template <typename T = char>
class BinTree {
public:
    BinTree() = default;
    BinTree(const BinTree& a);
    BinTree(const T& object);
    BinTree(const T& object, const BinTree& al, const BinTree& ar);
    
    ~BinTree();

    void insert(const T& object);
    void remove(const T& object);
    void makeEmpty();
    bool isFind(const T& object)  const;

    void preOrder() const;
    void postOrder() const;
    void inOrder() const;

private:
    struct nodeB {
        T obj;
        nodeB* left;
        nodeB* right;
    public:
        nodeB(const T& object, nodeB* nleft, nodeB* nright) :obj(object), left(nleft), right(nright) {};
        nodeB(const T& object) :obj(object), left(nullptr), right(nullptr) {};
        nodeB(const nodeB& n) :obj(n->obj), left(n->left), right(n->right) {};

   
    };

    nodeB* root;
    nodeB* clone(const nodeB* bNode);
    void insert(const T& object, nodeB*& t);
    void remove(const T& object, nodeB*& t);
    void makeEmpty(nodeB*& t);
    bool isFind(const T& object, nodeB* t)  const;

    void preOrder(nodeB* bNode) const;
    void postOrder(nodeB* bNode) const;
    void inOrder(nodeB* bNode) const;
};

/*class Iterator {      // clase iterator anidada dentro de la clase arbin
        friend class BinTree<T>;
    public:
        const T& observe() const throw(TreePositionException)
        {
            if (pnode == NULL) throw TreePositionException();
            return pnode->getObj();
        }
        bool EmptyTree() const
        {
            return pnode == NULL;
        }
        bool operator!=(const Iterator& i) const
        {
            return (pnode != i.pnode);
        }
        bool operator==(const Iterator& i) const
        {
            return (pnode == i.pnode);
        }
        //int height() const;
    private:
        PtrnodeB pnode;
        Iterator(PtrnodeB p) { pnode = p; }
    };*/

	/*template <typename T>
	NodeB<T>* BinTree<T>::copy(PtrNodeB p) {
		if p = NULL
			return NULL;
		else
			return new NodoB<T>(p->getObj(), copy(p->getLeft()), copy(p->getRight())
	}*/

	/*template <typename T>
	BinTree<T>::BinTree() {
		//root = new NodeB<T>(object, copy(ai.root), copy(ad.root))
	}*/

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
	root = new nodeB(object);
	/*root->obj = object;
	root->left = nullptr;
	root->right = nullptr;*/
}

template <typename T>
BinTree<T>::BinTree(const T& object, const BinTree& al, const BinTree& ar) {
	root = new nodeB<T>(object, clone(al.root), clone(ar.root));
}


template <typename T>
BinTree<T>::~BinTree() {
	makeEmpty();
}

template< typename T>
void BinTree<T>::insert(const T& object) {
	insert(object, root);
}

template< typename T>
void BinTree<T>::remove(const T& object) {
	remove(object, root);
};

template< typename T>
void BinTree<T>::makeEmpty() {
	makeEmpty(root);
};

template< typename T>
bool BinTree<T>::isFind(const T& object) const {
	isFind(object, root);
};

template <typename T>
typename BinTree<T>::nodeB* BinTree<T>::clone(const nodeB* r) {
	if (nullptr == r) {
		return nullptr;
	}
	else {
		return  new nodeB(r->obj, clone(r->left), clone(r->right));
	}
};

template< typename T>
void BinTree<T>::insert(const T& object, nodeB*& t) {
	if (nullptr == t)
		t = new nodeB(object);
	else if (object < t->obj)
		insert(object, t->left);
	else if (object > t->obj)
		insert(object, t->right);
	else  //duplicate data is not inserted
	{
	}
}

template< typename T>
void BinTree<T>::remove(const T& object, nodeB*& t) {
	if (nullptr == t)
		return;
	else {
		if (object < t->obj)
			remove(t->left);
		else if (object > t->obj)
			remove(t->right);
		else if (nullptr != t->left && nullptr != t->right) {

			t->obj = min(t->right)->obj;
			remove(t->obj, t->right);
		}
		else {
			nodeB* oldNode = t;
			t = (nullptr != t->left) ? t->left : t->right;
			delete oldNode;
		}
	}
}

template< typename T>
void BinTree<T>::makeEmpty(nodeB*& t) {
	if (nullptr != t) {
		makeEmpty(t->left);
		makeEmpty(t->right);
		//cout << "delete: " << t->obj << endl;
		delete t;
	}
	t = nullptr;
};

template< typename T>
bool BinTree<T>::isFind(const T& object, nodeB* t) const {
	if (nullptr == t) {
		return false;
	}
	else if (object < t->obj)
		return isFind(object, t->left);
	else if (object > t->element)
		return isFind(object, t->right);
	else
		return true;
};

template< typename T>
void BinTree<T>::preOrder() const {
	preOrder(root);
};

template< typename T>
void BinTree<T>::inOrder() const {
	inOrder(root);
};


template< typename T>
void BinTree<T>::postOrder() const {
	postOrder(root);
};

template< typename T>
void BinTree<T>::preOrder(nodeB* bNode) const {
	if (nullptr != bNode) {
		cout << bNode->obj << " ";
		preOrder(bNode->left);
		preOrder(bNode->right);
	}

};

template< typename T>
void BinTree<T>::inOrder(nodeB* bNode) const {
	if (nullptr != bNode) {
		inOrder(bNode->left);
		cout << bNode->obj << " ";
		inOrder(bNode->right);
	}
};

template< typename T>
void BinTree<T>::postOrder(nodeB* bNode) const {
	if (bNode->left != NULL)
		postOrder(bNode->left);
	if (bNode->right != NULL)
		postOrder(bNode->right);
	cout << bNode->obj << " ";
};

/*
template <typename T>
BinTree<T>::BinTree& operator=(const BinTree& a) {

}

template <typename T>
BinTree<T>::Iterator getRoot() const;
BinTree<T>::Iterator subLeft(const Iterator& r) const throw(TreePositionException) {
	if r.pnode == NULL
		throw TreePositionException();
	return Iterator(r.pnodo->getLeft());
};
BinTree<T>::Iterator subRight(const Iterator& r) const throw(TreePositionException);

bool BinTree<T>::isEmpty() const;

int BinTree<T>::height() const {
	return getRoot().height();
};

int BinTree<T>::Iterator::height() const {
	if isEmpty()
		return 0;
	else
		return 1 + max(Iterator(pnode->getLeft()).height(), Iterator(pnode->getRight());
}

*/


#endif







