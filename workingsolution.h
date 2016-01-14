#pragma once

#include "data.h"


// structure to keep the varying information on the nodes
struct RouteInfo;
struct NodeInfo					// Client - Element de liste chaînée
{
    Customer const * customer;
    Time        arrival; // arrival time at the node
    Load        load;    // load upon arrival: charges cumulées de tous les clients
    RouteInfo * route;   // route tag
    NodeInfo  * prev;    // next node
    NodeInfo  * next;    // previous node

		std::string	name;			// Ajout : nom du noeud
		double dist_from_dep;	// Ajout distance depuis le depot
};

typedef std::vector<NodeInfo> Nvector;

std::ostream & operator<< (std::ostream &, const NodeInfo &);


// structure to keep the varying information on the route				TOURNEE
struct RouteInfo
{
    Id          id;
    NodeInfo    depot;						// First customer
    Time        distance;
    RouteInfo * prev_;
    RouteInfo * next_;

		RouteInfo() {
		};

		RouteInfo(const Id id, const NodeInfo depot, RouteInfo * prev = nullptr, RouteInfo * next = nullptr) 
	:id(id), depot(depot), distance(0), prev_(prev), next_(next) { }
};

typedef std::vector<RouteInfo> Rvector;

std::ostream & operator<< (std::ostream &, const RouteInfo &);


// Solution to the VRPTW, used to allow fast modifications
class WorkingSolution
{
  public:
    static NodeInfo  * NO_NODE;
    static RouteInfo * NO_ROUTE;
    static const Time REDUCTION_BONUS;
    static const Time BAD_EVAL;
    enum OutputOption {EPS = 0, PDF, PNG, SVG};

  protected:
    const Data & data_;

    Nvector			nodes_;					// Vecteur de clients
    Nvector			depots_;				// Dépôts : pt de departs pr chaque tournees, le meme pour chaque client
    Rvector			routes_;				// Vecteur d'arcs (de tournées) = Stockage des arcs
    RouteInfo * first_;						// Première tournée, tête de liste chaînée
    RouteInfo * last_;						// Dernière tournée, fin de liste chaînée
    RouteInfo * free_;						// Liste des routes libres
    unsigned		nb_routes_;				// Nombre de routes
    Time				total_distance_;	// Distance totale des routes
    float				cpu_time_;			// Temps de calcul

  public:
    WorkingSolution (const Data &);          // Créer une solution

    WorkingSolution & operator= (const WorkingSolution &);

    void read (const std::string &);

    void clear ();														// Vider
    bool check () const;												// Vérifier la validité de la solution
    RouteInfo & open_route ();											// Ouvrir une tournée vide
    void close_route (RouteInfo &);										// Fermer une tournée /!\ VIDE /!\ */
    RouteInfo & open_specific_route (NodeInfo &);						// Ouvrir une tournée avec un client donnée
    void append (RouteInfo &, NodeInfo &);								// Ajouter en fin
    void insert (NodeInfo &, NodeInfo &);								// Insérer un client
    void remove (NodeInfo &);											// Extraire un client
    void do_merge (const Arc &);										// Intègre l'arc dans le graphe ?

    bool is_feasible (NodeInfo &, const Load &, const Time &) const;	// Vérifie si une configuration est possible
		bool is_feasible(NodeInfo & node, RouteInfo & route, const Load & incr_capa, const Time & incr_time) const;
    void update      (NodeInfo &, const Load &, const Time &, RouteInfo *);		// Met à jour les dates de passage
    void update2     (NodeInfo &);

		void display();

	// Getters/Setters
    const Data &      data      () const {return data_;}
		const unsigned &  nb_routes () const {return nb_routes_;}
          unsigned &  nb_routes ()       {return nb_routes_;}
          double      distance  () const {return (double(total_distance_ - data_.services()) * 0.01);}
          Time & total_distance () {return total_distance_;}
    const float &     cpu_time  () const {return cpu_time_;}
          float &     cpu_time  ()       {return cpu_time_;}

          Nvector &   nodes     ()                 {return nodes_;}
    RouteInfo const * first     ()           const {return first_;}
    RouteInfo       * first     ()                 {return first_;}
    const NodeInfo *  first     (unsigned r_id) const {return &(depots_[r_id]);}
    const NodeInfo *  last      (unsigned r_id) const {return &(depots_[r_id]);}
};

bool operator< (const WorkingSolution &, const WorkingSolution &);
std::ostream & operator<< (std::ostream &, const WorkingSolution &);
