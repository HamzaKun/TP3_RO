#pragma once
#include "workingsolution.h"

const int FENETRE_TEMPS=2500;
const bool DEBUG = true;

class recherche_locale
{
protected:
	WorkingSolution& ws_;
public:
	recherche_locale(WorkingSolution& ws);		// Appel de tous les algos dans le constructeur
	~recherche_locale() {}

	std::vector<NodeInfo *> recherche_node_fenetre_temps(const NodeInfo& node_cur);

	
	// Algos RL
	bool two_opt_etoile_cp();					//Cas particulier
	bool two_opt_etoile();						//Cas general (appel cas particulier)
	bool ot_opt_cp();									//Cas particulier
	bool ot_opt();										//Cas general (appel cas particulier)
	bool cross();											//Cas general
};

