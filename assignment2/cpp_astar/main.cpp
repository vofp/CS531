#include <iostream>
#include <vector>
#include "DomainT.hpp"
#include "NodeT.hpp"
#include "astar.hpp"
#include "rbfs.hpp"
#include <limits> 

using namespace std;

//Domain imports the map and defines functions such as the heuristic, cost between nodes, and neighbors. The node class is the definition of the different types of nodes, the nodes include their location and their g_score, and parent Node.
int main(int argc, char **argv){
	 DomainT Dom1;
	 Dom1.get_towers("1234");
	 cout << Dom1.nodesT[0]->set_id() << endl;

	 //Dom1.get_nodes();
	 // astar astar1;
	 cout << endl << "BEFORE GET PATH" << endl;
	 vector< NodeT*> path;
	 // path = astar1.get_path(Dom1);
	 rbfs rbfs1;
	 path = rbfs1.get_path(Dom1, Dom1.get_start(), numeric_limits<double>::max());
	 cout << path.back()->set_id() << endl;
	 return 0;
}
