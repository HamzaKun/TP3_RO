#include "recherche_locale.h"



recherche_locale::recherche_locale(WorkingSolution& ws) : ws_(ws)
{
}


recherche_locale::~recherche_locale()
{
}

void recherche_locale::two_opt_etoile_cp() {
	//Ajout si possible de 2 tournees
}

void recherche_locale::two_opt_etoile() {
	//inversement de fins de 2 tournees
}

void recherche_locale::ot_opt_cp() {
	//Insertion d'un point seul dans une tournee dans une autre tournee
	//melange au sein d'une meme tournee
	//swap a et b voisin
}
void recherche_locale::ot_opt() {
	//insertion d'un node dans une nouvelle tournee
}



void recherche_locale::cross() {
	//Echange de 2 points dans 2 tournees differentes
}

void recherche_locale::recherche_locale_generale() {
	//dabord cas particulier 2opt* et oropt
	//puis 2opt* et cross
}