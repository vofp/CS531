//Astar
//author: Ryan Skeele
#include "astar.hpp"
#include <iostream>
#include "DomainT.hpp"
#include <vector>
#include <algorithm>
//#include <queue>
//#include <math.h>

using namespace std;
/*
astar::astar()
{
	cout<<"Astar Constructed"<<endl;
}

astar::~astar(){

	cout<<"Astar Destroyed"<<endl;
}
*/

vector<NodeT*> astar::get_path(DomainT* d){
	 // cout << "help" << endl;
	 NodeT* start;
	 NodeT* goal;
	 start = d->get_start();
	 goal = d->get_goal();
	 vector<NodeT*> closed_set;
	 vector<NodeT*> open_set;
	 start->g_score = 0;
	 start->f_score = start->g_score + d->get_heuristic(start);
	 // cout << start->f_score << endl;
	 open_set.push_back(start);
	 // cout << endl << open_set[0] << endl;
	 NodeT* current;
	 // cout << "BEFORE ASTAR" << endl;
	 int timeout = 0;
	 int iter = 0;
	 while(open_set.size()!=0 && timeout < 10000){
	 	comp_func c;
	 	sort(open_set.begin(), open_set.end(), c);
	 	current = open_set[0];
	 	//cout << endl << current->x << "    " << current->y<< endl ;
	 	if(current->set_id().compare(goal->set_id()) == 0){
	 		// cout << endl << "GOALLL"<< endl;
	 		// cout << endl << "Loop Count :" << timeout << endl;
	 		vector< NodeT*> path;
	 		while(current->set_id().compare(d->get_start()->set_id()) != 0){
	 			//cout << "PATH" << current << endl;
	 			//cout << endl << current->x << "    " << current->y<< endl ;
	 			path.push_back(current);
	 			current = current->parent;
	 		}
			
	 		// for(int i = 0; i < path.size(); i++){
	 		// 	cout << endl << path[i]->set_id() << endl;
	 		// }
			// cout << endl << start->set_id() << endl;
	 		// cout << endl << "PATH LENGTH :" << path.size() << endl;
	 		return path;
	 	}
	 	open_set.erase(open_set.begin());
	 	closed_set.push_back(current);
	 	vector<NodeT*> neighbors;
	 	neighbors = d->get_neighbors(current);
	 	for(vector<NodeT*>::iterator i = neighbors.begin(); i< neighbors.end(); ++i){
	 		if(find(closed_set.begin(), closed_set.end(), *i)!= closed_set.end()){
	 			continue;
	 		}
	 		double tentative_g_score = current->g_score + d->get_cost();
	 		bool in_open_set(find(open_set.begin(), open_set.end(), *i)!= open_set.end());
	 		if(!in_open_set || tentative_g_score < (*i)->g_score){
	 			(*i)->parent = current;
	 			(*i)->g_score = tentative_g_score;
	 			(*i)->f_score = (*i)->g_score + d->get_heuristic(*i);
	 			if(!in_open_set){
	 				open_set.push_back(*i);
	 				iter ++;
	 				// cout << endl << "NODES EXPANDED  " << iter << "\t "<< current->set_id() << endl;
	 			}
	 		}
	 	}
	 	timeout++;
	 	//cout << endl << open_set[0];	
	 }
	 // cout << "OUT OF PATH";	
}

