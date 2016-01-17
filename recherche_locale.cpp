#include "recherche_locale.h"


recherche_locale::recherche_locale(WorkingSolution& ws) : ws_(ws)
{
	//dabord cas particulier 2opt* et oropt
	//puis 2opt* et cross
	bool fin = false;
	int nb_route = ws_.nb_routes();

	const int MAX_ITE = 10;

	int ite_two_opt_etoile_cp = 0;
	int ite_two_opt_etoile = 0;
	int ite_ot_opt_cp = 0;
	int ite_ot_opt = 0;
	int ite_cross = 0;

	while (!fin) {
		//if (!two_opt_etoile_cp() || ite_two_opt_etoile_cp < MAX_ITE) {
		//	std::cout << "otoptcp, Nb route :" << ws_.nb_routes() << std::endl;
			//if (!(ot_opt_cp() && ite_ot_opt_cp < MAX_ITE)) {
				if (!two_opt_etoile() || ite_two_opt_etoile >= MAX_ITE) {
					std::cout << "otopt, Nb route :" << ws_.nb_routes() << std::endl;
					if (!(ot_opt() && ite_ot_opt < MAX_ITE)) {
						std::cout << "cross, Nb route :" << ws_.nb_routes() << std::endl;
						if (!(cross() && ite_cross < MAX_ITE)) {
							fin = true;
						}	else {
							ite_cross++;
						}
			//		}	else {
			//			ite_ot_opt++;
			//		}
			//	}	else {
			//		ite_two_opt_etoile++;
			//	}
			}	else {
				ite_ot_opt_cp++;
				}
		}	else {
			ite_two_opt_etoile_cp++;
		}
	}

	//recherche_node_fenetre_temps(ws_.nodes()[1]);
	//std::vector<RouteInfo*> vect_r = get_all_unique_route();

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
	NodeInfo * y_node;
	NodeInfo * y_node_next;

	std::cout << "Lancement de 2-opt-* cas particulier" << std::endl;

	// Parcours des tournees
	for (RouteInfo * x = new_w.first(); x != nullptr; x = x->next_) {		// Pour chaque route
		for (RouteInfo * y = new_w.first(); y != nullptr; y = y->next_) {	// Pour chaque autre route
			if (x != y) {													// Si on a pas affaire à la même route
				x_node = x->depot.prev;										// Dernier point de la route x
				y_depot = &(y->depot);										// Dépôt de la route y
				y_node = y_depot->next;										// Premier point de la route y

				// Vérification de la charge
				Time diff = x->depot.arrival + new_w.data().distance(x_node->customer->id(), y_node->customer->id())
					- new_w.data().distance(y_node->route->depot.customer->id(), y_node->customer->id());

				if (new_w.is_feasible((*y_node), x->depot.load, diff)) {

					// Concaténation de deux routes (les tests sont faits)
					while (y_node->customer->id() != new_w.data().depot()) {
						y_node_next = y_node->next;
						new_w.append((*x), (*y_node));
						y_node = y_node_next;
					}

					// Triche pour "vider" la route
					y->depot.next = &(y->depot);							// On clean la route y
					y->depot.prev = &(y->depot);

					RouteInfo * y_prev = y->prev_;

					new_w.close_route((*y));								// Fermer la route

					y = y_prev;
					retour = true;
				}
			}
		}
	}

	if (retour) {															// S'il y a eu des changements on met à jour la workingsolution
		std::cout << "two_opt_etoile cas particulier :" << std::endl;
		std::cout << "Avant : " << ws_.nb_routes() << " routes" << std::endl;
		std::cout << "Apres : " << new_w.nb_routes() << " routes" << std::endl;
		new_w.check();														// Appel du check
		ws_ = new_w;
	}

	return retour;
}

