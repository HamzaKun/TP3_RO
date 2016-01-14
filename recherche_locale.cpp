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