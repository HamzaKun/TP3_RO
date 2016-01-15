#include "heuristique_insertion.h"

heuristique_insertion::heuristique_insertion(const Data & d)
	: WorkingSolution(d), c_free_() {
	
	depot_ = &nodes_[0];
	for (unsigned i = 1; i < nodes_.size(); i++) {		// Ajout de tous les clients libres - le depot
		nodes_[i].dist_from_dep = data_.distance(depot_->customer->id(), nodes_[i].customer->id());
		c_free_.push_back(&nodes_[i]);
	}
	
	std::sort(c_free_.begin(), c_free_.end(), compare_to_depot);

	//for each(NodeInfo* cli in c_free_) {
	//	std::cout << cli->dist_from_dep << std::endl;
	//}
}

void heuristique_insertion::construction_par_insertion()
{
	// Tant qu'un client libre existe
	while (c_free_.size()>0) {

		// Init d'une tournee
		NodeInfo* prec_node = c_free_[0];		// On prend l'element le plus pret du depot
		c_free_.erase(c_free_.begin());
		
		RouteInfo& cur_route = open_specific_route(*prec_node);			// Création de la route
		
		if (DEBUG)
			std::cout << "route n" << cur_route.id << std::endl;

		cur_route.depot.name = "depot";

		prec_node->name = "first";

		if (c_free_.size() <= 0) break;

		NodeInfo* cur_node= new NodeInfo;
		
		while (recherche_meilleur_client(prec_node, std::max(prec_node->arrival,prec_node->customer->open()), prec_node->load, cur_node)) {	// tant que l'on trouve un client
			
			append(cur_route, *cur_node);

			if (c_free_.size() <= 0) break;
			
			cur_node->name = "next";
			prec_node = cur_node;				// On incrémente
			cur_node = new NodeInfo;
		}
	}
	// On met a jour nodes_
	for (RouteInfo* r = first_; r != nullptr;r=r->next_)
	{
		for (NodeInfo* n = r->depot.next; n->customer->id() != r->depot.customer->id(); n=n->next)
		{
			nodes_[n->customer->id()] = *n;
		}
	}
}

// TODO Il faudrait renvoyer un booleen avec cette fonction vrai si une nouvel ele est trouve faux sinon et stocke ledit ele dans un parametre
bool heuristique_insertion::recherche_meilleur_client(NodeInfo* last_node, Time distance_already_run, Load load_charged, NodeInfo* res) {
	bool node_trouve = false;
	Time cur_distance;
	
	//Selection des nodes disponibles
	std::vector<std::pair<NodeInfo*,int>>  node_dispo; //Vecteur de NodeInfo et de leur indice dans le vecteur

	if(DEBUG)
		std::cout << "New node test : " << last_node->name << " " << last_node->dist_from_dep << " " << last_node->customer->id() << " " << distance_already_run << std::endl;
	
	for (int i = 0;i < c_free_.size(); i++){
		cur_distance = data_.distance(last_node->customer->id(), c_free_[i]->customer->id());
		if (distance_already_run + cur_distance > c_free_[i]->customer->open() && distance_already_run + cur_distance < c_free_[i]->customer->close() &&		//Temps d'arrivee
			(load_charged + c_free_[i]->customer->demand() <= data_.fleetCapacity())) {																						// Capacite
			
			node_dispo.push_back(std::pair<NodeInfo*, int>(c_free_[i],i));
			
			if (DEBUG)
				std::cout << c_free_[i]->customer->open() << " " << distance_already_run + cur_distance << " " << c_free_[i]->customer->close() << " id:" << c_free_[i]->customer->id() << std::endl;
		}
	}

	//Si des points sont disponibles -> test point plus pret
	if (!node_dispo.empty()) {
		node_trouve = true;

		Time time_cur_node;
		//Init
		int index_best_node = node_dispo[0].second;
		NodeInfo* best_node = node_dispo[0].first;
		Time time_best_node = data_.distance(best_node->customer->id(), last_node->customer->id());
		
		for (int i = 0;i < node_dispo.size(); i++) {
			time_cur_node = data_.distance(node_dispo[i].first->customer->id(), last_node->customer->id());
			if (time_cur_node < time_best_node) {																					//Si meilleur temps
				best_node = node_dispo[i].first;
				index_best_node = node_dispo[i].second;
				time_best_node = time_cur_node;
			}
		}

		//On donne le pointeur a res et on le supprime de c_free_
		*res = *best_node;
		c_free_.erase(c_free_.begin() + index_best_node);
	}
	return node_trouve;
}

bool compare_to_depot(NodeInfo* client1, NodeInfo* client2) {
	return (client1->dist_from_dep+client1->customer->open()) < (client2->dist_from_dep+client2->customer->open());
}