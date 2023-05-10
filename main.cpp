/*
AVOCADO - A Very Optimised Collisions And Diffusion Operation

This code is a lightweight version of EPOCH1D, written in C++.
*/

#include <iostream>
#include <vector>
#include <string>
#include <random>

// Physical constants
const double me = 9.1093837015e-31;
const double mp = 1.67262192369e-27;
const double qe = 1.602176634e-19;

// Declare data structures
class domain;
class cell;
class species;
class particle;

class domain{
  public:
    std::vector<cell> grid;
    void print_particles();
};

class cell {            
  public:
    double x_min; 
    double x_max;                 
    std::vector<species> species_list;
};

class species{
  public:
    double mass;
    double charge;
    double number_density;
    std::string name;
    std::vector<particle> partlist;
};

class particle{
  public:
    double weight;
    double x;
    double v[3];
};

int main() {

  int nx = 5;
  double dx = 10.0e-9;
  int nspecies = 2;  
  int ppc[2] = {3, 6};
  double num_dens[2] = {1.0e20, 1.0e20};
  double mass[2] = {me, mp};
  double charge[2] = {-qe, qe};
  std::string names[2] = {"Electron", "Proton"};
  
  // Initialise random number generator
  std::default_random_engine generator;

  // Loop over all cells and append to the domain
  species current_species;
  cell current_cell;
  particle current_particle;
  domain pic;
  for (int ix = 0; ix < nx; ix++) {
    current_cell.x_min = ix * dx;
    current_cell.x_max = (ix + 1.0) * dx;

    // Associate all species with the current cell
    for (int ispec = 0; ispec < nspecies; ispec++){
      current_species.mass = mass[ispec];
      current_species.charge = charge[ispec];
      current_species.name = names[ispec];
      
      // Populate the current species with particles
      double macro_weight = num_dens[ispec] * dx / ppc[ispec];
      std::uniform_real_distribution<double> cell_x_dist(current_cell.x_min,
                                                         current_cell.x_max);
      for (int ipart = 0; ipart < ppc[ispec]; ipart++){ 
        current_particle.weight = macro_weight;
        current_particle.x = cell_x_dist(generator);
        current_particle.v[0] = 0.0;
        current_particle.v[1] = 0.0;
        current_particle.v[2] = 0.0;
        current_species.partlist.push_back(current_particle);
      }
      current_cell.species_list.push_back(current_species);
      current_species.partlist.clear();
    }

    pic.grid.push_back(current_cell);
    current_cell.species_list.clear();
  }

  pic.print_particles();

  return 0;
}

void domain::print_particles(){
  
  // Cycles through every cell, species and particle present in the current 
  // domain, and writes them to terminal.

  std::vector<cell>::iterator cell_iter;
  std::vector<species>::iterator spec_iter;
  std::vector<particle>::iterator part_iter;

  // Loop over each cell in the domain
  for (cell_iter = grid.begin(); cell_iter < grid.end(); cell_iter++){
    std::cout << "Cell ranging: " << cell_iter->x_min << " to " 
              << cell_iter->x_max << std::endl;
    // Loop over all species
    for (spec_iter = cell_iter->species_list.begin(); 
         spec_iter < cell_iter->species_list.end(); spec_iter++){
      std::cout << "Species: " << spec_iter->name << ", mass: " << 
                spec_iter->mass << ", charge: " << spec_iter->charge 
                << std::endl;
      // Loop over all particles
      for (part_iter = spec_iter->partlist.begin(); 
           part_iter < spec_iter->partlist.end(); part_iter++){
        std::cout << "Weight:" << part_iter->weight << ", x: " << part_iter->x 
                  << ", v: (" << part_iter->v[0] << ", " << part_iter->v[1] 
                  << ", " << part_iter->v[2] << ")" << std::endl; 
      }
    }
  }
}