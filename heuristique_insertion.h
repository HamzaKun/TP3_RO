#pragma once

#include <stack>
#include "workingsolution.h"


class heuristique_insertion : public WorkingSolution
{
private:
	std::stack<NodeInfo *> c_free_;								// Clients libres pour insertion
public:
	heuristique_insertion(const Data &);
	~heuristique_insertion() {};

	// Méthode
	void construction_par_insertion();						// Construction de la solution
};