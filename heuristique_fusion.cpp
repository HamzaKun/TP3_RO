#include "heuristique_fusion.h"


heuristique_fusion::heuristique_fusion(const Data & d) : WorkingSolution(d) {
	
	// Tiré du fichier algorithms.cpp (le fichier a donc été supprimé)
	clear();
	for (auto & node : nodes())
	{
		if (node.customer->id() != data().depot())
		{
			open_specific_route(node);
		}
	}

	// Création de la tournée Dummy : on a une tournée par client
}

heuristique_fusion::~heuristique_fusion() {

}

void heuristique_fusion::sort()
{
	const Id & id_depot = data().depot();
	
	std::vector<std::pair<Time, Arc &>> * gain_order = new std::vector<std::pair<Time, Arc &>>();

	for (auto & cur_arc : data().arcs()) {										// Pour chaque arc
		Time c_i_o = data().arc(cur_arc.orig, id_depot).cost;
		Time c_j_o = data().arc(id_depot, cur_arc.dest).cost;					// On récupère les données par rapport au dépôt

		cur_arc.saving = c_i_o + c_j_o - cur_arc.cost;
	}

	data().sort_arcs();
}

void heuristique_fusion::construction_par_fusion() {
	
	

}


// pour la fusion, se servir de la fonction append de working solutions