#ifndef MINHEAP_H
#define MINHEAP_H
#include <vector>
#include <map>
//La classe implementa un albero binario di tipo minhea p ,
//ossia un albero che ha la seguente proprietà: il nodo padre è
//minore o uguale ai figli e l' albero è quasi completo, con l '
//ultimo livello tutto accatastato nella parte sinistr a .
//Viene controllato da un array di nodi, in cui il primo elemento è
//vuoto e gli altri contengono tutti i nodi dell' albero, il figl i o
//sinistro si trova nella sua posizione + 2*(sua posizion e )
//mentre il figlio destro in sua posizione + 2*(sua posizione) + 1 .
//Alla posizione 1 è sepmre presente il minimo (data la proprietà del l '
//albero). La proprietà viene mantenuta funzionale da alcune funzionalit à :
//inizialmente il buildMinHeap, che invoca il metodo minHeapify (control l o
//a scendere della proprietà a partire da un nodo padre) fino all' ulti m o
//padre. Un heapify a salire effettuato dopo un decreaseKey interno all' albe r o
//(che quindi altererebbe la proprietà verso l' alto, perchè ci potreb b e
//essere un figlio minore del padre). Dopo un' estrazione di minimo (radice) avvie n e
//sempre un minHeapify (a scenderedato che parte dalla radice ).
using std::map;
template <class Data, class Key> class Minheap
{
public:
Minheap( );//Costruttore
~Minheap( );//Distruttore
void insert(map<Data,Key> d);
bool isEmpty() { return isTreeEmpty; };
protected:
//Ricerca al suo interno per un parametro di input da ta
bool checkFor(Data data,std::string delegation, Key increment);
Data minimum( );//Metodo che restituisce il nodo radi ce
private:
bool isTreeEmpty;
class minHeapNode {
public:
//Attributi
Data myData;//il dato del no do
Key myKey;//chiave del no do
minHeapNode(Data data, Key key) {
this -> myData = data;
this -> myKey = key;
};//Inizializzato re
~minHeapNode(){};
};
std::vector<minHeapNode*> myArray;//Array che gestisce l' he ap
int heapsize;//dimensione dell' heap (size dell' array - 1, lo 0)
void minHeapify(int i );//Metodo di heapify verso il bas so
//I getter utilizzano gli interi per spiazzare le posizioni nell' arr ay
minHeapNode* getParent(int i );//Metodo che restituisce, se esiste, il pad re
//Metodi getter dei nodi fig li
minHeapNode* getLeftSon(int i) { if(i > 0 && i < this -> heapsize && (2*i) <= this -> heapsize) return this ->
myArray[2*i]; return nullptr;};
minHeapNode* getRightSon(int i) { if(i > 0 && i < this -> heapsize && (2*i + 1) <= this -> heapsize) return this ->
myArray[2*i + 1]; return nullptr;};
//ritorna il riferimento ad array[ 1]
minHeapNode* myRoot() { if(this -> heapsize > 1) return this -> myArray[1]; return nullptr;};
//Cancellazione del nodo i nell' arr ay
void deleteNode(int i);
//Inserimento, che utilizza il buildMinHeap, dopo aver inserito un certo numero di elemen ti
//dalla coda di priorità (per evitare un' elevata complessità di tempo : O(n) invece di O(nlogn) ).
void buildMinHeap();
};
template <class Data, class Key> Minheap<Data,Key>::Minheap() {(this -> myArray).resize(1);
this -> heapsize = 0;
this -> isTreeEmpty = true;
}//L'array aggiunge una locazio ne
template <class Data, class Key> Minheap<Data,Key>::~Minheap() {
for(int i=0;i<sizeof(myArray);i++) {
delete(myArray[i]);
}
}
template <class Data, class Key> typename Minheap<Data,Key>::minHeapNode* Minheap<Data,Key>::getParent(int i)
{
//Metodo che ritorna il nodo padre, se esiste, ossia myArray[i/ 2]
if(i <= this -> heapsize && (i/2) >= 1) return this -> myArray[i/2];
return nullptr;;
}
template <class Data, class Key> void Minheap<Data,Key>::insert(map<Data,Key> d) {
//Inserimento: viene passato come parametro di input una map contenente le copp ia
//di dati da aggiungere per poi richiamare la buildMinHea p.
this -> heapsize = d.size();
this -> isTreeEmpty = false;
(this -> myArray).resize((this -> heapsize) + 1);
minHeapNode* node;
typename map<Data,Key>::iterator it;
int i=1;
for(it = d.begin();it!=d.end();it++)
{
node = new minHeapNode(it->first, it->second);
this -> myArray[i] = node;
i++;
}
this -> buildMinHeap();
}
template <class Data, class Key> void Minheap<Data,Key>::buildMinHeap() {
//Effettua l' heapify a scendere dalla radice fino all' ultimo pad re
//in modo da ristabilire sicuramente e con una complessità O(n)
//la proprietà di he ap
int i = this -> heapsize;
for(int j = i/2; j>0; j--) {
this -> minHeapify(j);
}
}
//Metodo che ritorna il primo elemento dell' array, ossia il mini mo
template <class Data, class Key> Data Minheap<Data,Key>::minimum() {
if(this->isEmpty() == false) { return this -> myArray[1] -> myData; }
else { Data empty_data; return empty_data;}
}
//Heapify a scendere, ossia controllo che il padre sia minore di entrambi i figl i ,
//in caso il minore dei due sia minore del nodo padre lo si scambia con il nodo pad r e
//si procede a scendere sul nodo scambiato, ricorsivamente, fino al livello preceden t e
//le fogl ie
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
if((r -> myKey)<(min -> myKey)) {
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
//Metodo di ricerca con passaggio di un parametro di tipo data, nel nost r o
//caso l' ID dell' attività ricercata in coda, esegue una ricerca iterati v a
//sull' arr ay
template <class Data, class Key> bool Minheap<Data,Key>::checkFor(Data data,std::string delegation, Key k) {
for(int i = 1; i <= this -> heapsize; i++)
{
if(this -> myArray[i] -> myData == data) {
if(delegation == "Editing") {
this -> myArray[i] -> myKey = k;
//Effettua un heapify a salire (quindi nel caso in cui il nodo dell' albero modifica to
//non rispetti la proprietà heap) lo scambia con il padre e il controllo contin ua
//in modo iterativo sul pad re
while(i > 1 && this -> getParent(i) != nullptr && (this -> getParent(i) -> myKey)>(this -> myArray[i] -> myKey)) {
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
//Metodo di cancellazione del nodo i nell' array, scambia l' ultimo elemen t o
//con quest' ultimo e richiama un heapify a scendere dal nodo scambiato, in quan t o
//la situazione al di sopra di esso rimane invariata: non viene invocato dele t e
//perchè il puntatore al vecchio nodo è temp, che è alloca t o
//sullo stack e quindi verrà perso una volta ritornato il metodo (e quindi deallocato ).
template <class Data, class Key> void Minheap<Data,Key>::deleteNode(int i) {
minHeapNode* temp = this -> myArray[i];
this -> myArray [i] = this -> myArray[this -> heapsize];
this -> heapsize --;
if(this -> heapsize == 0) this -> isTreeEmpty = true;
(this -> myArray).resize(this -> heapsize + 1);
this -> minHeapify(i);
}
#endif // MINHEAP _H