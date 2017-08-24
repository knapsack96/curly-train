#ifndef GRAPH_H
#define GRAPH_H

//static linking
#include <map>
#include <limits>
#include <priorityQueue.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

/* 
The following class file represents a weighted oriented graph.
Its attributes are: the collection of vertices and of
arcs of the graph and a pointer to a priority
queue. 
Its methods are: population, find a shortest path 
between two vertices (using Dijkstra), isEmpty() and
a getter of arcs (given two vertices).
In applying Dijkstra algorithm we don't need to know if the
graph is full-connected, in fact if it wasn't the algorithm 
doesn't get any output.
*/
template <class Data> class Graph
{
    public:

        Graph();//Builder
        ~Graph();//Destructor
        void populateGraph(const string file1);//Populator
        void shortestPath(Data data,Data data1); //Getter of a shortest path between two vertices
        bool isEmpty() const { if((this->vertices).size() > 0) return false; return true; };//isEmpty

    protected:
    private:
    	
    	//inner classes: vertex and arc of graph (necessary for protection)
        class Vertex
        {
            public:
            Vertex(Data data)
            {
                this->myData=data;
            }//Initializer
            ~Vertex(){}//Destructor
            Data myData;//Internal template data of vertex
            vector<Vertex*> myAdjacent;//Collection of adjacent vertex
        };
        class Arc
        {
            public:
            Arc(Vertex* f,Vertex* s,int w) {
                this->first=f;
                this->second=s;
                this->weight=w;
            }//Initializer
            ~Arc(){}//Destructor
            Vertex* first; //Pointer vertex
            Vertex* second; //Pointed vertex
            int weight; //Weight of the arc
        };
		vector<Vertex*> vertices;//Container of all vertex
        vector<Arc*> arcs;//Container of all arcs
        priorityQueue<Vertex*,int>* myPQ;//Queue used by Dijkstra's algorithm
        Arc* findArc(const Vertex* first, const Vertex* second);//Getter of an arc
};

//External declaration
template <class Data> Graph<Data>::Graph() {}
template <class Data> Graph<Data>::~Graph() {}

//populateGraph fills vertices and arcs reading by a file in the same folder of the .cpp
template <class Data> void Graph<Data>::populateGraph(const std::string file1) {

    //bufferized reading is implemented reading line by line the file..
    string line;
    int j = 0;
    std::vector<Vertex*> allVertex;
    std::vector<Arc*> allArc;
    Vertex* vert;
    Vertex* temp;
    Arc* arc;
    std::ifstream file(file1);
    if(file.is_open()){
        while (!file.eof()){
            getline(file,line);
            char split_char = ':';
            std::istringstream split(line);
            vector<string> tokens;
            for (std::string each; std::getline(split, each, split_char); tokens.push_back(each));
            bool thereIs = false;

            //..then splitting the line in token and getting the vertices and their adjiacent vertices too.
            for (int i = 0; i < tokens.size(); i++) {
                if(line != ""  && line != "\n"  && j == 0) {
                    thereIs = false;
                    if(i == 1) break;
                    for(typename std::vector<Vertex*>::iterator x = allVertex.begin(); x != allVertex.end(); ++x) {
                        if(((*x) -> myData).compare(tokens.at(i)) == 0) {   
                          thereIs = true;
                          vert = *x;
                        }
                    }
                    if(thereIs == false) {
                        vert = new Vertex(tokens.at(i));
                        allVertex.push_back(vert);
                    }
                }
                if(line != "" && line != "\n" && j == 1) {
                    thereIs = false;
                    for(typename std::vector<Vertex*>::iterator x = allVertex.begin(); x != allVertex.end(); ++x) {
                        if(((*x) -> myData).compare(tokens.at(i)) == 0) {
                            temp = *x;
                            thereIs = true;
                        }
                    }
                    if(thereIs == false) {
                     	temp = new Vertex(tokens.at(i));
                        allVertex.push_back(temp);
                    }
                    thereIs = false;
                    for(typename std::vector<Vertex*>::iterator x = (vert->myAdjacent).begin(); x != (vert->myAdjacent).end(); ++x) {
                        if(((*x) -> myData).compare(tokens.at(i)) == 0) thereIs = true;
                    }

                    //..finally establishing which are the arcs and their weights.
                    int Result;
                    std::istringstream convert(tokens.at(i+1));
                    if ( !(convert >> Result) )  Result = 0;
                    arc = new Arc(vert, temp, Result);
                    allArc.push_back(arc);
                    if(thereIs == false) {
                      (vert -> myAdjacent).push_back(temp);
                    }
                    i++;
                }
             }
             tokens.clear();
			 j ++;
             if(j == 3) j = 0;   
         }
    }
    file.close();

    //once closed the file, the method checks if datas were loaded correctly
    this -> vertices = allVertex;
    this -> arcs = allArc;
    if(this ->isEmpty() == true) {
        std::cout << "Nothing loaded\n";
    }
    else {
        std::cout << "Datas loaded\n";
    }
}

