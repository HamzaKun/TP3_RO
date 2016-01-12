#pragma once

#include <stack>
#include "workingsolution.h"


class heuristique_insertion : public WorkingSolution
{
private:
	std::vector<NodeInfo*>	c_free_;								// Clients libres pour insertion
	NodeInfo *							depot_;									// Pointeur vers depot
public:
	heuristique_insertion(const Data &);
	~heuristique_insertion() {};

	// Méthode
	void construction_par_insertion();						// Construction de la solution
	NodeInfo* recherche_meilleur_client(NodeInfo*, Time);
};