#include "rbfs.hpp"
#include <iostream>
#include "DomainT.hpp"
#include <vector>
#include <algorithm>
#include <limits> 

using namespace std;

vector<NodeT*> rbfs::get_path(DomainT d, NodeT* current, double f_limit){
	NodeT* g = d.get_goal();
	vector< NodeT*> path;
	if(current->id.compare(g->id)){
		d.best_f_score = current->f_score;
		path.push_back(current);
		return path;
	}
	vector< NodeT*> successors = d.get_neighbors(current);
	if(successors.empty()){
		d.best_f_score = numeric_limits<double>::max();
		return path;
	}
	for (vector< NodeT* >::iterator s = successors.begin(); s != successors.end(); ++s) {
		(*s)->f_score = max((*s)->g_score + d.get_heuristic(*s), current->f_score);
	}
	comp_func c;
	sort(successors.begin(), successors.end(), c);

	while(true){
		NodeT *best = successors[0];
		if(best->f_score > f_limit){
			d.best_f_score = best->f_score;
			return path;
		}
		double alt = successors[1]->f_score;
		vector<NodeT*> result = get_path(d, best, min(f_limit, alt) );
		best->f_score = d.best_f_score;
		if(! result.empty() ){
			result.insert(result.begin(),current);
			return result;
		}
	}

}