//Dijkstra algorithm for finding a shortest path (given two datas). Its complexity is O(Arcs*logVertices)
template <class Data>  void Graph<Data>::shortestPath(const Data data, const Data data1) {
    
    //Check for existence of the vertices with datas passed in input
    if(this -> isEmpty() == false) {

        Vertex* source;
        Vertex* destination;
        bool found = false;
        bool found1 = false;
        for(typename std::vector<Vertex*>::iterator x = (this -> vertices).begin(); x != (this->vertices).end(); ++x) {

            if((*x)->myData == data) {
                found = true;
                source = *x;
            }
            if((*x)->myData == data1) {
                found1 = true;
                destination = *x;
            }
        }
     	if(found == false || found1 == false) {
        	std::cout << "Data not found in graph\n";
        	return;
     	}

     	//if vertices exist, the method declares some data structures..
     	map<Vertex*,Vertex*> mySP;//Vertices of the shortest path
     	map<Vertex*,int> dist;//distances of all visited vertices from source
     	bool success;
     	myPQ = new priorityQueue<Vertex*,int>();//myPQ will contain this structure: key:=vertex, value:=distance from key

     	//Initialization of distances at "infinity (max of integers)", provided distance(source) = 0
     	for(typename std::vector<Vertex*>::iterator x = (this -> vertices).begin(); x != (this->vertices).end(); ++x) {
            if(*x != source) {
                dist.insert(std::make_pair((*x), std::numeric_limits<int>::max()));
            }
        }
        dist.insert(std::make_pair(source, 0));
        success = myPQ -> insert(dist);

        /*Dijkstra loop : once exctracted the minimum distance, look at the related vertex,
 		check among its adjacent vertices which one has to be "relaxed".. if the extracted 
 		is the vertex "destination", then we have got the path!
        */
        Vertex* u;                            
        int h;
        while(myPQ -> isEmpty() == false) {
            u = myPQ -> getMin();

            //the source is unconnected from the rest
            if(dist.at(u) == std::numeric_limits<int>::max()) {
            	std::cout << "Source is unconnected from the rest!"  << std::endl;
            	return;
            } 
            if(u == destination) break;//we have already got the minimum distance from u at each step of extraction..
            myPQ -> extractMin();

            /*proof:
            the minimum in the queue couldn't have less distance path, because other paths passing
            by other vertices are bigger or equal to it and so summing other weights till the found
            vertex, they would become still bigger than before!
            */
            for(typename std::vector<Vertex*>::iterator v = (u -> myAdjacent).begin(); v != (u->myAdjacent).end(); ++v) {
                
                //relaxing: if the algorithm finds another shorter way to a vertex that is still in queue, it updates it
                h = dist.at(u)+this->findArc(u,*v)->weight;
                if(h < dist.at(*v)) {
                    dist.erase(*v);
                    dist.insert(std::pair<Vertex*,int>(*v,h));
                    mySP[*v] = u;
                    myPQ -> increaseKey(*v, h);
                }
            }
        }

        //print of the shortest path by a reversing algorithm
        std::cout << "Shortest path created succesfully\n";
        std::cout << "Path:\n";
        std::vector<Data> print;
        Vertex* actual;
        if((destination) != source) {
            actual=destination;
            print.push_back(actual -> myData);
            while(mySP[actual] != source) {
                print.push_back(mySP[actual] -> myData);
                actual = mySP[actual];
            }
            print.push_back(source -> myData);
            for(typename std::vector<Data>::iterator y = (print).end()-1; y != (print).begin()-1; y--) {
                std::cout << (*y) << std::endl;
                std::cout << " -> " << std::endl;
            }
            std::cout << "Finished in " << std::endl;
            std::cout << dist.at(destination) << std::endl;
            std::cout << "\n" << std::endl;
            print.clear();
        }
    }
    else {
        std::cout << "There aren't vertices in graph\n";
    }
 
}

//Method for looking for an arc (given its pointer vertex and its pointed vertex)
template <class Data> typename Graph<Data>::Arc* Graph<Data>::findArc(const Vertex* first, const Vertex* second) {
	for(typename std::vector<Arc*>::iterator x = (this -> arcs).begin(); x != (this->arcs).end(); ++x) {
	    if(((*x)->first == first && (*x)->second == second)) return *x;
	}
}
#endif // GRAPH_H
