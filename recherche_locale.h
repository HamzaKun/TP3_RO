#pragma once
#include "workingsolution.h"

class recherche_locale
{
protected:
	WorkingSolution& ws_;
public:
	recherche_locale(WorkingSolution& ws);
	~recherche_locale();
	
	// Algos RL
	void two_opt_etoile_cp();					//Cas particulier
	void two_opt_etoile();						//Cas general (appel cas particulier)
	void ot_opt_cp();									//Cas particulier
	void ot_opt();										//Cas general (appel cas particulier)
	void cross();											//Cas general
	void recherche_locale_generale();	//Appel de tous les algo
};

