#include "rbfs.hpp"
#include <iostream>
#include "DomainT.hpp"
#include <vector>
#include <algorithm>
#include <limits> 

using namespace std;

int counter = 0;

vector<NodeT*> rbfs::get_path(DomainT* d, NodeT* current, double f_limit, int depth){
	counter += 1;

	string s;
	//cin >> s;
	NodeT* g = d->get_goal();
	vector< NodeT*> path;
	if(current->set_id().compare(g->set_id()) == 0){
		d->best_f_score = current->f_score;
		path.push_back(current);
		return path;
	}
	vector< NodeT*> successors = d->get_neighbors(current);
	if(successors.empty()){
		//d->best_f_score = numeric_limits<double>::max();
		d->best_f_score = 999.0;
		return path;
	}
	for (vector< NodeT* >::iterator s = successors.begin(); s != successors.end(); ++s) {
		// (*s)->g_score = min( (double)depth, (*s)->g_score );
		(*s)->g_score = (double)depth;
		double h = d->get_heuristic(*s);
		//(*s)->f_score = max((*s)->g_score + h, current->f_score);
		(*s)->f_score = max((*s)->g_score + h, (current->f_score));
	}
	comp_func c;

	while(true){
		sort(successors.begin(), successors.end(), c);
		// for (vector<NodeT*>::iterator n = successors.begin(); n != successors.end(); ++n){
		// 	cout << "\t" << (*n)->set_id() << "\t" << (*n)->f_score << endl;
		// }
		NodeT *best = successors[0];
		if(best->f_score > f_limit){
			d->best_f_score = best->f_score;
			return path;
		}
		double alt = successors[1]->f_score;
		vector<NodeT*> result = get_path(d, best, min(f_limit, alt), depth+1 );
		best->f_score = d->best_f_score;
		// cout << best->f_score << endl;
		if( best->f_score < 1){
			best->f_score = 999.0;
			// cout << best->f_score << endl;
		}
		
		if(! result.empty() ){
			result.insert(result.begin(),current);
			return result;
		}

		if(counter >= 10000){
			return path;
		}

	}
}
