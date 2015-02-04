#ifndef NODE_T_HPP
#define NODE_T_HPP
#include <vector>
#include <string>
#include <limits> 
using namespace std;

class NodeT{
	public:
		vector< int > a;
		vector< int > b;
		vector< int > c;
		double g_score, f_score;
		const bool operator<(const NodeT& other) const;
		NodeT* parent;
		NodeT();
		void copy(NodeT*);
		string set_id();
		// string id;
		// bool same_state(vector< int >,vector< int >,vector< int >);
};

#endif
