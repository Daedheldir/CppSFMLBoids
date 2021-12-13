#ifndef _nodeB_H_
#define _nodeB_H_

#include <stdlib.h>

template <typename T>
class nodeB {
	typedef nodeB<T>* PtrnodeB;
public:
	nodeB(const T& object) :obj(object), left(nullptr), right(nullptr){};
	nodeB(const T& object, PtrnodeB nleft, PtrnodeB nright):obj(object), left(nleft), right(nright) {};
	nodeB(const nodeB& n):obj(n->obj), left(n->left), right(n->right){};

	nodeB* root;

	/*const T& getObj() const;
	PtrnodeB getLeft() const;
	PtrnodeB getRight() const;
	void setObj(const T& objeto);
	void setLeft(PtrnodeB nleft);
	void setRight(PtrnodeB nright);*/
private:
	T  obj;
	PtrnodeB  left, right; //nodeB *left,*right
};

#endif

