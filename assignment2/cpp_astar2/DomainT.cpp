#include "DomainT.hpp"
#include "math.h"
#include <iostream>
using namespace std;

void DomainT::get_towers_size(){
	return size;
}

void DomainT::get_towers(String s){
	NodeT *new_node = new NodeT;
	for(int i = length(s); s >= 0; s--){
		NodeT->a.push_back(atoi(s[i]));
	}
	nodesT.push_back(new_node)
}

NodeT* DomainT::get_start(){
	return nodesT[0];
}

NodeT* DomainT::get_goal(){
	NodeT *goal = new NodeT;
	
	return &nodes[];
}


vector<NodeT*> DomainT::get_neighbors(NodeT* current){
	vector<NodeT*> neighbors;
	if(!current->a.empty()){
		NodeT* new_node1 = new NodeT;
		NodeT* new_node2 = new NodeT;
		new_node1 = copy(current);
		int ring = new_node1->a.end();
		new_node1->b.push_back(ring);
		new_node2->c.push_back(ring);
		neighbors.push_back(new_node1);
		neighbors.push_back(new_node2);
	}
	if(!current->b.empty()){
		NodeT* new_node1 = new NodeT;
		NodeT* new_node2 = new NodeT;
		new_node1 = copy(current);
		int ring = new_node1->b.end();
		new_node1->a.push_back(ring);
		new_node2->c.push_back(ring);
		neighbors.push_back(new_node1);
		neighbors.push_back(new_node2);
	}
	if(!current->c.empty()){
		NodeT* new_node1 = new NodeT;
		NodeT* new_node2 = new NodeT;
		new_node1 = copy(current);
		int ring = new_node1->c.end();
		new_node1->a.push_back(ring);
		new_node2->b.push_back(ring);
		neighbors.push_back(new_node1);
		neighbors.push_back(new_node2);
	}
	return neighbors;
}

double DomainT::get_heuristic(NodeT* current){

	return heuristic;
}

double DomainT::get_cost(NodeT* curr, NodeT* neigh){
	return 1;
}

