#include "NodeT.hpp"
using namespace std;
const bool NodeT::operator<(const NodeT& other) const {
	return f_score > other.f_score;
}
void NodeT::copy(NodeT* n){
	n->a = a;
	n->b = b;
	n->c = c;
	n->g_score = g_score;
	n->f_score = f_score;
	n->parent = parent;
}


NodeT::NodeT(vector< NodeT* > nodesT){
	g_score = 0;
	f_score = 0;
	nodesT->push_back(this);
}