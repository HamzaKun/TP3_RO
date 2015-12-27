#include "heuristique_insertion.h"


heuristique_insertion::heuristique_insertion(const Data & d)
	: WorkingSolution(d), c_free_(nodes_.size) {
	for (unsigned i = 0; i < nodes_.size; i++) {		//Ajout de tous les clients libres
		c_free_.push(&nodes_[i]);
	}
}


void heuristique_insertion::construction_par_insertion()
{
	// Tant qu'un client libre existe
	while (c_free_.size > 0) {
		//Init d'une tournee
		NodeInfo prec_node = depots_[0];														// Creation du client prec
		RouteInfo cur_route = open_specific_route(prec_node);			// Creation d'une tournee avec 1 client(depot)
		NodeInfo cur_node = *(c_free_.top);													// creation du client courant


		int i = 0;
		while (is_feasible(cur_node, cur_node.load, cur_node.arrival)									// Si possible
					&& data_.distance(prec_node.customer->id, cur_node.customer->id) <			// distance prec/cur
					data_.distance(cur_route.depot.customer->id, cur_node.customer->id)) {	// < distance depot/cur
			
			append(cur_route, cur_node);		// Ajout du client dans la tournee
			c_free_.pop();									// On vire le client de de la pile
			
			prec_node = cur_node;						// On incrémente
			cur_node = c_free_.top;					
		}
		close_route(cur_route);
	}
}