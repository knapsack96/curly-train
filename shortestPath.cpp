//static linking
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <Graph.h>

/*
Application for finding shortest paths in a net.
For executing it, compile it with your last version
of c++ (after c++11). Ensure that you have included
Graph.h, Minheap.h and priorityQueue.h in your C++
libraries directory (example /usr/include/C++/6)
From shell: g++ -std=c++11 shortestPath.cpp -o shortestPath
(ensure having gnu g++ compiler).
From editor: make a project containing those files 
in include folder and build it as an executable.
Finally, in both cases, for loading datas inside, consult the 
net_example_file for writing the right format needed
by the program.
*/
using namespace std;
int main()
{
    std::cout << "MINIMAL PATH FROM A SOURCE TO A DESTINATION:\n";
    std::cout << "If you press 'a' - Populate your graph from file\n";
    std::cout << "If you press 'b' - Write the name of the source and the destination and get the shortestPath \n";
    std::cout << "If you press 'c' - Finish\n";
    Graph<string>* g = new Graph<string>();
    std::string c;
    cin >> c;
    while(c != "c") {
        if(c.size() > 1) cin >> c;
        switch(c[0]) {
            default: {
              break;
            }
            case('a'): {
              g -> populateGraph("net_example_file");
              break;
            }
            case('b'): {
              if(g->isEmpty()) {
                  std::cout << "Graph is empty\n";
                  break;
              }
              string line;
              std::cout << "Please, type name of source..\n";
              cin >> line;
              string get = line;
              getline(cin, line);
              string finalString = get + line;
              std::cout << "\nPlease, type name of destination..\n";
              cin >> line;
              get = line;
              getline(cin, line);
              string finalString1 = get + line;
              g -> shortestPath(finalString, finalString1);
              break;
            }
        }
        cin >> c;
    }
    return 0;
}
