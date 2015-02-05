#ifndef DOMAIN_T_HPP
#define DOMAIN_T_HPP
#include "NodeT.hpp"
#include <vector>
#include <iostream> 
#include <string> 
using namespace std;

struct comp_func{
	bool operator()( const NodeT* a, const NodeT* b){
		return(a->f_score< b->f_score);
	}
};

class DomainT{
	public:
		//void get_map();
		double best_f_score;
		int get_towers_size();
		int tower_size;
		void get_towers(string);
		vector< NodeT* > nodesT;
		vector< NodeT* > get_neighbors(NodeT*);
		double get_heuristic(NodeT*);
		double get_heuristic2(NodeT*);
		double get_cost();
		NodeT* get_start();
		NodeT* get_goal();
		NodeT* goal;
		NodeT* find_or_create(string);
		NodeT* find(string);
		NodeT* create(string);
		//My_map map;

};

#endif