bool recherche_locale::two_opt_etoile() {
	
	bool retour = false;													// Par défaut on retourne faux
	WorkingSolution new_w(ws_);												// Création d'une nouvelle solution

	NodeInfo * a = nullptr;
	NodeInfo * b = nullptr;
	NodeInfo * a2 = nullptr;
	NodeInfo * b2 = nullptr;

	std::cout << "Lancement de 2-opt-* cas general : " << std::endl;
	for (RouteInfo * x = new_w.first(); x != nullptr; x = x->next_) {					// Pour chaque route
		for (NodeInfo * a = x->depot.next; a != &(x->depot) ; a = a->next) {			// Pour chaque point de la première route
			for (RouteInfo * y = x->next_ ; y != nullptr; y = y->next_) {				// Pour chaque autre route
				for (NodeInfo * b = y->depot.next; b != &(y->depot); b = b->next) {
					a2 = a->next;		b2 = b->next;

					// Calcul de distances pour calcul gain
					Time a_a2 = new_w.data().distance(a->customer->id(), a2->customer->id());
					Time b_b2 = new_w.data().distance(b->customer->id(), b2->customer->id());
					Time a_b2 = new_w.data().distance(a->customer->id(), b2->customer->id());
					Time b_a2 = new_w.data().distance(b->customer->id(), a2->customer->id());

					// Premier gain potentiel : (a -> a') - (a -> b')
					Time gain = new_w.data().distance(a->customer->id(), a2->customer->id()) - a_b2;
					// Second gain potentiel : (b -> b') - (b -> a')
					gain += new_w.data().distance(b->customer->id(), b2->customer->id()) - b_a2;
					if (gain > 0) {
						if (new_w.is_feasible((*a2), b->load, b->arrival + b_a2 - b_b2)
							&& new_w.is_feasible((*b2), a->load, a->arrival + a_b2 - a_a2)) {

								NodeInfo * next_node = nullptr;
								Nvector a_nodes;
								Nvector b_nodes;

								// Remove des points de la route a
								while (a2 != &(a->route->depot)) {
									next_node = a2->next;
									a_nodes.push_back(*a2);
									new_w.remove(*a2);
									a2 = next_node;
								}

								// Remove des points de la route b
								while (b2 != &(b->route->depot)) {
									next_node = b2->next;
									b_nodes.push_back(*b2);
									new_w.remove(*b2);
									b2 = next_node;
								}

								// Ajout des points de la route a
								for (int i = 0; i < b_nodes.size(); i++) {
									b2 = &(b_nodes[i]);
									new_w.append(*(a->route), *(b2));
								}

								// Ajout des points de la route b
								for (int i = 0; i < a_nodes.size(); i++) {
									a2 = &(a_nodes[i]);
									new_w.append(*(b->route), *(a2));
								}


								// TODO : to remove : display, cout, check
								new_w.display();
								std::cout << "Avant le check" << std::endl;
								new_w.check();						
								std::cout << "Après le check" << std::endl;
								retour = true;
						}
					}
				}
			}
		}
	}

	if (retour) {															// S'il y a eu des changements on met à jour la workingsolution
		std::cout << "two_opt_etoile cas general :" << std::endl;
		std::cout << "Avant : " << ws_.nb_routes() << " routes" << std::endl;
		std::cout << "Apres : " << new_w.nb_routes() << " routes" << std::endl;
		new_w.check();
		ws_ = new_w;

		std::cout << "Cas général passé avec succès !" << std::endl;
	}

	return retour;
}

bool recherche_locale::ot_opt_cp() {

	bool retour = false;

	// Insertion d'un point seul dans une tournee dans une autre tournee
	std::vector<RouteInfo*> vect_road_clients_uniques = get_all_unique_route();

	// Parcours des clients seuls
	for (int i = 0;i < vect_road_clients_uniques.size();i++) {
		NodeInfo* client_unique = vect_road_clients_uniques[i]->depot.next;
		
		bool client_insere = false;

		//Parcours des autres routes
		for (RouteInfo* route_cur = ws_.first(); !client_insere && route_cur != nullptr; route_cur = route_cur->next_) {
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

						ws_.is_feasible(*cur_node, client_unique->customer->demand(), time_to_reach_unique_client-cur_node->arrival))
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
					//	ws_.display2();
						ws_.check();

						client_insere = true;
						retour = true;

						std::cout << client_unique->customer->id() << " insert after " << cur_node->customer->id() << std::endl;
					}
					cur_node = cur_node->next;
				}
			}
		}
	}
	//melange au sein d'une meme tournee
	//swap a et b voisin

	return retour;
}

