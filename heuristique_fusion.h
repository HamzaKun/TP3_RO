#pragma once

#include <algorithm>
#include <cassert>
#include "workingsolution.h"


class heuristique_fusion : public WorkingSolution
{
public:
	heuristique_fusion(const Data &);
	~heuristique_fusion();

	void sort();							// Trie les arcs par ordre de gain d�croissant
	void construction_par_fusion();
};