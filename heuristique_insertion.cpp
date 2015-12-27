#include "heuristique_insertion.h"


heuristique_insertion::heuristique_insertion(const Data & d)
	: WorkingSolution(d), c_free_(nodes_.size) {
	for (unsigned i = 0; i < nodes_.size; i++) {
		c_free_.push(nodes_[i]);
	}
}

heuristique_insertion::~heuristique_insertion() { }

void heuristique_insertion::c_free(NodeInfo & n)
{
	c_free_.push(n);
}

void heuristique_insertion::construction_par_insertion()
{
	// Tant qu'un client libre existe
	while (c_free_.size > 0) {
		Id id = 0;
		NodeInfo & cur_node = c_free_.top;
		RouteInfo & cur_route = open_route();

		int i = 0;
		while (is_feasible(cur_node, cur_node.load, cur_node.arrival)) {
			append(cur_route, cur_node);
			c_free_.pop();
			cur_node = c_free_.top;
		}
	}
}

void heuristique_insertion::insertion(Customer * new_client)
{

}
