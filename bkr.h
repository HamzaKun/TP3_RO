#pragma once

#include "data.h"

/*---------------------------------------------*/
/* Permet de lire les meilleur résulat trouvés */
/* par des chercheurs sur l'instance traitée   */
/*---------------------------------------------*/
struct BKR
{
    static const std::string BKR_FILE;

    std::string name_;
    unsigned    nb_routes_;			// Nombre de tournées effectuées
    double      total_dist_;		

    BKR (const Data &);
};


std::ostream & operator<< (std::ostream &, const BKR &);
