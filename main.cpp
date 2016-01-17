#include <iostream>
#include <iomanip>
#include <chrono>
#include <random>
#include <fstream>

#include "bkr.h"
#include "algorithms.h"
#include "workingsolution.h"

#include "heuristique_insertion.h"
#include "heuristique_fusion.h"
#include "recherche_locale.h"


// inline function to compute the relative gap
inline double relative_gap (const unsigned & val, const unsigned & ref)
{
  return (100.0 * (double(val) - double(ref)) / double(ref));
}


// inline function to compute the relative gap
inline double absolute_gap (const unsigned & val, const unsigned & ref)
{
  return (double(val) - double(ref));
}


// inline function to print out a global report about a method
inline void report (std::ostream & os, const std::string & method, const WorkingSolution & sol, const float & time, const BKR & bkr)
{
  std::streamsize w = os.width();
  int gap_routes   = (int)absolute_gap(sol.nb_routes(), bkr.nb_routes_);
  float gap_distance = (float)relative_gap((int)sol.distance(), (int)bkr.total_dist_);
  os << bkr.name_ << " [" << std::setw(20) << method << "]   " << std::setw(3) << sol.nb_routes() << std::setw(w) << "," <<  std::setw(5) << sol.distance() << std::setw(w) << "   in " << std::fixed << std::setw(7) << std::setprecision(2) << time << " s   -> " << std::setw(7) << std::showpos << gap_routes << std::noshowpos << ",  " << std::setprecision(2) << gap_distance << " %" << std::setw(w) << std::endl;
}


// function to test the functionalities around WorkingSolution
void test_solution (const Data & data, const BKR & bkr)
{
  WorkingSolution sol(data);
  std::chrono::time_point<std::chrono::system_clock> start, end;
  float duration;

  if (false)
  {
    std::cout << "default solution" << std::endl;
    std::cout << sol << std::endl;
  }

  if (true)
  {
    std::cout << "dummy solution" << std::endl;
    start = std::chrono::system_clock::now();
    dummy (sol);
    sol.check();
    end = std::chrono::system_clock::now();
    duration = (float)0.001 * std::chrono::duration_cast<std::chrono::milliseconds> (end - start).count();
    report(std::cout, "dummy", sol, duration, bkr);
    std::cout << sol << std::endl;
  }
}


