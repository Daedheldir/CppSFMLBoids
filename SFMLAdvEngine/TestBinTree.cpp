#include "TestBinTree.h"

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include <string>
#include <sstream>

#include <queue>

#include "BoidAgentData.h"
#include "BinTree.h"

#include "AdditionFunctor.h"
#include "MultiplicationFunctor.h"
#include "DivisionFunctor.h"

using namespace std;

typedef NodeB<std::shared_ptr<FunctorBase>>* PointerNodeB;

template <typename T>
void inorder(const BinTree<T>& bintree, const typename BinTree<T>::Iterator& it) {
	if (!it.EmptyTree()) {
		inorder(bintree, bintree.SubLeft(it));
		//cout <<  << " ";
		T base = it.observe();
		cout << base->getVal() << " ";
		inorder(bintree, bintree.SubRight(it));
	}
}

template <typename T>
void preorder(const BinTree<T>& bintree, const typename BinTree<T>::Iterator& it) {
	if (!it.EmptyTree()) {
		//cout <<  << " ";
		T base = it.observe();
		cout << base->getVal() << " ";
		preorder(bintree, bintree.SubLeft(it));
		preorder(bintree, bintree.SubRight(it));
	}
}

template <typename T>
void postorder(const BinTree<T>& bintree, const typename BinTree<T>::Iterator& it) {
	if (!it.EmptyTree()) {
		postorder(bintree, bintree.SubLeft(it));
		postorder(bintree, bintree.SubRight(it));
		//cout <<  << " ";
		T base = it.observe();
		cout << base->getVal() << " ";
	}
}

template <typename T>
void width(const BinTree<T>& bintree) {
	if (!bintree.isEmpty()) {
		queue<typename BinTree<T>::Iterator> c;
		typename BinTree<T>::Iterator ic = bintree.getRoot();
		c.push(ic);
		while (!c.empty()) {
			ic = c.front();
			c.pop();
			cout << ic.observe() << " ";
			if (!bintree.subLeft(ic).emptyTree())
				c.push(bintree.subIzq(ic));
			if (!bintree.subRight(ic).emptyTree())
				c.push(bintree.subRight(ic));
		}
	}
}

template <typename T>
const BinTree<T>& get2nodesSubTree(const BinTree<T>& bintree) {
	//typename BinTree<T>::Iterator ic = root;
	return *get2nodesSubTree<T>(bintree, bintree.getRoot());
}

template <typename T> //transform to boolean? and if there is a 2 nodes tree true else false
const BinTree<T>* get2nodesSubTree(const BinTree<T>& bintree, const typename BinTree<T>::Iterator& iterator) {
	//bintree no cambia, cambia iterator
	if (iterator.EmptyTree()) {
		return new BinTree<T>(); //empty tree
	}
	//from here at least the root is not empty
	else if (bintree.SubLeft(iterator).EmptyTree() && bintree.SubRight(iterator).EmptyTree())//case 1: there is root but not subleft or subright
		return new BinTree<T>(iterator.observe()); //returns a tree with just the root which means that it will be a int value >0 (negatives == operators)
	else if (!bintree.SubLeft(iterator).EmptyTree() && bintree.SubRight(iterator).EmptyTree()) //case 2: root == operator and left so left passes to be the root of that subtree
		return new BinTree<T>(bintree.SubLeft(iterator).observe());
	else if (bintree.SubLeft(iterator).EmptyTree() && !bintree.SubRight(iterator).EmptyTree()) //case 3: same as case 2 but with right
		return new BinTree<T>(bintree.SubRight(iterator).observe());
	else {
		typename BinTree<T>::Iterator itleft = bintree.SubLeft(iterator);
		typename BinTree<T>::Iterator itright = bintree.SubRight(iterator);
		if (IsA2NodesTree(bintree, iterator)) //case 4: root, left and right but just two nodes
											 // cannot be asked before because if subleft or subright are null = error
			return new BinTree<T>(bintree.getRoot().observe(), bintree.SubLeft(iterator).observe(), bintree.SubRight(iterator).observe());
		else
			return get2nodesSubTree(bintree, bintree.SubRight(iterator));
	}

	//return new BinTree<T>(iterator.observe(), iterator.getLeft(), iterator.getRight());
		/*
		*get3nodesSubTree(bintree, bintree.SubLeft(iterator)),
		*get3nodesSubTree(bintree, bintree.SubRight(iterator)));
		*/
}

