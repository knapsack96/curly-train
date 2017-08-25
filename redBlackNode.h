#ifndef REDBLACKNODE_H
#define REDBLACKNODE_H

//static linking
#include <iostream>
using namespace std;

//Structure for colors
enum Color {black, red};

/*
The class file represents a node of RB-tree.
The implementation has been done considering
every node as a root of a subtree, so including
its maximum, minimum, successor, predecessor..
Node class is external from RB tree class because
of its size (there are a lot of methods). So
node will be instantiated at runtime and hooked
at the main RB structure (redBlack).
*/
template <class Data, class Key, class T>
class redBlackNode
{
    public:
       
        redBlackNode();//Builder
        redBlackNode(Data resource, Key myKey);//Initializer
        ~redBlackNode();//Destructor

    protected:
        
        //Getters of all properties : relatives, color, data, key
        const redBlackNode* getRightSon(); 
        const redBlackNode* getLeftSon(); 
        const redBlackNode* getParent(); 
        const redBlackNode* getGrandparent();
        const redBlackNode* getUncle(); 
        const Color getColor(); 
        const Data getData() { return this->resource; }
        const Key getKey() { return this->myKey; }
        
        //Corresponding setters
        void setColor(Color color); 
        void setParent(redBlackNode<Data, Key, T>* parent); 
        void setLeftSon(redBlackNode<Data, Key, T>* leftSon); 
        void setRightSon(redBlackNode<Data, Key, T>* rightSon); 
        void setKey(Key otherKey);
        void setData(Data otherData);

        //Getters of other nodes
        const redBlackNode<Data, Key, T>* myRoot(); 
        const redBlackNode<Data, Key, T>* successor();
        const redBlackNode<Data, Key, T>* maximum();
        const redBlackNode<Data, Key, T>* minimum();

        //leaf
        void setLeaf(bool is) { this -> isLeaf = is; }
        bool getLeaf() { return this -> isLeaf; }

    private:
        
        //attributes
        Data resource; 
        Key myKey; 
        Color redBlackNodeColor;
        bool isLeaf;
        redBlackNode* rightSon;
        redBlackNode* leftSon;
        redBlackNode* parent;
};

//external declarations
template <class Data, class Key, class T> redBlackNode<Data, Key, T>::redBlackNode()   {
    this->isLeaf=false;
    this->setLeftSon(nullptr);
    this->setRightSon(nullptr);
    this->setParent(nullptr);
}

//initializer
template <class Data, class Key, class T> redBlackNode<Data, Key, T>::redBlackNode(Data resource, Key myKey)
{
    this->setLeftSon(nullptr);
    this->setRightSon(nullptr);
    this->setParent(nullptr);
    this -> isLeaf=false;
    this -> resource = resource;
    this -> myKey = myKey;
}
template <class Data, class Key, class T> redBlackNode<Data, Key, T>::~redBlackNode(){} 
template <class Data, class Key, class T> Color redBlackNode<Data, Key, T>::getColor() {
    return this -> redBlackNodeColor;
}
template <class Data, class Key, class T> redBlackNode<Data, Key, T>* redBlackNode<Data, Key, T>::getParent() {
    return this -> parent;
}
template <class Data, class Key, class T> redBlackNode<Data, Key, T>* redBlackNode<Data,Key, T>::getGrandparent() {
    if(this->getParent()!=nullptr) return this -> parent -> parent;
    return nullptr;
}
template <class Data, class Key, class T> redBlackNode<Data, Key, T>* redBlackNode<Data, Key, T>::getUncle() {
    redBlackNode* grandparent;
    if(this -> getParent() != nullptr) grandparent = getGrandparent();
    if(grandparent != nullptr) {
        if(this -> parent == grandparent -> rightSon) {
            return grandparent -> leftSon;
        }
        else {
            return grandparent -> rightSon;
        }
    }
}
template <class Data, class Key, class T> redBlackNode<Data, Key, T>* redBlackNode<Data, Key, T>::getLeftSon() {
    return this -> leftSon;
}
template <class Data, class Key, class T> redBlackNode<Data, Key, T>* redBlackNode<Data, Key, T>::getRightSon() {
    return this -> rightSon;
}
template <class Data, class Key, class T> void redBlackNode<Data, Key, T>::setColor(Color color) {
    this -> redBlackNodeColor = color;
}
template <class Data, class Key, class T> void redBlackNode<Data, Key, T>::setRightSon(redBlackNode<Data, Key, T>* rightSon) {
    this -> rightSon = rightSon;
}
template <class Data, class Key, class T> void redBlackNode<Data, Key, T>::setLeftSon(redBlackNode<Data, Key, T>* leftSon) {
    this -> leftSon = leftSon;
}
template <class Data, class Key, class T> void redBlackNode<Data, Key, T>::setParent(redBlackNode<Data, Key, T>* parent) {
    this -> parent = parent;
}
template <class Data, class Key, class T> void redBlackNode<Data, Key, T>::setKey(Key otherKey) {
    this -> myKey = otherKey;
}
template <class Data, class Key, class T> void redBlackNode<Data, Key, T>::setData(Data otherData) {
    this -> resource = otherData;
}

//root of all tree
template <class Data, class Key, class T> redBlackNode<Data,Key, T>* redBlackNode<Data, Key, T>::myRoot() {
    redBlackNode<Data, Key, T>* ptr = this;
    while(ptr -> getParent() != nullptr) {
        ptr = ptr -> getParent();
    }
    return ptr;
}

//after deleting, research successor
template <class Data, class Key, class T> redBlackNode<Data,Key,T>* redBlackNode<Data, Key, T>::successor() {

    //research of successor (minimum of right son)
    redBlackNode<Data, Key, T>* node = this;
    if(node -> getRightSon() -> isLeaf == false) {
        return node -> getRightSon() -> minimum();
    }

    //if it doesn't exist, research of predecessor: first ancestor of which the node it's included in right subtree
    redBlackNode<Data, Key, T>* p=node -> getParent();
    while(p != nullptr && node == p-> getLeftSon()) {
        node = p;
        p=node->getParent();
    }
    return p;

}

//Maximum key of subtree : in ABR, it's the deepest node at the right side
template <class Data, class Key, class T> redBlackNode<Data,Key,T>* redBlackNode<Data, Key, T>::maximum() {
    redBlackNode<Data, Key, T>* ptr = this;
    while(ptr -> getRightSon() -> isLeaf == false) {
        ptr = ptr -> getRightSon();
    }
    return ptr;
}

//Minimum key of subtree : in ABR, it's the deepest node at the left side 
template <class Data, class Key, class T> redBlackNode<Data,Key,T>* redBlackNode<Data, Key, T>::minimum()
{
    redBlackNode<Data, Key, T>* ptr = this;
    while(ptr -> getLeftSon() -> isLeaf == false) {
        ptr = ptr -> getLeftSon();
    }
    return ptr;
}
#endif // REDBLACKNODE_H
