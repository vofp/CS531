#ifndef NODE_T_HPP
#define NODE_T_HPP
class NodeT{
	public:
		vector< int > a;
		vector< int > b;
		vector< int > c;
		double g_score, f_score;
		const bool operator<(const NodeT& other) const;
		NodeT* parent;
};

#endif