/*template <typename T>
const PointerNodeB& get2nodesSubTreePointer(const BinTree<T>& bintree) {
	//typename BinTree<T>::Iterator ic = root;
	return *get2nodesSubTreePointer<T>(bintree, bintree.getRoot());
}

template <typename T> //transform to boolean? and if there is a 2 nodes tree true else false
const PointerNodeB* get2nodesSubTreePointer(const BinTree<T>& bintree, const typename BinTree<T>::Iterator& iterator) {
	//bintree no cambia, cambia iterator
	if (iterator.EmptyTree())
		return new PointerNodeB(); //empty node
	//from here at least the root is not empty
	else if (bintree.SubLeft(iterator).EmptyTree() && bintree.SubRight(iterator).EmptyTree())//case 1: there is root but not subleft or subright
		return new PointerNodeB(iterator); //returns a tree with just the root which means that it will be a int value >0 (negatives == operators)
	else if (!bintree.SubLeft(iterator).EmptyTree() && bintree.SubRight(iterator).EmptyTree()) //case 2: root == operator and left so left passes to be the root of that subtree
		return new PointerNodeB(bintree.SubLeft(iterator).observe());
	else if (bintree.SubLeft(iterator).EmptyTree() && !bintree.SubRight(iterator).EmptyTree()) //case 3: same as case 2 but with right
		return new PointerNodeB(bintree.SubRight(iterator).observe());
	else {
		typename BinTree<T>::Iterator itleft = bintree.SubLeft(iterator);
		typename BinTree<T>::Iterator itright = bintree.SubRight(iterator);
		if (IsA2NodesTree(bintree, iterator)) //case 4: root, left and right but just two nodes
											 // cannot be asked before because if subleft or subright are null = error
			return new PointerNodeB(bintree.getRoot().observe(), bintree.SubLeft(iterator).observe(), bintree.SubRight(iterator).observe());
		else
			return get2nodesSubTreePointer(bintree, bintree.SubRight(iterator));
	}

	//return new BinTree<T>(iterator.observe(), iterator.getLeft(), iterator.getRight());

		//get3nodesSubTree(bintree, bintree.SubLeft(iterator)),
		//get3nodesSubTree(bintree, bintree.SubRight(iterator)));
}*/

template <typename T>
bool IsA2NodesTree(const BinTree<T>& bintree, const typename BinTree<T>::Iterator& iterator) {
	bool ret = false;
	typename BinTree<T>::Iterator itleft = bintree.SubLeft(iterator);
	typename BinTree<T>::Iterator itright = bintree.SubRight(iterator);
	if (bintree.SubLeft(itleft).EmptyTree() && bintree.SubLeft(itright).EmptyTree())
		ret = true;
	return ret;
}

/*/template <typename T>
int functor(const BinTree<T>& bintree) {
	AdditionFunctor add;
	typename BinTree<T>::Iterator ic = bintree.getRoot();
	return add(bintree.SubLeft(bintree.SubLeft(ic).getRoot().observe(), bintree.SubRight(ic).getRoot().observe()));
}*/

/*template <typename T>
float calculate(const BinTree<T>& bintree) {
	float result=0;
	typedef NodeB<float>* PointerNodeB;

	while (!bintree.isEmpty()) {
		BinTree<float> testsubtree = get2nodesSubTree(bintree);
		//PointerNodeB testsubtree = get2nodesSubTreePointer(bintree);;
		int NodeValue = testsubtree.getRoot().observe(); //test2.getRoot().observe();

		if (NodeValue > 0) { //it means its not an operator
			cout << endl << "BinTree has finished with value: " << bintree.getRoot().observe() << endl;
			system("pause");
		}
		else if (NodeValue == -1) { // +
			AdditionFunctor add;
			result = add(testsubtree.getLeftObj(), testsubtree.getRightObj());
			cout << endl << "Operation + result: " << add(testsubtree.getLeftObj(), testsubtree.getRightObj());
		}
		else if (NodeValue == -2) { //*
			MultiplicationFunctor mul;
			result = mul(testsubtree.getLeftObj(), testsubtree.getRightObj());
			cout << endl << "Operation * result: " << mul(testsubtree.getLeftObj(), testsubtree.getRightObj());
		}
		else if (NodeValue == -3) { // /
			DivisionFunctor div;
			result = div(testsubtree.getLeftObj(), testsubtree.getRightObj());
			cout << endl << "Operation / result: " << div(testsubtree.getLeftObj(), testsubtree.getRightObj());
		}

		if (NodeValue < 0) {
			PointerNodeB nodepointer = testsubtree.getRootPointer();
			testsubtree.setRoot(result);
		}

		cout << endl;
		system("pause");

		return result;
	}
}*/

