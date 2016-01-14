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

	recherche_node_fenetre_temps(ws_.nodes()[1]);

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

std::vector<NodeInfo *> recherche_locale::recherche_node_fenetre_temps(const NodeInfo& given_node) {
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
