#include "DomainT.hpp"
#include "math.h"
#include <iostream>
using namespace std;

int DomainT::get_towers_size(){
	return 0;
}

void DomainT::get_towers(string s){
	NodeT *new_node = new NodeT();
	for(int i = s.size()-1; i >= 0; --i){
		int n = s[i]-'0';
		// cout << n << endl;
		new_node->a.push_back(n);
	}
	new_node->set_id();
	nodesT.push_back(new_node);

	NodeT *new_node2 = new NodeT();
	for(unsigned int i = 0; i < s.size(); i++){
		new_node2->c.push_back(i);
	}
	// cout << new_node2->set_id() << endl;
	// nodesT.push_back(new_node2);
	goal = new_node2;
}

NodeT* DomainT::get_start(){
	return nodesT[0];
}

NodeT* DomainT::get_goal(){
	return goal;
}

NodeT* DomainT::find_or_create(string s){
	NodeT* n = find(s);
	if (n==NULL){
		return create(s);
	}
	return n;
}

NodeT* DomainT::find(string s){
	for (vector<NodeT*>::iterator n = nodesT.begin(); n != nodesT.end(); ++n){
		if ((*n)->id.compare(s) == 0 ){
			return *n;
		}
	}
	return NULL;
}

NodeT* DomainT::create(string s){
	cout << "create " << s << endl;
	NodeT *new_node = new NodeT();
	unsigned int i = 0;
	while(i < s.size() && s[i] != '.'){
		int n = s[i]-'0';
		new_node->a.push_back(n);
		i += 1;
	}
	i += 1;
	while(i < s.size() && s[i] != ','){
		int n = s[i]-'0';
		new_node->b.push_back(n);
		i += 1;
	}
	i += 1;
	while(i < s.size()){
		int n = s[i]-'0';
		new_node->c.push_back(n);
		i += 1;
	}
	new_node->set_id();
	nodesT.push_back(new_node);
	return new_node;
}


vector<NodeT*> DomainT::get_neighbors(NodeT* current){
	vector<NodeT*> neighbors;
	string s3;
	// cin >> s3;
	if(!current->a.empty()){
		// cout << "a is not empty" << endl;
		string s (current->id);
		std::size_t found = s.find('.');
		if (found!=std::string::npos){
			// cout << "found . at " << found << endl;
			char c = s[found-1];
			s.erase(found-1,1);
			string s2 (s);
			// cout << s << endl;
			found = s.find(',');
			if (found!=std::string::npos){
				// cout << "found , at " << found << endl;
				s.insert(s.begin()+found,c);
				s2.insert(s2.end(),c);

				// cout << "\t\t" << s << "\t" << s2 << endl; 

				neighbors.push_back(find_or_create(s));
				neighbors.push_back(find_or_create(s2));
			}
		}
	}
	if(!current->b.empty()){
		string s (current->id);
		std::size_t found = s.find(',');
		if (found!=std::string::npos){
			char c = s[found-1];
			s.erase(found-1,1);
			string s2 (s);
			found = s.find('.');
			if (found!=std::string::npos){
				s.insert(s.begin()+found,c);
				s2.insert(s2.end(),c);

				neighbors.push_back(find_or_create(s));
				neighbors.push_back(find_or_create(s2));
			}
		}
	}
	if(!current->c.empty()){
		string s (current->id);
		char c = s.back();
		s.erase(s.end());
		std::size_t found = s.find('.');
		if (found!=std::string::npos){
			string s2 (s);
			s.insert(found,c+"");
			neighbors.push_back(find_or_create(s));
			found = s2.find(',');
			if (found!=std::string::npos){
				s2.insert(found,c+"");
				neighbors.push_back(find_or_create(s2));
			}
		}
	}
	cout << "neighbors of " << current->set_id() << " :" << endl;
	for (vector<NodeT*>::iterator n = neighbors.begin(); n != neighbors.end(); ++n){
		cout << "\t" <<(*n)->set_id() << endl;
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

