#ifndef DOMAIN_T_HPP
#define DOMAIN_T_HPP
#include "NodeT.hpp"
#include <vector>
#include <iostream> 
using namespace std;

class DomainT{
	public:
		//void get_map();
		int get_towers_size();
		void get_towers(string);
		vector< NodeT* > nodesT;
		vector< NodeT* > get_neighbors(NodeT*);
		double get_heuristic(NodeT*);
		double get_cost();
		NodeT* get_start();
		NodeT* get_goal();
		//My_map map;

};

#endif
