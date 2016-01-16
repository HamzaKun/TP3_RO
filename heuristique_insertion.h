#pragma once

#include <stack>
#include "workingsolution.h"
#include <algorithm>    // std::sort

class heuristique_insertion : public WorkingSolution
{
private:
	const bool DEBUG = false;

	std::vector<NodeInfo*>	c_free_;								// Clients libres pour insertion
	NodeInfo *							depot_;									// Pointeur vers depot
public:
	heuristique_insertion(const Data &);
	~heuristique_insertion() {};

	// Méthode
	void construction_par_insertion();						// Construction de la solution
	bool recherche_meilleur_client(NodeInfo*, Time, Load ,Id*);


	const NodeInfo *  depot() const { return depot_; }
	NodeInfo *			  depot() { return depot_; }
};

bool compare_to_depot(NodeInfo*, NodeInfo*);