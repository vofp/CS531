#ifndef ASTAR_HPP
#define ASTAR_HPP
#include "DomainT.hpp"

struct comp_func{
	bool operator()( const NodeT* a, const NodeT* b){
		return(a->f_score< b->f_score);
	}
};


class astar{
	public:
		//vector< NodeT* > path;
		vector< NodeT*>  get_path(DomainT);

		//astar(void);
		//~astar(void);
};

#endif
