#include "recherche_locale.h"


recherche_locale::recherche_locale(WorkingSolution& ws) : ws_(ws)
{
	//dabord cas particulier 2opt* et oropt
	//puis 2opt* et cross
}


recherche_locale::~recherche_locale()
{
}

bool recherche_locale::two_opt_etoile_cp() {
	//Ajout si possible de 2 tournees

	
	NodeInfo * x_node;
	NodeInfo * y_depot;
	NodeInfo * y_first_node;

	NodeInfo * next_depot;
	NodeInfo * cur_node;

	WorkingSolution new_w(ws_);
	RouteInfo* x = nullptr;
	RouteInfo* y = nullptr;


	// Parcours des tournees
	
	for (RouteInfo * x = new_w.first()->next_; x != nullptr; x = x->next_) {		// Pour chaque route
		for (RouteInfo * y = new_w.first(); x != nullptr; x = x->next_) {		// Pour chaque autre route
			if (x != y) {														// Si on a pas affaire à la même route
				x_node = x->depot.prev;
				y_depot = &(x->depot);
				y_first_node = y_depot->next;
				

				// Vérification de la charge
				if (x_node->load + y_depot->load < new_w.data().fleetCapacity()) {
					
					// Vérification de la fenêtre de temps : calcul de la distance
					Time arrivalTime = x_node->arrival + new_w.data().distance(x_node->customer->id, y_first_node->customer->id);
					if (arrivalTime < y_first_node->customer->close && arrivalTime > y_first_node->customer->open) {
						new_w.append((*x), (*y_first_node));
					}
				}
			}
		}
	}
}

bool recherche_locale::two_opt_etoile() {
	//inversement de fins de 2 tournees
}

bool recherche_locale::ot_opt_cp() {
	//Insertion d'un point seul dans une tournee dans une autre tournee
	//melange au sein d'une meme tournee
	//swap a et b voisin
}
bool recherche_locale::ot_opt() {
	//insertion d'un node dans une nouvelle tournee
}

bool recherche_locale::cross() {
	//Echange de 2 points dans 2 tournees differentes
}