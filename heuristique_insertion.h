#pragma once

#include <stack>
#include "workingsolution.h"


class heuristique_insertion : public WorkingSolution
{
private:
	std::stack<NodeInfo &> c_free_;								// Clients libres pour insertion
public:
	heuristique_insertion(const Data &);
	~heuristique_insertion();

	// Getters/Setters
	std::vector<Customer *> c_free();							// Getter pour client libre
	void c_free(NodeInfo &);									// Setter pour client libre

	// Méthode
	void construction_par_insertion();							//
	void insertion(Customer* new_client);						//
};