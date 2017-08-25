#ifndef REDBLACK_H
#define REDBLACK_H

//static linking
#include <redBlackNode.h>

/*
The class file represents a red-black tree :
a data structure used for implementing priority
queues, or BST. Each node of the tree can be 
red or black (1 or 0), and the tree has to respect
4 properties: every root-leaf path has the same
number of black nodes, every red has only black children,
root is black and leaves are black. Thus, at most
the deepest level could be twice the lowest, so
the superior limit for every operation of the tree
is 2*lowestLevel; in full BST, having n nodes at 
least 2^(lowLevel+1)-1, n >= 2^(lowLevel+1)-1 => 
logn >=  lowLevel => red-black is O(2*logn).
Be careful, it needs to link redBlackNode to work.
*/
template <class Data, class Key, class T> class redBlack {
    public:

        redBlack(); //Builder
        ~redBlack(); //Destructor
		void insert(Data resource, Key otherKey);//Populator
		const Data minimum();//Getter of minimum key
		bool isEmpty();//isEmpty

    protected:
    	//toCheckFor is protected because it' s useful in inheritance, it recalls privately checkFor
        bool toCheckFor(T x, string delegation, Key increment);

    private:

    	//inner class : node of red-black
        class node: public redBlackNode<Data, Key, T> {
            public:

            	//each thing is owned by a node, not by the tree
                node(Data resource, Key otherkey):redBlackNode<Data, Key, T>::redBlackNode(resource,otherkey) {};
                ~node() { redBlackNode<Data, Key, T>::~redBlackNode(); };

                //Getters/Setters of each property (color, relatives, data, key, successor, etc..)
                const Color toGetColor() { return redBlackNode<Data, Key, T>::getColor(); };
                const node* toGetParent() { return static_cast<node*>(redBlackNode<Data, Key, T>::getParent()); };
                const node* toGetLeftSon() { return static_cast<node*>(redBlackNode<Data, Key, T>::getLeftSon()); };
                const node* toGetRightSon() { return static_cast<node*>(redBlackNode<Data, Key, T>::getRightSon()); };
                const Key toGetKey() { return redBlackNode<Data, Key, T>::getKey(); };
                const Data toGetData() { return redBlackNode<Data, Key, T>::getData(); };
                const bool toGetLeaf() { return redBlackNode<Data, Key, T>::getLeaf(); };
                void toSetColor(Color  color) { redBlackNode<Data, Key, T>::setColor(color); };
                void toSetParent(node* my_Node) { redBlackNode<Data, Key, T>::setParent(my_Node); };
                void toSetLeftSon(node* my_Node) { redBlackNode<Data, Key, T>::setLeftSon(my_Node); };
                void toSetRightSon(node* my_Node) { redBlackNode<Data, Key, T>::setRightSon(my_Node); };
                void toSetKey(Key my_Key) { redBlackNode<Data, Key, T>::setKey(my_Key); };
                void toSetData(Data my_data) { redBlackNode<Data, Key, T>::setData(my_data); };
				void toSetLeaf(bool is) { redBlackNode<Data, Key, T>::setLeaf(is); }

				//following methods use static cast for translating pointer of base class to derived class in runtime
                const node* toMyRoot() { return static_cast<node*>(redBlackNode<Data, Key, T>::myRoot()); };
                const node * toSuccessor() { return static_cast<node*>(redBlackNode<Data, Key, T>::successor()); };
                const node* toMaximum() { return static_cast<node*>(redBlackNode<Data, Key, T>::maximum()); };
                const node* toMinimum() { return static_cast<node*>(redBlackNode<Data, Key, T>::minimum()); };
		};
        node* nodeX;//node used as reference (it's usually the root)
        int nodes;//number of nodes
        node* leaf;//leaf sentinel node, it is the leaf of each node at the last level (diamond shape)
        bool found;//it's usefull in data search
        bool isTreeEmpty;

        //private methods
        void checkBalanceAfterInserting(node* ptr);//fixup after inserting
        void checkBalanceAfterDeleting(node* ptr, node* root, node* dad);//fixup after deleting
        void checkFor( T x, node* root, string delegation, Key increment);//data search (or editing)
        void deleteNode(node* myNode);//destroying a node
       
        //Rotation methods
        void rotate_right(node* myNode);
        void rotate_left(node* myNode);

};

