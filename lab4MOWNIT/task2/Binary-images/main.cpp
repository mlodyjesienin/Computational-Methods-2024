#include <iostream>
#include <string>
#include "energyTypes/energyfunctions.h"
#include "utilities/globals.h"
#include "algorithm/find_minimum_energy.h"
#include "energyTypes/neighbourhood_types.h"
#include <filesystem>

const std::string OUTPUT_DIR = "/output";
const std::string FRAMES_DIR = "/frames";

const std::string RESULT_DIR = "/seventh";

const int NEIGHBOURHOOD[] = {0, 1, 1, 1, 1, 0, 1, -1, 0, -1, -1, -1, -1, 0, -1, 1, 0, 2, 1, 2, 2, 1, 2, 0, 2, -1, 1, -2, 0, -2};
const int NUM_NGHB = 15;
const float PERCENTAGE_OF_BLACK = 0.4;

const int IMAGE_SIZE = 512;
const int MAX_ITER = 100000;

const double INITIAL_TEMPERATURE = 500;
const double ALPHA_COEFFICIENT = 0.9997;

const int FRAMES_DIVISOR = 100;
const int PLOT_DIVISOR = 10;

EnergyFunction const   ENERGY = parabola_energy;
EnergyUpdate const ENERGY_UPDATE = update_parabola_energy;

int main(){
    std::string current_dir = std::filesystem::current_path().parent_path().string();
    std::cout << "curr path: " <<current_dir << std::endl;
    annealing_simulation(current_dir);
}
