#include "heuristique_insertion.h"


heuristique_insertion::heuristique_insertion(const Data & d)
	: WorkingSolution(d), c_free_() {
	for (unsigned i = 0; i < nodes_.size(); i++) {		//Ajout de tous les clients libres
		c_free_.push(& nodes_[i]);
	}
}


void heuristique_insertion::construction_par_insertion()
{
	// Tant qu'un client libre existe
	int i;
	while (c_free_.size() > 0) {
		//Init d'une tournee
		depots_.push_back(NodeInfo());															// Ajout de depot de la route
		NodeInfo& prec_node = depots_[i];														// Creation du client prec
		i++;																												// inc ce l'id

		routes_.push_back(open_specific_route(prec_node));										// ajout de la route
		RouteInfo& cur_route = routes_.back();													// pt courant

		prec_node.route = &cur_route;															// lien node/route

		NodeInfo* cur_node = c_free_.top();														// creation du client courant
		cur_node->route = &cur_route;															// lien node/route

		int i = 0;
		while (is_feasible(*cur_node, cur_node->load, cur_node->arrival)						// Si possible
					&& data_.distance(prec_node.customer->id(), cur_node->customer->id()) <		// distance prec/cur
					data_.distance(cur_route.depot.customer->id(), cur_node->customer->id())) {	// < distance depot/cur
			
			append(cur_route, *cur_node);														// Ajout du client dans la tournee
			c_free_.pop();																		// On vire le client de de la pile
			
			prec_node = *cur_node;																// On incrémente
			cur_node = c_free_.top();
			cur_node->route = &cur_route;
		}
		close_route(cur_route);
	}
}