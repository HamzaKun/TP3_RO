#include "recherche_locale.h"


recherche_locale::recherche_locale(WorkingSolution& ws) : ws_(ws)
{
	//dabord cas particulier 2opt* et oropt
	//puis 2opt* et cross
	bool fin = false;

	while (!fin) {
		if (!two_opt_etoile_cp()) {
			if (!ot_opt_cp()) {
				if (!two_opt_etoile()) {
					if (!ot_opt()) {
						if (!cross()) {
							fin = true;
						}
					}
				}
			}
		}
	}

	/*recherche_node_fenetre_temps(ws_.nodes()[1]);
	std::vector<RouteInfo*> vect_r = get_all_unique_route();*/

	//std::cout << "Appel get_all_unique_route" << std::endl;
	//for (int i = 0;i < vect_r.size();i++) {
	//	vect_r[i]->display();
	//}
	
	//si ca ca marche pas (ce qui est probable)
	//while (two_opt_etoile_cp()) {}
	//while (ot_opt_cp()) {}
	//while (two_opt_etoile()) {}
	//while (ot_opt()) {}
	//while (cross()) {}
}

bool recherche_locale::two_opt_etoile_cp() {
	//Ajout si possible de 2 tournees
	bool retour = false;														// Return false par défaut

	WorkingSolution new_w(ws_);

	NodeInfo * x_node;
	NodeInfo * y_depot;
	NodeInfo * y_first_node;

	// Parcours des tournees
	for (RouteInfo * x = new_w.first(); x != nullptr; x = x->next_) {		// Pour chaque route
		for (RouteInfo * y = new_w.first(); y != nullptr; y = y->next_) {	// Pour chaque autre route
			if (x != y) {													// Si on a pas affaire à la même route
				x_node = x->depot.prev;
				y_depot = &(x->depot);
				y_first_node = y_depot->next;

				// Vérification de la charge
				if (x_node->load + y_depot->load < new_w.data().fleetCapacity()) {
					// Vérification de la fenêtre de temps : calcul de la distance
					Time arrivalTime = x_node->arrival + new_w.data().distance(x_node->customer->id(), y_first_node->customer->id());
					
					if ( (arrivalTime < y_first_node->customer->close()) && (arrivalTime > y_first_node->customer->open())) {
						new_w.append((*x), (*y_first_node));
						ws_ = new_w;										// Nouvelle solution
						retour = true;
						std::cout << "Amélioration faite !" << std::endl;
			}
		}
	}
}
	}
	return retour;
}

bool recherche_locale::two_opt_etoile() {
	
	bool retour = false;													// Par défaut on retourne faux
	WorkingSolution new_w(ws_);												// Création d'une nouvelle solution

	for each (NodeInfo a in new_w.nodes())
	{
		for each (NodeInfo b in new_w.nodes())
		{
			// TODO : tester qu'on est pas au dépôt
			if (a.route != b.route) {										// On vérifie que a et b ne soient pas sur la même route
				// Premier gain potentiel : (a -> a') - (a -> b')
				Time a_b2 = new_w.data().distance(a.customer->id(), b.next->customer->id());
				Time b_a2 = new_w.data().distance(b.customer->id(), a.next->customer->id());

				Time gain = new_w.data().distance(a.customer->id(), a.next->customer->id()) - a_b2;
				// Second gain potentiel : (b -> b') - (b -> a')
				gain += new_w.data().distance(b.customer->id(), b.next->customer->id()) - b_a2;
				if (gain > 0) {
					// Vérification de la charge :
					Load c1 = b.route->depot.load - b.load + a.load;
					Load c2 = a.route->depot.load - a.load + b.load;
					if (c1 < new_w.data().fleetCapacity() && c2 < new_w.data().fleetCapacity()) {
						// Vérification de la fenêtre de temps, premier changement :
						if ((a.arrival + a_b2 < b.next->customer->close()) && (a.arrival + a_b2 > b.next->customer->open())) {
							if ((b.arrival + b_a2 < a.next->customer->close()) && (b.arrival + b_a2 > a.next->customer->open())) {
								// On fait le 2-opt *
								// Permutation
								NodeInfo * tmp = a.next;					// On fait une permutation, obligé d'utiliser une variable temporaire
								a.next = b.next;
								b.next = tmp;

								// Update des données :
								new_w.update2(a);
								new_w.update2(b);

								ws_ = new_w;								// On garde la nouvelle solution
								retour = true;
							}
						}
					}
				}
			}
		}
	}

	return retour;
}

