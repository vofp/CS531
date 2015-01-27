#include "Node2d.hpp"
const bool Node2d::operator<(const Node2d& other) const {
	return f_score > other.f_score;
}
