#include "recherche_locale.h"


recherche_locale::recherche_locale(WorkingSolution& ws) : ws_(ws)
{
	//dabord cas particulier 2opt* et oropt
	//puis 2opt* et cross
	bool fin = false;

	
	ot_opt_cp();
	/*while (!fin) {
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
	}*/

	recherche_node_fenetre_temps(ws_.nodes()[1]);
	std::vector<RouteInfo*> vect_r = get_all_unique_route();

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
	WorkingSolution new_ws(ws_);
	RouteInfo* x = new_ws.first();
	RouteInfo* y = x->next_;
	//Parcours des tournees
	for (RouteInfo* x = new_ws.first(); x != nullptr; x = x->next_) {
		for (RouteInfo* y = new_ws.first(); y != nullptr; y = y->next_) {
			if (x != y) {
			/*	if (new_ws.is_feasible(y->depot, ,y->distance)) {

				}*/
			}
		}
	}

	return false;
}

bool recherche_locale::two_opt_etoile() {
	//inversement de fins de 2 tournees

	return false;
}

bool recherche_locale::ot_opt_cp() {
	//Insertion d'un point seul dans une tournee dans une autre tournee
	std::vector<RouteInfo*> vect_road_clients_uniques = get_all_unique_route();

	//Parcours des clients seuls
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

					//Si client eligible
					if (cur_node->arrival > client_unique->customer->open() &&																								//Intervalle temps
						cur_node->arrival < client_unique->customer->close() &&

						ws_.is_feasible(*cur_node, client_unique->customer->demand(), client_unique->customer->service())) {		//test temps et capacite

																									// On supprime le client inutil
						ws_.remove(*client_unique);						// Supprime egalement la tournee

						//Mise a jour du client
						client_unique->route = route_cur;
						client_unique->arrival =
							cur_node->arrival
							+ cur_node->customer->service()
							+ std::max(ws_.data().distance(client_unique->customer->id(), cur_node->customer->id()), client_unique->customer->open());
						client_unique->load = cur_node->load + cur_node->customer->demand();

						client_unique->prev =	cur_node;
						client_unique->next = cur_node->next;

						client_unique->prev->next = client_unique;
						client_unique->next->prev = client_unique;

						//Mise a jour des autres clients
						ws_.update2(*client_unique);
						std::cout << std::endl;
						ws_.display();
						ws_.check();

						client_insere = true;
					}
					cur_node = cur_node->next;
				}
			}
		}
		std::cout << std::endl;
	}
	//melange au sein d'une meme tournee
	//swap a et b voisin
	
	return false;
}
bool recherche_locale::ot_opt() {
	//insertion d'un node dans une nouvelle tournee

	return false;
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