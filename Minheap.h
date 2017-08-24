#ifndef MINHEAP_H
#define MINHEAP_H

//static linking
#include <vector>
#include <map>
#include <stdlib.h>

/*
The class file represents a min-heap data structure.
In particular, it is represented by "array method",
so displacing father-son like : given i, its children
(if exists) are 2*i and 2*(i+1).
So, its attributes are the array, the array size, the
emptyness, the abstract data structure of its nodes.
Its main methods are inserting, checkFor, get the root
(the minimum key) etc.. some of them are based on the
important method heapify.
*/
using namespace std;
template <class Data, class Key> class Minheap {
	public:
		Minheap( );//Builder
		~Minheap( );//Destructor
		void insert(map<Data,Key> d);//Populator
		const bool isEmpty() { return isTreeEmpty; };//isEmpty

	protected:
		
		//protected methods are important in inheritance utilizations
		bool checkFor(Data data,string delegation, Key increment);//Getter/setter of key, given data
		const Data minimum( );//Getter of the root
	
	private:

		//inner class : min-heap node structure
		class minHeapNode {
		public:
			Data myData;//Key
			Key myKey;//Value
			minHeapNode(Data data, Key key) {
				this -> myData = data;
				this -> myKey = key;
			};//Initializer
			~minHeapNode(){};//Destructor
		};

		//attributes
		bool isTreeEmpty;
		vector<minHeapNode*> myArray;//Heap-array
		int heapsize;//Heap-offset

		//private methods
		void minHeapify(int i);//Heapify down
		void buildMinHeap();//Heap-property builder, once the heap has been populated 
		const minHeapNode* getParent(int i) {
			if(i <= this -> heapsize && (i/2) >= 1) return this -> myArray[i/2];
			return nullptr;
		};//Getter of parent
		const minHeapNode* getLeftSon(int i) { 
			if(i > 0 && i < this -> heapsize && (2*i) <= this -> heapsize) return this -> myArray[2*i];
			return nullptr;
		};//Getter of left son
		const minHeapNode* getRightSon(int i) { 
			if(i > 0 && i < this -> heapsize && (2*i + 1) <= this -> heapsize) return this -> myArray[2*i + 1];
		    return nullptr;
		};//Getter of right son
		void deleteNode(int i);//Used mainly in exctracting
};

//Builder/Setter without parameters
template <class Data, class Key> Minheap<Data,Key>::Minheap() {
	(this -> myArray).resize(1);
	this -> heapsize = 0;
	this -> isTreeEmpty = true;
}
template <class Data, class Key> Minheap<Data,Key>::~Minheap() {}

/*
Populator: the method fills the heap-array with a map, then it recalls buildMinHeap 
in order to reorder the heap
*/
template <class Data, class Key> void Minheap<Data,Key>::insert(map<Data,Key> d) {
	this -> heapsize = d.size();
	this -> isTreeEmpty = false;
	(this -> myArray).resize((this -> heapsize) + 1);
	minHeapNode* node;
	typename map<Data,Key>::iterator it;
	int i=1;
	for(it = d.begin();it != d.end(); it++) {
		node = new minHeapNode(it -> first, it -> second);
		this -> myArray[i] = node;
		i++;
	}
	this -> buildMinHeap();
}

/*
The heap-builder applys heapify (O(logn)) on half heap-array, it's proofable 
that its complexity of time is : O(2n)
*/
template <class Data, class Key> void Minheap<Data,Key>::buildMinHeap() {
	int i = this -> heapsize;
	for(int j = i/2; j > 0; j--) {
		this -> minHeapify(j);
	}
}
template <class Data, class Key> Data Minheap<Data,Key>::minimum() const {
	if(this -> isEmpty() == false) { 
		return this -> myArray[1] -> myData;
    }
	else { 
		Data empty_data; 
		return empty_data;
	}
}

/*
Heapify down : method that reorder the heap according to the following property:
key of father is <= of keys of its children
*/
template <class Data, class Key> void Minheap<Data,Key>::minHeapify(int i) {
	if(i < 1 || i > this -> heapsize ) return;
	minHeapNode* l = this -> getLeftSon(i);
	minHeapNode* r = this -> getRightSon(i);
	minHeapNode* min;
	int j=i;
	if (l != nullptr) {
		if((this -> myArray[i] -> myKey)<(l -> myKey)) {
			min = this -> myArray[i];
		}
		else {
			min = l;
			j = 2*i;
		}
	}
	if(r != nullptr) {
		if((r -> myKey) < (min -> myKey)) {
			min = r;
			j = 2*i + 1;
		}
	}
	if(j != i) {
		minHeapNode* temp = this -> myArray[j];
		this -> myArray[j] = this -> myArray[i];
		this -> myArray[i] = temp;
		this -> minHeapify(j);
	}
}

/*
Finder of a node given its internal data. With delegation parameter the method can edit its key,
or it can delete a the node
*/
template <class Data, class Key> bool Minheap<Data,Key>::checkFor(Data data,string delegation, Key k) {
	for(int i = 1; i <= this -> heapsize; i++) {
		if(this -> myArray[i] -> myData == data) {
			if(delegation == "Editing") {
				this -> myArray[i] -> myKey = k;

				//Heapify top, the heap-property is checked towards the root
				while(i > 1 && this -> getParent(i) != nullptr && (this -> getParent(i) -> myKey) > (this -> myArray[i] -> myKey)) {
					minHeapNode* temp = this -> myArray[i];
					this -> myArray[i] = this -> myArray[i/2];
					this -> myArray[i/2] = temp;
					i = i/2;
				}
				return true;
			}
			else {
				if(delegation == "Deleting") this -> deleteNode(i);
				return true;
			}
		}
	}
	return false;
}

//Method of deleting of a single node (it costs O(logn) because of the heapify check)
template <class Data, class Key> void Minheap<Data,Key>::deleteNode(int i) {
	minHeapNode* temp = this -> myArray[i];
	this -> myArray [i] = this -> myArray[this -> heapsize];
	this -> heapsize --;
	if(this -> heapsize == 0) this -> isTreeEmpty = true;
	(this -> myArray).resize(this -> heapsize + 1);
	this -> minHeapify(i);
}
#endif // MINHEAP _H
