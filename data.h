#pragma once


#include <string>
#include <iostream>
#include <vector>
#include <list>


typedef unsigned Id;
typedef int      Load;
typedef float    Location;
typedef int      Time;


const Load NO_LOAD = Load(0);
const Time NO_TIME = Time(0);


class Customer
{
  public:
    static const Id NO_NODE;

  protected:
    Id       id_;			// ID du client
    Location x_;			// Abscisse
    Location y_;			// Ordonn�e
    Load     demand_;		// Quantit� de produit demand�e
    Time     open_;			// Date au plus t�t
    Time     close_;		// Date au plus tard
    Time     service_;		// Temps de service

  public:
    Customer (const Id &, const Location &, const Location &, const Load &, const Time &, const Time &, const Time &);

    const Id       & id      () const {return id_;}
    const Location & x       () const {return x_;}
    const Location & y       () const {return y_;}
    const Load &     demand  () const {return demand_;}
    const Time &     open    () const {return open_;}
          Time &     open    ()       {return open_;}
    const Time &     close   () const {return close_;}
          Time &     close   ()       {return close_;}
    const Time &     service () const {return service_;}
};
typedef std::vector<Customer> Cvector;

std::ostream & operator<< (std::ostream &, const Customer &);


class Arc	//arc uv
{
  public:
    static const Time USELESS;
    static const Time INFINITE;

  protected:
    Id   orig_;		// u
    Id   dest_;		// v
    Time cost_;		// distance
    Time saving_;	// Gain pour l'heuristique - Sert pour comparaison

  public:
    Arc (const Id & i, const Id & j, const Time & c): orig_(i), dest_(j), cost_(c) {}

    bool operator< (const Arc & a) const {return cost_ < a.cost_;}

    const Id &   orig () const {return orig_;}
    const Id &   dest () const {return dest_;}
    const Time & cost () const {return cost_;}

    const Time & saving () const {return saving_;}
          Time & saving () {return saving_;}
};
typedef std::list<Arc> Alist;
typedef std::vector<Arc> Avector;

std::ostream & operator<< (std::ostream &, const Arc &);


class Data
{
  protected:
    std::string name_;								// Nom graphe
    bool     has_coordinates_;						//
    unsigned nb_vehicles_;							// Nombre de vehicules dispo
    Load     capacity_;								// Capacit�s par v�hicules
    Cvector  nodes_;								// Sommets du graphes - Vector de clients
    Id       depot_;								// Id du d�p�t ? - Unsigned		
    Time     total_services_;						//
    std::vector< std::vector<Time> > distance_;		// Matrice des distances
    std::vector< std::vector<bool> > is_valid_;		// Matrices de bool�ens
    Avector  arcs_;									// Arcs - Vector d'Arcs

    void build_arcs ();
    void build_adjacency ();

  public:
    Data (const std::string &);						// Contructeur

	// Getters
    const std::string & name () const {return name_;}
    const bool & has_coordinates () const {return has_coordinates_;}

    const unsigned & fleetSize     () const {return nb_vehicles_;}
    const Load &     fleetCapacity () const {return capacity_;}
          unsigned   nb_nodes      () const {return (unsigned)nodes_.size();}
          unsigned   nb_clients    () const {return (unsigned)nodes_.size() - 1;}
    const Id &       depot         () const {return depot_;}
    const Time &     services      () const {return total_services_;}
    const Cvector  & nodes         () const {return nodes_;}
    const Customer & node          (const Id & i) const {return nodes_[i];}
    const Avector &  arcs          () const {return arcs_;}
    const Arc &      arc           (const Id & i, const Id & j) const {return arcs_[0];}
          Time       distance      (const Id & i, const Id & j) const {return distance_[i][j];}
          bool       is_valid      (const Id & i, const Id & j) const {return is_valid_[i][j];}

};

std::ostream & operator<< (std::ostream &, const Data &);

