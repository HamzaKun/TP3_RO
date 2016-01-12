#include "heuristique_insertion.h"


heuristique_insertion::heuristique_insertion(const Data & d)
	: WorkingSolution(d), c_free_() {
	for (unsigned i = 1; i < nodes_.size(); i++) {		// Ajout de tous les clients libres - le depot
		c_free_.push_back(&nodes_[i]);
	}

	// TODO : mélanger c_free_
}

void heuristique_insertion::construction_par_insertion()
{
	// Tant qu'un client libre existe
	while (c_free_.size()>0) {

		// Init d'une tournee
		NodeInfo* prec_node = c_free_.front();														//Creation du premier point (hasard, a ameliorer)
		c_free_.erase(c_free_.begin());

		RouteInfo& cur_route = open_specific_route(*prec_node);			// Création de la route
		
		cur_route.depot.name = "depot";

		prec_node->name = "first";

		if (c_free_.size() <= 0) break;

		NodeInfo* cur_node = recherche_meilleur_client(prec_node);
		cur_node->route = &cur_route;																// lien cur_node/route
		cur_node->name = "second";

		while (is_feasible(*cur_node, cur_node->load, cur_node->arrival)									// Si possible
					&& data_.distance(prec_node->customer->id(), cur_node->customer->id()) <=			// distance prec/cur
					data_.distance(cur_route.depot.customer->id(), cur_node->customer->id())) {	// < distance depot/cur
			
			//insert(*(cur_route.depot.prev), cur_node);		// Ajout du client dans la tournee	
			append(cur_route, *cur_node);

			if (c_free_.size() <= 0) break;
			
			prec_node = cur_node;				// On incrémente
			cur_node = recherche_meilleur_client(prec_node);
			cur_node->route = &cur_route;
			cur_node->name = "next";
		}
	}
}

NodeInfo* heuristique_insertion::recherche_meilleur_client(NodeInfo* cur_node_fix) {
	Time time_cur_node;

	//Init
	int index_best_node = 0;
	NodeInfo* best_node=c_free_[index_best_node];
	Time time_best_node = data_.distance(best_node->customer->id(), cur_node_fix->customer->id());

	for (int i = 0;i < c_free_.size(); i++) {
		time_cur_node = data_.distance(cur_node_fix->customer->id(), c_free_[i]->customer->id());
		if (time_cur_node < time_best_node) {
			best_node = c_free_[i];
			time_best_node = time_cur_node;
			index_best_node = i;
		}
	}

	c_free_.erase(c_free_.begin()+index_best_node);

	return best_node;
}
