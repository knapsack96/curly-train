#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H
#include <iostream>
#include <Minheap.h>
#include <stdlib.h>
//La classe implementa una struttura coda di priorità, che dovrà interfacciare un albero binario minhea p ,
//è templata e presenta 2 parametri generici a sua volta, il tipo Data per g l i
//Datai contenuti in coda (che servirà per passare oggetti di tipo Vertice nel nostro caso ) ,
//il tipo Key (ossia la priorità di un Datao della coda legato univocamente a un tipo aritmetic o ,
//in quanto sfrutta operazioni logiche di confronto sui numeri ).
using namespace std;
template <class Data, class Key>
class priorityQueue
{
public:
priorityQueue( );//costrutto re
~priorityQueue( );//distrutto re
bool insert(map<Data,Key> d );//Inserimento in coda di un
//nodo con dato Data e chiave otherK ey
void extractMin( );//Estrae il dato e lo ritorna per valo re
// con la minima chiave (la radi ce
//dell' heap associat o)
Data getMin( );//getter del dato ritornato per valore e non elimina to
void increaseKey(Data x, Key k );//diminuisce la chiave di un Datao x
//all' interno della co da
bool isEmpty( );//Controllo su gli Datai presenti in co da
private:
bool isQueueEmpty;//Attributo per la presenza di Datai in co da
class tree: public Minheap<Data, Key> {
public:
tree():Minheap<Data, Key>::Minheap() {};
~tree() { Minheap<Data, Key>::~Minheap(); };
bool checkFor(Data x, std::string delegation, Key increment) { return Minheap<Data,
Key>::checkFor(x,delegation,increment); }
Data minimum() { return Minheap<Data,Key>::minimum(); }
};
tree* myMH;//Attributo del minheap associa to
};
template <class Data, class Key> priorityQueue<Data, Key>::priorityQueue()
{
this -> isQueueEmpty=true;
this -> myMH = new tree();
}
template <class Data, class Key> priorityQueue<Data, Key>::~priorityQueue()
{
//lo cancella quando la coda viene dealloca ta
}
//Metodo di inserimen to
template <class Data, class Key> bool priorityQueue<Data, Key>:: insert(std::map<Data,Key> d) {
this -> myMH -> insert(d );//Viene inserito un nuovo nodo minHeapNode nell' albero he ap
this -> isQueueEmpty = false ; //La coda non è più vuo ta
return true;
}
//Metodo per l'estrazione dell' Datao con chiave minima, nel nostro ca s o
//sarà per i cammini minimi la distanza dalla sorgente aggiunta in coda nel l '
//algoritmo di Dijkstra
template <class Data, class Key> void priorityQueue<Data, Key>::extractMin() {
if(this -> isQueueEmpty == false) {
myMH -> checkFor(myMH->minimum(), "Deleting",0);
if(myMH -> isEmpty()) this -> isQueueEmpty = true;
}
else {
cout << "There aren't Datas in queue!";
}
}
//Getter del valore del dato della radice, senza la sua estrazio ne
template <class Data, class Key> Data priorityQueue<Data,Key>::getMin() {
if(this -> isQueueEmpty == false) {
return myMH -> minimum();
}
else {
std::cout << "There aren't Datas in queue!";
Data empty_data;
return empty_data;
}
}
//test isEmpty
template <class Data, class Key> bool priorityQueue<Data, Key>:: isEmpty() {return this -> isQueueEmpty; }
//Metodo per la modifica di una chiave a un valore minore, utilizzato solo nell' algorit m o
//di Dijkstra per l' aggiornamento di una distanza di un qualsiasi vertice dal vertice sorgen te
template <class Data, class Key> void priorityQueue<Data, Key>:: increaseKey(Data x, Key k) {
if(this -> myMH -> checkFor(x,"Editing",k));
{
return;
//std::cout << "Decreased succesfully! ";
}
}

#endif 