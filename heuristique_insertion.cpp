#include "heuristique_insertion.h"


heuristique_insertion::heuristique_insertion(const Data & d)
	: WorkingSolution(d), c_free_() {
	for (unsigned i = 0; i < nodes_.size(); i++) {		// Ajout de tous les clients libres
		c_free_.push(nodes_[i]);
	}

	// TODO : mélanger c_free_
}

void heuristique_insertion::construction_par_insertion()
{
	// Tant qu'un client libre existe
	int i = 1;															// 0 = dépôt
	while (i < nodes_.size()) {

		// Init d'une tournee
		NodeInfo& prec_node = nodes_[i++];								// Definition du prec

		prec_node = nodes_[i++];										// Definition du prec

		if (prec_node.customer->id() == data().depot())
			std::cout << "Lapin !" << std::endl;

		RouteInfo& cur_route = open_specific_route(prec_node);			// Création de la route
		
		cur_route.depot.name = "depot";

		prec_node.name = "first";

		if (i >= nodes_.size()) break;

		NodeInfo& cur_node = nodes_[i++];																	// creation du client courant
		cur_node.route = &cur_route;																// lien cur_node/route
		cur_node.name = "second";

		int i = 0;
		while (is_feasible(cur_node, cur_node.load, cur_node.arrival)									// Si possible
					&& data_.distance(prec_node.customer->id(), cur_node.customer->id()) <=			// distance prec/cur
					data_.distance(cur_route.depot.customer->id(), cur_node.customer->id())) {	// < distance depot/cur
			
			append(cur_route, cur_node);		// Ajout du client dans la tournee	

			if (i >= nodes_.size()) break;
			
			prec_node = cur_node;				// On incrémente
			cur_node = nodes_[i++];
			cur_node.route = &cur_route;
			cur_node.name = "next";
		}
	}
}