bool recherche_locale::ot_opt_cp() {

	bool retour = false;

	// Insertion d'un point seul dans une tournee dans une autre tournee
	std::vector<RouteInfo*> vect_road_clients_uniques = get_all_unique_route();

	// Parcours des clients seuls
	for (int i = 0;i < vect_road_clients_uniques.size();i++) {
		std::cout << ((float)i * 100.0) / (float)vect_road_clients_uniques.size() << "%" << std::endl;
		NodeInfo* client_unique = vect_road_clients_uniques[i]->depot.next;
		
		bool client_insere = false;

		//Parcours des autres routes
		for (RouteInfo* route_cur = ws_.first(); !client_insere && route_cur != nullptr; route_cur = route_cur->next_) {
			std::cout << "-";
			if (std::find(vect_road_clients_uniques.begin(), vect_road_clients_uniques.end(), route_cur) == vect_road_clients_uniques.end()) {	//Si la route courante n'est pas une route a client unique
				
				NodeInfo * cur_node = &(route_cur->depot);
				cur_node = cur_node->next;
				NodeInfo * first_node = cur_node;
				bool cont = true;
				//Parcours des clients de la route
				while ((cont || cur_node->customer->id() != route_cur->depot.customer->id()) && !client_insere){
					cont = false;
					Time distance = ws_.data().distance(client_unique->customer->id(), cur_node->customer->id());
					Time time_to_reach_unique_client = std::max(cur_node->arrival, cur_node->customer->open()) 
						+ distance;

					//Si client eligible
					if (time_to_reach_unique_client > client_unique->customer->open() &&																								//Intervalle temps
						time_to_reach_unique_client < client_unique->customer->close() &&

						ws_.is_feasible(*cur_node, client_unique->customer->demand(), std::max(distance, client_unique->customer->open())))
					{		//test temps et capacite
																									// On supprime le client inutil
						ws_.remove(*client_unique);						// Supprime egalement la tournee

						//Mise a jour du client
						client_unique->route = cur_node->route;
						client_unique->arrival = NO_TIME;
						client_unique->load = NO_LOAD;

						ws_.insert(*cur_node, *client_unique);

						//Mise a jour des autres clients
						ws_.update2(*cur_node);
						std::cout << std::endl;
					//	ws_.display2();
						ws_.check();

						client_insere = true;
						retour = true;
					}
					cur_node = cur_node->next;
				}
			}
		}
		std::cout << std::endl;
	}
	//melange au sein d'une meme tournee
	//swap a et b voisin

	return retour;
}
bool recherche_locale::ot_opt() {
	
	std::vector<Id> already_done; // vecteur contenant les points deja echange;
	bool retour = false;
	
	//insertion d'un node dans une nouvelle tournee
	for (int i = 1; i < ws_.nodes().size(); i++) {
		for (int j = 1; j < ws_.nodes().size();j++) {

			// Test si i et j ne pointent pas sur le meme point et si le point j n'a pas deja ete insere en tant que point i
			if (i != j && std::find(already_done.begin(), already_done.end(), ws_.nodes()[j].customer->id()) == already_done.end()) {
				
				NodeInfo& node_to_insert = ws_.nodes()[i];
				NodeInfo& prec_to_node_to_insert = ws_.nodes()[j];

				Time distance = ws_.data().distance(prec_to_node_to_insert.customer->id(), node_to_insert.customer->id());
				Time time_to_reach_node_to_insert = std::max(prec_to_node_to_insert.arrival, prec_to_node_to_insert.customer->open())
					+ distance;

				if (time_to_reach_node_to_insert > node_to_insert.customer->open() &&												// Intervalle temps
					time_to_reach_node_to_insert < node_to_insert.customer->close() &&

					ws_.is_feasible(prec_to_node_to_insert, node_to_insert.customer->demand(), std::max(distance,node_to_insert.customer->open())))
				{
					std::cout << "HERE : " << prec_to_node_to_insert.arrival << " " << time_to_reach_node_to_insert << std::endl;
					ws_.remove(node_to_insert);
					// ajout dans les points taites
					already_done.push_back(node_to_insert.customer->id());

					// mise a jout du point a inserer
					node_to_insert.route = prec_to_node_to_insert.route;
					node_to_insert.arrival = NO_TIME;
					node_to_insert.load = NO_LOAD;

					ws_.insert(prec_to_node_to_insert, node_to_insert);

					//Mise a jour des autres clients
					//ws_.update2(*prev);
					ws_.update2(prec_to_node_to_insert);
					std::cout << std::endl;
					ws_.display();
					ws_.check();

					retour = true;
				}
			}
		}
	}

	return retour;
}

