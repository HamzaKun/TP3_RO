#pragma once

#include <algorithm>
#include <cassert>
#include "workingsolution.h"


class heuristique_fusion : public WorkingSolution
{
public:
	heuristique_fusion(const Data &);
	~heuristique_fusion();

	void construction_par_fusion();
};