/*template <typename T>
const T& calculate(const BinTree<T>& bintree) {
	//typedef NodeB<T>* PointerNodeB;
	PointerNodeB rootpointer = bintree.getRootPointer();
	if (hasChildNodes(rootpointer)) {
		calculate(bintree,bintree.getRoot());
	}

	return bintree.getRootObj();
}*/

/*template <typename T>
bool hasChildNodes(const BinTree<T>& bintree, const typename BinTree<T>::Iterator& iterator) { //1
	if (bintree.getLeftPointer() != nullptr && bintree.getRightPointer() != nullptr)
		if ((bintree.getLeftPointer()->getLeft() == nullptr && bintree.getLeftPointer()->getRight() == nullptr) &&
			((bintree.getRightPointer()->getLeft() == nullptr) && bintree.getRightPointer()->getRight() == nullptr))
			return true;
		else
			return false;
}*/

template <typename T>
float calculatewithpointer(const BinTree<T>& bintree) {
	float result = 0;
	//typedef NodeB<T>* PointerNodeB;

	while (!bintree.isEmpty()) {
		//BinTree<float> testsubtree = get2nodesSubTree(bintree);
		PointerNodeB testsubtree = get2nodesSubTreePointer(bintree);;
		int NodeValue = testsubtree->getObj(); //testsubtree.getRoot().observe(); //test2.getRoot().observe();

		if (NodeValue > 0) { //it means its not an operator
			cout << endl << "BinTree has finished with value: " << bintree.getRoot().observe() << endl;
			system("pause");
		}
		else if (NodeValue == -1) { // +
			AdditionFunctor add;
			result = add(testsubtree->getLeft()->getObj(), testsubtree->getRight()->getObj());
			cout << endl << "Operation + result: " << add(testsubtree->getLeft()->getObj(), testsubtree->getRight()->getObj());
		}
		else if (NodeValue == -2) { //*
			MultiplicationFunctor mul;
			result = mul(testsubtree->getLeft()->getObj(), testsubtree->getRight()->getObj());
			cout << endl << "Operation * result: " << mul(testsubtree->getLeft()->getObj(), testsubtree->getRight()->getObj());
		}
		else if (NodeValue == -3) { // /
			DivisionFunctor div;
			result = div(testsubtree->getLeft()->getObj(), testsubtree->getRight()->getObj());
			cout << endl << "Operation / result: " << div(testsubtree->getLeft()->getObj(), testsubtree->getRight()->getObj());
		}

		if (NodeValue < 0) {
			testsubtree->setObj(result);
			testsubtree->setLeft(nullptr);
			testsubtree->setRight(nullptr);
		}

		cout << endl;
		system("pause");

		return result;
	}
}

//template <typename T>
void TestBinTree::testBinTree() {
	//craeting tree
	//typedef NodeB<T>* PtrNodeB;
	typedef std::shared_ptr<FunctorBase> FunctorBasePtr;
	float af = 1;
	float bf = 2;
	float cf = 3;
	float df = 0;
	FunctorBasePtr a(new FunctorBase(&af));
	FunctorBasePtr add_ab(new AdditionFunctor());
	FunctorBasePtr b(new FunctorBase(&bf));
	FunctorBasePtr add_ab_cd(new MultiplicationFunctor());
	FunctorBasePtr c(new FunctorBase(&cf));
	FunctorBasePtr add_cd(new DivisionFunctor());
	FunctorBasePtr d(new FunctorBase(&df));

	//                add_ab_cd
	//                /        \
    //            add_ab      add_cd
	//           /      \    /      \
    //          a       b    c      d

	BinTree<FunctorBasePtr> test1(add_ab_cd, BinTree<FunctorBasePtr>(), BinTree<FunctorBasePtr>());
	test1.insert(add_ab);
	test1.insert(add_cd);
	test1.insert(a);
	test1.insert(b);
	test1.insert(c);
	test1.insert(d);

	/*Orders*/
	cout << endl << "Orders" << endl;
	cout << endl << "InOrder: ";
	inorder(test1, test1.getRoot());
	cout << endl << "PreOrder: ";
	preorder(test1, test1.getRoot());
	cout << endl << "PostOrder: ";
	postorder(test1, test1.getRoot());

	cout << endl << "End of Orders";

	system("pause");

	system("cls");

	cout << "Calculation:" << endl << TestBinTree::calculate(test1, test1.getRoot());

	cout << endl << "End calculation";

	system("pause");

	system("cls");
};