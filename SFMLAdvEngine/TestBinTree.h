#include <iostream>
#include "BinTree.h"
using namespace std;

//template <typename T>
class TestBinTree {
private:
	float value;
public:
	void testBinTree();
	template <typename T>
	static float calculate(const BinTree<T>& bintree, const typename BinTree<T>::Iterator& iterator) {
		NodeB<T>* rootpointer = iterator.getNodePtr(); //bintree.getRootPointer();
		if (hasChildNodes(rootpointer) && !iterator.EmptyTree()) {
			FunctorBase* FunctorPtr = rootpointer->getObj();
			float leftval = calculate(bintree, bintree.SubLeft(iterator));
			float rightval = calculate(bintree, bintree.SubRight(iterator));
			float val = (*FunctorPtr)(leftval, rightval);

			return val;
		}

		FunctorBase* base = iterator.observe(); //bintree.getRootObj();
		float val = base->getVal();
		return val;
	}
	template <typename T>
	static bool hasChildNodes(NodeB<T>* NodePointer) { //1
		if (NodePointer->getLeft() != nullptr && NodePointer->getRight() != nullptr)
			if ((NodePointer->getLeft()->getLeft() == nullptr && NodePointer->getLeft()->getRight() == nullptr) &&
				((NodePointer->getRight()->getLeft() == nullptr) && NodePointer->getRight()->getRight() == nullptr))
				return true;
			else
				return hasChildNodes(NodePointer->getLeft()) || hasChildNodes(NodePointer->getRight());
		else
			return false;
	}
};