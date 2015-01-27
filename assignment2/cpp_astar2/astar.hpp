#ifndef ASTAR_HPP
#define ASTAR_HPP
#include "Domain2d.hpp"

struct comp_func{
	bool operator()( const Node2d* a, const Node2d* b){
		return(a->f_score< b->f_score);
	}
};


class astar{
	public:
		//vector< Node2d* > path;
		vector< Node2d*>  get_path(Domain2d);
		

		//astar(void);
		//~astar(void);
};

#endif
