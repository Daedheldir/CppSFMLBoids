#include <iostream>
#include "BinTree.h"
#include "FunctorBase.h"

using namespace std;

//template <typename T>
class TestBinTree {
private:
	float value;
public:
	void testBinTree();
	template <typename T>
	static float calculate(const BinTree<T>& bintree, const typename BinTree<T>::Iterator& iterator) {
		std::shared_ptr<NodeB<T>> rootpointer = iterator.getNodePtr(); //bintree.getRootPointer();
		if (hasChildNodes(rootpointer) && !iterator.EmptyTree()) {
			const T FunctorPtr = rootpointer->getObj();
			float leftval = calculate(bintree, bintree.SubLeft(iterator));
			float rightval = calculate(bintree, bintree.SubRight(iterator));
			float val = (*FunctorPtr)(leftval, rightval);

			return val;
		}

		const T base = iterator.observe(); //bintree.getRootObj();
		float val = base->getVal();
		return val;
	}
	template <typename T>
	static bool hasChildNodes(std::shared_ptr<NodeB<T>> NodePointer) { //1
		if (NodePointer->getLeft() != nullptr && NodePointer->getRight() != nullptr)
			if ((NodePointer->getLeft()->getLeft() == nullptr && NodePointer->getLeft()->getRight() == nullptr) &&
				((NodePointer->getRight()->getLeft() == nullptr) && NodePointer->getRight()->getRight() == nullptr))
				return true;
			else
				return hasChildNodes(NodePointer->getLeft()) || hasChildNodes(NodePointer->getRight());
		else
			return false;
	}
	template <typename T>
	static std::shared_ptr<NodeB<T>> getRandFunctorNode(const BinTree<T>& bintree, const typename BinTree<T>::Iterator& iterator) {
		std::shared_ptr<NodeB<T>> rootpointer = iterator.getNodePtr(); //bintree.getRootPointer();

		//if current node doesn't have children it's not a functor node, return its parent
		if (!hasChildNodes(rootpointer)) return nullptr;

		//else select randomly what to do
		int choice = rand() % 3;

		switch (choice)
		{
		case 0:	//go left
		{
			std::shared_ptr<NodeB<T>> node = getRandFunctorNode(bintree, bintree.SubLeft(iterator));
			if (node == nullptr) return rootpointer; //if nullptr was returned it means that it was a leaf and we need to return its parent, which is the current node

			return node; //if it wasn't a leaf then return whatever it has returned
		}
		case 1:	//go right
		{
			std::shared_ptr<NodeB<T>> node = getRandFunctorNode(bintree, bintree.SubRight(iterator));
			if (node == nullptr) return rootpointer; //if nullptr was returned it means that it was a leaf and we need to return its parent, which is the current node

			return node; //if it wasn't a leaf then return whatever it has returned
		}
		case 2:	//return current
		{
			return rootpointer;
		}
		}
	}
};