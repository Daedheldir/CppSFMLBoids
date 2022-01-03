#ifndef _NODEB_H_
#define _NODEB_H_

#include <iostream>
#include <stdlib.h>
#include <memory>

#include "FunctorBase.h"

template <typename T>
class NodeB {
	typedef std::shared_ptr<NodeB<T>> PointerNodeB;
public:
	//NodeB():left(nullptr), right(nullptr) {};
	NodeB(const T& object, PointerNodeB nleft, PointerNodeB nright) :obj(object), left(nleft), right(nright) {};
	NodeB(const T& object) :obj(object), left(nullptr), right(nullptr) {};
	NodeB(const NodeB& n) :obj(n.obj), left(n.left), right(n.right) {};
	~NodeB() {};
	const T& getObj() const { return obj; };
	PointerNodeB getLeft() const { return left; };
	PointerNodeB getRight() const { return right; };
	void setObj(const T& object) { this->obj = object; };
	void setLeft(PointerNodeB left) { this->left = left; };
	void setRight(PointerNodeB right) { this->right = right; };
private:
	T obj;
	std::shared_ptr<NodeB<T>> left;
	std::shared_ptr<NodeB<T>> right;
};

#endif