//external declarations
template <class Data, class Key, class T> redBlack<Data, Key, T>::redBlack() {
 	this->found = false;
 	this -> isTreeEmpty = true;
 	this -> nodes = 0;
} 
template <class Data, class Key, class T> redBlack<Data, Key, T>::~redBlack() {}
//Insert : the same as BST insert (while you don't find the right key-position, go on searching)
template <class Data, class Key, class T> void redBlack<Data, Key, T>:: insert(Data resource, Key otherKey) {
    //trivial case : root
    if(this -> isEmpty()) {
        this -> nodeX = new node(resource, otherKey);
        this -> nodes++;
        this -> isTreeEmpty = false;
        Key empty_key;
        Data empty_data;
        this -> leaf = new node(empty_data, empty_key);
        this -> leaf -> toSetLeaf(true);
        this -> leaf -> toSetColor(black);
        this -> nodeX -> toSetLeftSon(this -> leaf);
        this -> nodeX -> toSetRightSon(this -> leaf);
    }     
    else {
    //inductive step	
        node* currentNode = this -> nodeX -> toMyRoot();
        bool inserted = false;
        //recursive restart once visited right subtree and left subtree
        while(inserted == false) {
        	//visit left subtree
            while((otherKey)<(currentNode -> toGetKey()) && (currentNode -> toGetLeftSon()) -> toGetLeaf() ==  false) {
                currentNode = currentNode -> toGetLeftSon();
            }
            //visit right subtree
            while((otherKey)>=(currentNode -> toGetKey()) && (currentNode -> toGetRightSon()) -> toGetLeaf() ==  false) {
                currentNode = currentNode -> toGetRightSon();
            }
            //if key is good-set, settle there and hook the leaf
            if((otherKey)<(currentNode -> toGetKey())  && (currentNode -> toGetLeftSon()) -> toGetLeaf() ==  true) {
                node* son = new node(resource, otherKey);
                currentNode -> toSetLeftSon(son);
                currentNode -> toGetLeftSon() -> toSetParent(currentNode);
                currentNode -> toGetLeftSon() -> toSetLeftSon(this -> leaf);
                currentNode -> toGetLeftSon() -> toSetRightSon(this -> leaf);
                this -> checkBalanceAfterInserting(currentNode -> toGetLeftSon());
                currentNode -> toMyRoot() -> toSetColor(black);
                this -> nodes++;
                inserted = true;
            }
            if((otherKey)>=(currentNode -> toGetKey())  && (currentNode -> toGetRightSon()) -> toGetLeaf() ==  true) {
                node* son = new node(resource, otherKey);
                currentNode -> toSetRightSon(son);
                currentNode -> toGetRightSon() -> toSetParent(currentNode);
                currentNode -> toGetRightSon() -> toSetLeftSon(this -> leaf);
                currentNode -> toGetRightSon() -> toSetRightSon(this -> leaf);
                this -> checkBalanceAfterInserting(currentNode -> toGetRightSon());
                currentNode -> toMyRoot() -> toSetColor(black);
                this -> nodes++;
                inserted = true;
            }
        }
        //overhead for reference
        this -> nodeX = this -> nodeX -> toMyRoot();
    }
}
template <class Data, class Key, class T> Data redBlack<Data, Key, T>::minimum() const {
    return this -> nodeX -> toMyRoot() -> toMinimum() -> toGetData();
}

