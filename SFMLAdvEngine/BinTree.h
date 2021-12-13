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



#endif