bool recherche_locale::cross() {
	//Echange de 2 points dans 2 tournees differentes

	return false;
}

//retourne toutes les routes ayant un unique client
std::vector<RouteInfo*> recherche_locale::get_all_unique_route() {
	std::vector<RouteInfo *> res;
	for (RouteInfo* route_cur = ws_.first(); route_cur != nullptr; route_cur=route_cur->next_) {
		if (route_cur->depot.next->next->customer->id() == route_cur->depot.customer->id()) {
			res.push_back(route_cur);
		}
	}
	return res;
}

std::vector<NodeInfo *> recherche_locale::recherche_node_fenetre_temps(const NodeInfo& given_node) {
	bool DEBUG = false;
	std::vector<NodeInfo *> res;
	if (DEBUG) {
		std::cout << "For " << given_node.customer->id() << " open:" << given_node.customer->open() 
			<< " and close:" << given_node.customer->close() << std::endl;
	}
	for (int i=1;i < ws_.nodes().size();i++) //On ne regarde pas le depot
	{
		//std::cout << "id:" << ws_.nodes()[i].customer->id() << " op:" << ws_.nodes()[i].customer->open() <<
		//	" cl:" << ws_.nodes()[i].customer->close() << std::endl;
		bool open_in_range = (ws_.nodes()[i].customer->open() <= given_node.customer->open() + FENETRE_TEMPS)
			&& (ws_.nodes()[i].customer->open() > given_node.customer->open() - FENETRE_TEMPS);

		bool close_in_range = (ws_.nodes()[i].customer->close() <= given_node.customer->close() + FENETRE_TEMPS)
			&& (ws_.nodes()[i].customer->close() > given_node.customer->close() - FENETRE_TEMPS);
		
		if (open_in_range && close_in_range) {
			if (DEBUG) {
				std::cout << "Node n" << i << std::endl;
				std::cout << " Open range: " << given_node.customer->open() - FENETRE_TEMPS << " " << ws_.nodes()[i].customer->open()
					<< " " << given_node.customer->open() + FENETRE_TEMPS << std::endl;
				std::cout << " Close range: " << given_node.customer->close() - FENETRE_TEMPS << " " << ws_.nodes()[i].customer->close()
					<< " " << given_node.customer->close() + FENETRE_TEMPS << std::endl;
			}
			res.push_back(&(ws_.nodes()[i]));
		}
	}
	return res;
}



bool recherche_locale::test_cap() {
	//TODO
	return false;
}
bool recherche_locale::test_temps() {
	//TODO
	return false;
}