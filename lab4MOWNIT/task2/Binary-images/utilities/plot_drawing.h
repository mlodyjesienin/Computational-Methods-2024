//
// Created by jesienin on 01.04.24.
//

#ifndef BINARY_IMAGES_PLOT_DRAWING_H
#define BINARY_IMAGES_PLOT_DRAWING_H
#include <iostream>
#include <vector>
#include <string>
#include "globals.h"
#include "gnuplot-iostream.h"
void plot_array(const float* arr, int size,const std::string& filepath);
#endif //BINARY_IMAGES_PLOT_DRAWING_H
