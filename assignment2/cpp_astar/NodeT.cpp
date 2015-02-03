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


NodeT::NodeT(){
	g_score = 0;
	f_score = 0;
	// nodesT->push_back(this);
}

string NodeT::set_id(){
	string s("");
	for (vector<int>::iterator i = a.begin(); i != a.end(); ++i){
		s.append(to_string(*i));
	}
	s.push_back('.');
	for (vector<int>::iterator i = b.begin(); i != b.end(); ++i){
		s.append(to_string(*i));
	}
	s.push_back(',');
	for (vector<int>::iterator i = c.begin(); i != c.end(); ++i){
		s.append(to_string(*i));
	}
	id = s;
	return id;
}
// bool NodeT::same_state(vector< int > a,vector< int > b,vector< int > c){
// 	if(a.size() != this.a.size() &&
// 			b.size() != this.b.size() &&
// 			c.size() != this.c.size() ) {
// 		return false;
// 	}

// 	for (int i = 0; i < a.size(); ++i){
// 		if(a.size)
// 	}

// }