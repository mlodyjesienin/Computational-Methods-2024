//
// Created by jesienin on 01.04.24.
//

#ifndef BINARY_IMAGES_FIND_MINIMUM_ENERGY_H
#define BINARY_IMAGES_FIND_MINIMUM_ENERGY_H
#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>
#include "../energyTypes/energyfunctions.h"
#include "../utilities/globals.h"
#include "../utilities/plot_drawing.h"

// Function to print an array, helper function currently useless.
void print_image(const unsigned char* array, int size);

// Function generates random  binary image of given size, with probability of 0 occurrence equal to p.
void generate_binary_image(float p, unsigned char* array, int size);

// Function to calculate the energy of the array
float calculate_energy(const unsigned char* array, int size);

// Function to calculate the probability of accepting a state change
float p_accept(float curr_cost, float nghb_cost, double temperature);

// Function to generate a neighbor of the current state
void generate_neighbour(unsigned char* current, int size, float* current_cost, double temperature);

// Function to perform the simulated annealing simulation
int annealing_simulation(const std::string &current_directory);

#endif //BINARY_IMAGES_FIND_MINIMUM_ENERGY_H
