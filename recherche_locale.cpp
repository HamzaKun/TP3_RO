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