bool recherche_locale::ot_opt() {
	
	std::vector<Id> already_done; // vecteur contenant les points deja echange;
	bool retour = false;
	
	Time old_gain = 0;
	
	//insertion d'un node dans une nouvelle tournee
	for (int i = 1; i < ws_.nodes().size(); i++) {
		for (int j = 1; j < ws_.nodes().size();j++) {

			// Test si i et j ne pointent pas sur le meme point et si le point j n'a pas deja ete insere en tant que point i
			if (i != j && std::find(already_done.begin(), already_done.end(), ws_.nodes()[j].customer->id()) == already_done.end()) {
				
				NodeInfo& node_to_insert = ws_.nodes()[i];
				NodeInfo& prec_to_node_to_insert = ws_.nodes()[j];

				Time gain = ws_.data().distance(prec_to_node_to_insert.prev->customer->id(), prec_to_node_to_insert.customer->id()) 
					- ws_.data().distance(node_to_insert.customer->id(), prec_to_node_to_insert.customer->id());
				Time distance = ws_.data().distance(prec_to_node_to_insert.customer->id(), node_to_insert.customer->id());
				Time time_to_reach_node_to_insert = std::max(prec_to_node_to_insert.arrival, prec_to_node_to_insert.customer->open())
					+ distance;

				if (gain>0 && old_gain>gain && 
					
					time_to_reach_node_to_insert > node_to_insert.customer->open() &&												// Intervalle temps
					time_to_reach_node_to_insert < node_to_insert.customer->close() &&

					ws_.is_feasible(prec_to_node_to_insert, node_to_insert.customer->demand(), time_to_reach_node_to_insert - prec_to_node_to_insert.arrival))
				{
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
				//	ws_.display();
					ws_.check();

					std::cout << node_to_insert.customer->id() << " insert after " << prec_to_node_to_insert.customer->id() << " gain:" << gain << std::endl;
					old_gain = gain;
					retour = true;
				}
			}
		}
	}

	return retour;
}

bool recherche_locale::cross() {
	//Echange de 2 points dans 2 tournees differentes
	bool modification = false;
	for each (NodeInfo a in ws_.nodes())
	{
		for each (NodeInfo b in ws_.nodes())
		{
			/*std::vector<Id> a_possibles = recherche_node_fenetre_temps(a);
			std::vector<Id> b_possibles = recherche_node_fenetre_temps(b);*/

			if (a.customer->id() != b.customer->id() && a.customer->id() != 0 && b.customer->id() != 0) {
				NodeInfo& a_temp = ws_.nodes()[a.customer->id()];
				NodeInfo& b_temp = ws_.nodes()[b.customer->id()];

				Time gain = ws_.data().distance(b_temp.prev->customer->id(), b_temp.customer->id());
				gain -= ws_.data().distance(a_temp.customer->id(), b_temp.customer->id());
				gain += ws_.data().distance(a_temp.prev->customer->id(), a_temp.customer->id());
				gain -= ws_.data().distance(b_temp.customer->id(), a_temp.customer->id());

				Time distance_a = ws_.data().distance(b_temp.prev->customer->id(), a_temp.customer->id());
				Time time_to_reach_node_to_insert_a = std::max(b_temp.prev->arrival, a_temp.customer->open())
					+ distance_a;

				Time distance_b = ws_.data().distance(a_temp.prev->customer->id(), b_temp.customer->id());
				Time time_to_reach_node_to_insert_b = std::max(a_temp.prev->arrival, b_temp.customer->open())
					+ distance_b;

				if ((gain>0 &&
					
					time_to_reach_node_to_insert_a > a_temp.customer->open() &&												// Intervalle temps
					time_to_reach_node_to_insert_a < a_temp.customer->close() &&

					ws_.is_feasible(*b_temp.prev, a_temp.customer->demand(), time_to_reach_node_to_insert_a - b_temp.prev->arrival)) 
					
					&& 
					
					(time_to_reach_node_to_insert_b > b_temp.customer->open() &&												// Intervalle temps
					time_to_reach_node_to_insert_b < b_temp.customer->close() &&

					ws_.is_feasible(*a_temp.prev, b_temp.customer->demand(), time_to_reach_node_to_insert_b - a_temp.prev->arrival))					
					){
						swap(a_temp, b_temp);
						ws_.check();
						
						std::cout << a_temp.customer->id() << " swaped with " << b_temp.customer->id() << " gain:" << gain << std::endl;

						modification = true;
				}
			}
		}
	}
	return modification;
}

void recherche_locale::swap(NodeInfo& a, NodeInfo& b) {
	NodeInfo* prev_a = a.prev;
	NodeInfo* next_a = a.next;

	NodeInfo* prev_b = b.prev;
	NodeInfo* next_b = b.next;

	if (next_a->customer->id() != 0 && next_b->customer->id() !=0 ) {

		ws_.remove(a);
		ws_.remove(b);

		ws_.insert(*prev_a, b);

		ws_.insert(*prev_b, a);
	}
	else {
		std::cout << "Swap fails" << std::endl;
	}
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

std::vector<Id> recherche_locale::recherche_node_fenetre_temps(const NodeInfo& given_node) {
	bool DEBUG = false;
	std::vector<Id> res;
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
			res.push_back(ws_.nodes()[i].customer->id());
		}
	}
	return res;
}