//fixup after inserting (in input the pointer to new node). new node = me
template <class Data, class Key, class T> void redBlack<Data, Key, T>:: checkBalanceAfterInserting(node* ptr) {
    ptr -> toSetColor(red);//for hypothesis, i'm red
    node* temp1 = ptr;
    node* temp;
    node* root = ptr -> toMyRoot();
    
    //condition of while : i'm not root and my father is red
    while (temp1 != root  && temp1 -> toGetParent() -> toGetColor() == red)
    {
    
        //my father is left son
        if(temp1 -> toGetParent() == temp1 -> toGetParent() -> toGetParent() -> toGetLeftSon()) {
            temp = temp1 -> toGetParent() -> toGetParent() -> toGetRightSon();
    
            //my uncle is red
            if(temp -> toGetColor() == red) {
    
                //so daddy and uncle become black and grandpa red -> i'm grandpa 
                temp1 -> toGetParent() -> toSetColor(black);
                temp -> toSetColor(black);
                temp1 -> toGetParent() -> toGetParent() -> toSetColor(red);
                temp1 = temp1 -> toGetParent() -> toGetParent();
            }
            else { 
    
                //my uncle is black (or leaf)
                if(temp1 == temp1 -> toGetParent() -> toGetRightSon()) {
    
                    //i'm right son? ok, let's rotate left on my dad
                    temp1 = temp1 -> toGetParent();
                    this -> rotate_left(temp1);
                }
    
                /*
                my new dad becomes black, grandpa red and then let's rotate right on it!
                It sets the property of black children
				*/
                temp1 -> toGetParent() -> toSetColor(black);
                temp1 -> toGetParent() -> toGetParent() -> toSetColor(red);
                this -> rotate_right(temp1 -> toGetParent() -> toGetParent());
            }
        }
        else {
    
            //the else is simmetric to the if
            temp = temp1 -> toGetParent() -> toGetParent() -> toGetLeftSon();
            if(temp -> toGetColor() == red) {
                temp1 -> toGetParent() -> toSetColor(black);
                temp -> toSetColor(black);
                temp1 -> toGetParent() -> toGetParent() -> toSetColor(red);
                temp1 = temp1 -> toGetParent() -> toGetParent();
            }
            else {
                if(temp1 == temp1 -> toGetParent() -> toGetLeftSon()) {
                    temp1 = temp1 -> toGetParent();
                    this -> rotate_right(temp1);
                }
                temp1 -> toGetParent() -> toSetColor(black);
                temp1 -> toGetParent() -> toGetParent() -> toSetColor(red);
                this -> rotate_left(temp1 -> toGetParent() -> toGetParent());
            }
        }
    }
    
    //first property, i'm root, so i'm black
    if(temp1 == temp1 -> toMyRoot()) temp1 -> toSetColor(black);
}
template <class Data, class Key, class T> bool redBlack<Data, Key, T>::toCheckFor( T x, string delegation, Key increment) {
	this -> checkFor(x,nodeX -> toMyRoot(),delegation,increment);
	bool attempt = this -> found;
	this -> found = false;
	return attempt;
}
//data-search used for editing keys or deleting
template <class Data, class Key, class T> void redBlack<Data, Key, T>::checkFor( T x, node* root, string delegation, Key increment) {
    
//algorithm is a post-order visit of BST
    if((root -> toGetData()).getID() == x) {
            found = true;
        
            //Editing key
	        if(delegation == "Editing" && increment > 0) {
		        Data temp = root -> toGetData();
                Key key_temp = (root -> toGetKey())+increment;
                this -> deleteNode(root);
                this -> insert(temp, key_temp);
			}
            else {
        
            //Deleting
                if(delegation == "Deleting") this -> deleteNode(root);
            }
            return;
    }
    if(root -> toGetLeftSon() -> toGetLeaf() ==  true && root -> toGetRightSon() -> toGetLeaf() ==  true) {
            return;
    }
    else {

    //attribute found is useful when research has to break off (it reduces the complexity)
        if(root -> toGetLeftSon() -> toGetLeaf() ==  false && this -> found == false) {
            this -> checkFor(x, root -> toGetLeftSon(), delegation, increment);
        }
        if(root -> toGetRightSon() -> toGetLeaf() ==  false && this -> found == false) {
            this  -> checkFor(x, root -> toGetRightSon(), delegation, increment);
        }
    }
}

