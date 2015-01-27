#ifndef DOMAIN_T_HPP
#define DOMAIN_T_HPP
#include "NodeT.hpp"
#include "My_map.hpp"
#include <iostream> 
using namespace std;

class Domain2d{
	public:
		//void get_map();
		int get_tower_size();
		vector< NodeT > nodesT;
		void get_nodes();
		vector<NodeT*> get_neighbors(NodeT*);
		double get_heuristic(NodeT*);
		double get_cost();
		NodeT* get_start();
		NodeT* get_goal();
		//My_map map;

};

#endif
