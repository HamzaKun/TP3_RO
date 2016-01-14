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

	//recherche_node_fenetre_temps(ws_.nodes()[1]);

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