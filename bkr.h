#pragma once

#include "data.h"

/*---------------------------------------------*/
/* Permet de lire les meilleur r�sulat trouv�s */
/* par des chercheurs sur l'instance trait�e   */
/*---------------------------------------------*/
struct BKR
{
    static const std::string BKR_FILE;

    std::string name_;
    unsigned    nb_routes_;			// Nombre de tourn�es effectu�es
    double      total_dist_;		

    BKR (const Data &);
};


std::ostream & operator<< (std::ostream &, const BKR &);
