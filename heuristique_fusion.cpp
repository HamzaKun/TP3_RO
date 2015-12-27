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

//void heuristique_fusion::construction_par_fusion() {
//	
//	RouteInfo * cur_route = nullptr;
//	NodeInfo & cur_node = cur_route->depot;
//	for (auto & cur_arc : data().arcs()) {
//		cur_route = cur_arc.orig
//		if (cur_arc.orig == cur_node && cur_arc.dest == data().depot) {
//			if (is_feasible(cur_node, , nullptr)) {
//				// DO YOLO
//			}
//		}
//	}
//
//	/*
//	NodeInfo & cur_node = 
//	
//	while (is_feasible(cur_node, cur_node)) {
//		// Se servir de la fonction do merge
//	}*/
//	
//
//}


// pour la fusion, se servir de la fonction append de working solutions