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
	DomainT Dom2;
	string s = argv[1];
	Dom1.get_towers(s);
	Dom2.get_towers(s);
	// cout << Dom1.get_start()->set_id() << endl;
	//Dom1.get_nodes();
	// cout << endl << "BEFORE GET PATH" << endl;

	clock_t t1;
	clock_t t2;
	vector< NodeT*> path1;
	vector< NodeT*> path2;
	
	t1 = clock();
	astar astar1;
	path1 = astar1.get_path(&Dom1);
	t1 = clock() - t1;

	t2 = clock();
	rbfs rbfs1;
	path2 = rbfs1.get_path(&Dom2, Dom2.get_start(), numeric_limits<double>::max(),0);
	t2 = clock() - t2;


	cout << s << ", " << s.size() << ", " << Dom1.nodesT.size() << ", " << path1.size() << ", " << t1 << ", " << Dom2.nodesT.size() << ", " << path2.size() << ", " << t2 << endl;
	// printf ("It took me %d clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);

	return 0;
}