//Deleting. myNode = me
template <class Data, class Key, class T> void redBlack<Data, Key, T>::deleteNode(node* myNode) {
	
	//trivial case : i'm root and my children leaves, so let's deallocate my children, then me
	if(this -> nodes == 1) {
        this -> nodes = 0;
        this -> isTreeEmpty = true;
        return;
    }
    node* y;
    
    //inductive step : algorithm of deleting in BST; if i have both children, i have to look for my successor to replace me
    if(myNode -> toGetLeftSon() -> toGetLeaf() ==  true || myNode -> toGetRightSon() -> toGetLeaf() ==  true) {
        y = myNode;
    }
    else {
        y = myNode -> toSuccessor();
    }
    node* x;
    
    //x will contain or my unique son, or at most the unique son of my successor
    if(y -> toGetLeftSon() -> toGetLeaf() ==  false) {
        x = y -> toGetLeftSon();
    }
    else {
        x = y -> toGetRightSon();
    }
	if(x -> toGetLeaf() ==  false) x -> toSetParent(y -> toGetParent());
	node* dad = y -> toGetParent();
    
    //if i'm root, set a reference and then there will be a fixup..
    if(y->toGetParent() == nullptr) {
        nodeX = x;
    }
    else {

    //replace y with x
    if(y == y -> toGetParent() -> toGetLeftSon()) {
        y -> toGetParent() -> toSetLeftSon(x);
    }
    else {
        y -> toGetParent() -> toSetRightSon(x);
    }
    }
	if(y != myNode) {
        myNode -> toSetData(y -> toGetData());
        myNode -> toSetKey(y -> toGetKey());
    }

    //fixup has to be called passing x, and it has to be black
	if(y -> toGetColor() == black ) checkBalanceAfterDeleting(x, this -> nodeX -> toMyRoot(),dad);
    this -> nodeX = nodeX -> toMyRoot();
    this -> nodes--;
}
//post-delete fixup (input in order : ex son of deleted node, root, ex dad). ptr = me
template <class Data, class Key, class T> void redBlack<Data, Key, T>::checkBalanceAfterDeleting(node* ptr, node* root, node* dad )
{   
	//condition : i'm black and i'm not root
    while(ptr != root && ptr -> toGetColor() == black) {
		node* ptr1;
        if(ptr == dad -> toGetLeftSon()) {
        //if I'm left son
            if(dad -> toGetRightSon() -> toGetLeaf() ==  false){
                //if my brother isn't a leaf
                ptr1 = dad -> toGetRightSon();
                //if my brother is red, it becomes black and daddy red, let's rotate left on daddy, then i'm grandpa
                if(ptr1 -> toGetColor() == red) {
                    dad-> toSetColor(red);
                    ptr1 -> toSetColor(black);
                    this -> rotate_left(dad);
                    ptr1 = dad -> toGetRightSon();
                }
                //if my nephews are black, then my bro will become red and I'll be my dad
                if(ptr1 -> toGetLeftSon() -> toGetColor() == black && ptr1 -> toGetRightSon() -> toGetColor() == black) {
                    ptr1 -> toSetColor(red);
                    ptr = dad;
                    dad = dad -> toGetParent();
                }
                else {
                  //otherwise if only my right nephew is black, its bro'll be black, daddy red and rotate right on it
                    if(ptr1 -> toGetRightSon() -> toGetColor() == black) {
                        ptr1 -> toGetLeftSon() -> toSetColor(black);
                        ptr1 -> toSetColor(red);
                        this -> rotate_right(ptr1);
                        ptr1 = dad -> toGetRightSon();
                    }
                    /* 
                    nevertheless, if my left nephew is black, its bro'll be black, my bro of dad's color, daddy black and
                    let's rotate left on dad (my ex bro will be at most a red father of two black ones)
                    */ 
                    ptr1 -> toSetColor(dad -> toGetColor());
                    dad -> toSetColor(black);
                    if(ptr1 -> toGetRightSon() ->  toGetLeaf() ==  false) {
                    	ptr1 -> toGetRightSon() -> toSetColor(black);
                  	}
                  	this -> rotate_left(dad);
                  	ptr = myNode;
                  	dad = nullptr;
                }
            }
			else {
               	//my brother is a leaf, exit
               	ptr = myNode;
                dad = nullptr;
            }
        }
        else { 
        //i'm right son, else is symmetric to if	
            if(dad -> toGetLeftSon() -> toGetLeaf() ==  false){
                ptr1 = dad -> toGetLeftSon();
                if(ptr1 -> toGetColor() == red) {
                    dad -> toSetColor(red);
                    ptr1 -> toSetColor(black);
                    this -> rotate_right(dad);
                    ptr1 = dad -> toGetLeftSon();
                }
                if(ptr1 -> toGetRightSon() -> toGetColor() == black && ptr1 -> toGetLeftSon() -> toGetColor() == black) {
                    ptr1 -> toSetColor(red);
                    ptr = dad;
                    dad = dad -> toGetParent();
                }
                else {
                    if(ptr1 -> toGetLeftSon() -> toGetColor() == black) {
                        ptr1 -> toGetRightSon() -> toSetColor(black);
                        ptr1 -> toSetColor(red);
                        this -> rotate_left(ptr1);
                        ptr1 = dad -> toGetLeftSon();
                    }
                    ptr1 -> toSetColor(dad -> toGetColor());
                    dad -> toSetColor(black);
                    if(ptr1 -> toGetLeftSon() ->  toGetLeaf() ==  false) {
                        ptr1 -> toGetLeftSon() -> toSetColor(black);
                    }
                    this -> rotate_right(dad);
                    ptr = myNode;
                    dad = nullptr;
                }
            }
            else {
                ptr = myNode;
                dad = nullptr;
            }
        }
    }
    ptr -> toSetColor(black);
}
template <class Data, class Key, class T> bool redBlack<Data, Key, T>:: isEmpty() {
    return this -> isTreeEmpty;
}
/*
Rotations : operations involved in fixup; example, rotate to right on a node is like its left son becomes its dad 
from right, and the right son of its left son becomes his left son. Then, there could be a swap of colors in fixup.
Both rotations are logarithmic.
*/
template <class Data, class Key, class T> void redBlack<Data, Key, T>:: rotate_right(node* myNode) {
    
    //dealing with a swapping of pointers, the method has to save all references avoiding to lose them   
    node* temp = myNode -> toGetLeftSon();
    node* ptr = myNode;
    ptr -> toSetLeftSon(temp -> toGetRightSon());
    temp -> toGetRightSon() -> toSetParent(ptr);
    temp -> toSetParent(ptr -> toGetParent());
    if(ptr -> toGetParent() != nullptr) {
        if (ptr == ptr -> toGetParent() -> toGetLeftSon()) {
            ptr -> toGetParent() -> toSetLeftSon(temp);
        }
        else {
            ptr -> toGetParent() -> toSetRightSon(temp);
        }
    }
    temp -> toSetRightSon(ptr);
    ptr -> toSetParent(temp);
}
//rotate left is exactly symmetric to right
template <class Data, class Key, class T> void redBlack<Data, Key, T>:: rotate_left(node* myNode) {

    node* ptr = myNode;
    node* temp = ptr -> toGetRightSon();
    ptr -> toSetRightSon(temp -> toGetLeftSon());
    temp -> toGetLeftSon() -> toSetParent(ptr);
    temp -> toSetParent(ptr -> toGetParent());
    if(ptr -> toGetParent() != nullptr) {
        if (ptr == ptr -> toGetParent() -> toGetLeftSon()) {
            ptr -> toGetParent() -> toSetLeftSon(temp);
        }
        else {
            ptr -> toGetParent() -> toSetRightSon(temp);
        }
    }
    temp -> toSetLeftSon(ptr);
    ptr -> toSetParent(temp);
}
#endif // REDBLACK_H
