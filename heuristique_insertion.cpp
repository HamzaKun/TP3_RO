#include "heuristique_insertion.h"


heuristique_insertion::heuristique_insertion(const Data & d)
	: WorkingSolution(d), c_free_() {
	for (unsigned i = 0; i < nodes_.size(); i++) {		//Ajout de tous les clients libres
		c_free_.push(nodes_[i]);
	}
}

void heuristique_insertion::construction_par_insertion()
{
	// Tant qu'un client libre existe
	while (c_free_.size() > 0) {
		//Init d'une tournee
				
		RouteInfo& cur_route = open_specific_route(top());					// creation de la route
			
		NodeInfo& prec_node = top();																//Definition du prec
		pop();

		if (c_free_.size() <= 0) break;

		NodeInfo& cur_node = top();																	// creation du client courant
		cur_node.route = &cur_route;																// lien cur_node/route

		int i = 0;
		while (is_feasible(cur_node, cur_node.load, cur_node.arrival)									// Si possible
					&& data_.distance(prec_node.customer->id(), cur_node.customer->id()) <=			// distance prec/cur
					data_.distance(cur_route.depot.customer->id(), cur_node.customer->id())) {	// < distance depot/cur
			
			append(cur_route, cur_node);		// Ajout du client dans la tournee	
			pop();

			if (c_free_.size() <= 0) break;
			
			prec_node = cur_node;					// On incrémente
			cur_node = top();
			cur_node.route = &cur_route;
		}
	}
}