void put_to_file()
{
	clock_t	chronometer = clock();				//Chronometre
	std::ofstream file_res("res.csv");

	if (!file_res) {
		std::cout << "fichiers non initialises" << std::endl;
	}

	std::vector<Data> data_tab;

	data_tab.push_back(Data("INSTANCES/c101.txt"));
	data_tab.push_back(Data("INSTANCES/c102.txt"));
	data_tab.push_back(Data("INSTANCES/c103.txt"));
	data_tab.push_back(Data("INSTANCES/c104.txt"));
	data_tab.push_back(Data("INSTANCES/c105.txt"));
	data_tab.push_back(Data("INSTANCES/c106.txt"));
	data_tab.push_back(Data("INSTANCES/c107.txt"));
	data_tab.push_back(Data("INSTANCES/c108.txt"));
	data_tab.push_back(Data("INSTANCES/c109.txt"));

	data_tab.push_back(Data("INSTANCES/c201.txt"));
	data_tab.push_back(Data("INSTANCES/c202.txt"));
	data_tab.push_back(Data("INSTANCES/c203.txt"));
	data_tab.push_back(Data("INSTANCES/c204.txt"));
	data_tab.push_back(Data("INSTANCES/c205.txt"));
	data_tab.push_back(Data("INSTANCES/c206.txt"));
	data_tab.push_back(Data("INSTANCES/c207.txt"));
	data_tab.push_back(Data("INSTANCES/c208.txt"));

	data_tab.push_back(Data("INSTANCES/r101.txt"));
	data_tab.push_back(Data("INSTANCES/r102.txt"));
	data_tab.push_back(Data("INSTANCES/r103.txt"));
	data_tab.push_back(Data("INSTANCES/r104.txt"));
	data_tab.push_back(Data("INSTANCES/r105.txt"));
	data_tab.push_back(Data("INSTANCES/r106.txt"));
	data_tab.push_back(Data("INSTANCES/r107.txt"));
	data_tab.push_back(Data("INSTANCES/r108.txt"));
	data_tab.push_back(Data("INSTANCES/r109.txt"));
	data_tab.push_back(Data("INSTANCES/r110.txt"));
	data_tab.push_back(Data("INSTANCES/r111.txt"));
	data_tab.push_back(Data("INSTANCES/r112.txt"));

	data_tab.push_back(Data("INSTANCES/r201.txt"));
	data_tab.push_back(Data("INSTANCES/r202.txt"));
	data_tab.push_back(Data("INSTANCES/r203.txt"));
	data_tab.push_back(Data("INSTANCES/r204.txt"));
	data_tab.push_back(Data("INSTANCES/r205.txt"));
	data_tab.push_back(Data("INSTANCES/r206.txt"));
	data_tab.push_back(Data("INSTANCES/r207.txt"));
	data_tab.push_back(Data("INSTANCES/r208.txt"));
	data_tab.push_back(Data("INSTANCES/r209.txt"));
	data_tab.push_back(Data("INSTANCES/r210.txt"));
	data_tab.push_back(Data("INSTANCES/r211.txt"));

	data_tab.push_back(Data("INSTANCES/rc101.txt"));
	data_tab.push_back(Data("INSTANCES/rc102.txt"));
	data_tab.push_back(Data("INSTANCES/rc103.txt"));
	data_tab.push_back(Data("INSTANCES/rc104.txt"));
	data_tab.push_back(Data("INSTANCES/rc105.txt"));
	data_tab.push_back(Data("INSTANCES/rc106.txt"));
	data_tab.push_back(Data("INSTANCES/rc107.txt"));

	data_tab.push_back(Data("INSTANCES/rc201.txt"));
	data_tab.push_back(Data("INSTANCES/rc202.txt"));
	data_tab.push_back(Data("INSTANCES/rc203.txt"));
	data_tab.push_back(Data("INSTANCES/rc204.txt"));
	data_tab.push_back(Data("INSTANCES/rc205.txt"));
	data_tab.push_back(Data("INSTANCES/rc206.txt"));
	data_tab.push_back(Data("INSTANCES/rc207.txt"));
	data_tab.push_back(Data("INSTANCES/rc208.txt"));

	// Calcul des RL et des 5eme generation de chaque donnee 
	for (int i = 0;i < data_tab.size();i++) {
		std::cout << "Data n" << i+1 << std::endl;
		file_res << "Data " << i+1 << ";" << std::endl;
		file_res << "nb_road\tdist" << std::endl;

		std::cout << "Heuristique d'insertion" << std::endl;
		heuristique_insertion hi(data_tab[i]);
		hi.construction_par_insertion();
		file_res << hi.nb_routes() << "\t" << hi.total_distance() << std::endl;

		std::cout << "Recherche locale" << std::endl;
		recherche_locale r(hi);
		file_res << hi.nb_routes() << "\t" << hi.total_distance() << std::endl;
		std::cout << (i * 100) / data_tab.size() << "%" << std::endl << std::endl;
	}

	file_res.close();
}

// main function
int main (int argc, char * argv[])
{
  std::string filename("INSTANCES/c104.txt");

  // check the command line
  if (argc > 2)
  {
    std::cerr << "Error: expecting one single argument (instance name)" << std::endl;
    return 1;
  }
  else if (argc == 2)
  {
    filename = argv[1];
  }

  // load the instance
  Data data(filename);
  //std::cout << data << std::endl;

  // load the BKR
  BKR bkr(data);
  std::cout << "BKR = " << bkr << std::endl;
  std::cout << std::endl;

  // test the solution functionalities
  if (true)
  {
    test_solution (data, bkr);
  }
	
	//Appel des 2 heuristiques
	heuristique_insertion		h_insertion(data);
	h_insertion.construction_par_insertion();
	h_insertion.display();
	h_insertion.check();

	recherche_locale rl(h_insertion);

	std::cout << " Rech loc" << std::endl;
	h_insertion.check();
	h_insertion.display();

//	put_to_file();


  return 0;
}
