#include "DomainT.hpp"
#include "math.h"
#include <iostream>
using namespace std;

int DomainT::get_towers_size(){
	return 0;
}

void DomainT::get_towers(string s){
	NodeT *new_node = new NodeT(&this->nodesT);
	for(int i = s.size(); i >= 0; i--){
		new_node->a.push_back(atoi(s[i]+""));
	}
	// nodesT.push_back(new_node);
}

NodeT* DomainT::get_start(){
	return nodesT[0];
}

NodeT* DomainT::get_goal(){
	NodeT *goal = new NodeT(&this->nodesT);
	return goal;
}


vector<NodeT*> DomainT::get_neighbors(NodeT* current){
	vector<NodeT*> neighbors;
	if(!current->a.empty()){
		NodeT* new_node1 = new NodeT(&this->nodesT);
		NodeT* new_node2 = new NodeT(&this->nodesT);
		current->copy(new_node1);
		current->copy(new_node1);
		int ring = new_node1->a.back();
		new_node1->b.push_back(ring);
		new_node2->c.push_back(ring);
		neighbors.push_back(new_node1);
		neighbors.push_back(new_node2);
	}
	if(!current->b.empty()){
		NodeT* new_node1 = new NodeT(&this->nodesT);
		NodeT* new_node2 = new NodeT(&this->nodesT);
		current->copy(new_node1);
		current->copy(new_node1);
		int ring = new_node1->b.back();
		new_node1->a.push_back(ring);
		new_node2->c.push_back(ring);
		neighbors.push_back(new_node1);
		neighbors.push_back(new_node2);
	}
	if(!current->c.empty()){
		NodeT* new_node1 = new NodeT(&this->nodesT);
		NodeT* new_node2 = new NodeT(&this->nodesT);
		current->copy(new_node1);
		current->copy(new_node1);
		int ring = new_node1->c.back();
		new_node1->a.push_back(ring);
		new_node2->b.push_back(ring);
		neighbors.push_back(new_node1);
		neighbors.push_back(new_node2);
	}
	return neighbors;
}

double DomainT::get_heuristic(NodeT* current){
	double h = 0.0;
	for(unsigned int i = 0; i < current->a.size(); ++i) {
		for (unsigned int j = i+1; j < current->a.size(); ++j)
		{
			if(current->a[i] < current->a[j]) {
				h += 1;
			}
			h += current->a[i];
		}
	}
	for(unsigned int i = 0; i < current->b.size(); ++i) {
		for (unsigned int j = i+1; j < current->b.size(); ++j)
		{
			if(current->b[i] < current->b[j]) {
				h += 1;
			}
			h += current->b[i];
		}
	}
	for(unsigned int i = 0; i < current->c.size(); ++i) {
		for (unsigned int j = i+1; j < current->c.size(); ++j)
		{
			if(current->c[i] != (signed)i+1) {
				if(current->c[i] < current->c[j]) {
					h += 1;
				}
				h += current->c[i];
			}
		}
	}
	return h;
}

double DomainT::get_cost(){
	return 1;
}

