#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

//static linking
#include <iostream>
#include <Minheap.h>
#include <stdlib.h>

/*
The class file represents a priority queue
inheriting by a min-heap.
The implementation can be edited changing
the inner class, for example embeding a red
black tree.
*/
template <class Data, class Key> class priorityQueue {
	
	public:
		priorityQueue( );//Builder
		~priorityQueue( );//Destructor
		bool insert(map<Data,Key> d);//Populator
		void extractMin( );//Getter and deleter of the minimum
		const Data getMin( );//Getter of the minimum
		void editKey(Data x, Key k );//Edit a key
		const bool isEmpty( );//isEmpty
	
	private:

		//inner class : the structure underneath the queue, in this case it exploits a min-heap
		class tree: public Minheap<Data, Key> {
			public:
				tree():Minheap<Data, Key>::Minheap() {};//Builder
				~tree() { Minheap<Data, Key>::~Minheap(); };//Destructor

				//Following two methods recalls two inherited methods (under protection) by overloading
				bool checkFor(Data x, std::string delegation, Key increment) { 
					return Minheap<Data,Key>::checkFor(x,delegation,increment); 
				}
				const Data minimum() { return Minheap<Data,Key>::minimum(); }
		};
		tree* myMH;//Pointer to the min-heap
		bool isQueueEmpty;//Check for emptyness
};

//external declarations
template <class Data, class Key> priorityQueue<Data, Key>::priorityQueue()
{
	this -> isQueueEmpty = true;
	this -> myMH = new tree();
}
template <class Data, class Key> priorityQueue<Data, Key>::~priorityQueue() {}

//insert: a simply recalling of min-heap method
template <class Data, class Key> bool priorityQueue<Data, Key>:: insert(std::map<Data,Key> d) {
	this -> myMH -> insert(d);
	this -> isQueueEmpty = false;
	return true;
}

//extractMin calls the overloaded method checkFor (in the inner class) for extracting the minimum key
template <class Data, class Key> void priorityQueue<Data, Key>::extractMin() {
	if(this -> isQueueEmpty == false) {
		myMH -> checkFor(myMH -> minimum(), "Deleting",0);
		if(myMH -> isEmpty()) this -> isQueueEmpty = true;
	}
	else {
		cout << "There aren't Datas in queue!";
	}
}
template <class Data, class Key> Data priorityQueue<Data,Key>::getMin() const {
	if(this -> isQueueEmpty == false) {
		return myMH -> minimum();
	}
	else {
		cout << "There aren't Datas in queue!";
		Data empty_data;
		return empty_data;
	}
}
template <class Data, class Key> bool priorityQueue<Data, Key>:: isEmpty() const {
	return this -> isQueueEmpty; 
}
template <class Data, class Key> void priorityQueue<Data, Key>:: editKey(Data x, Key k) {
	if(this -> myMH -> checkFor(x,"Editing",k)) {
    	return;
	}
	else {
		cout << "Editing failed.";
	}
}

#endif 
