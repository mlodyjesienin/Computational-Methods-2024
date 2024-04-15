//
// Created by jesienin on 01.04.24.
//

#ifndef BINARY_IMAGES_GIF_CREATION_H
#define BINARY_IMAGES_GIF_CREATION_H

#include <iostream>
#include <string>
#include <vector>
#include <filesystem> // Include the filesystem library
#include <cstdlib> // For std::system
#include "globals.h"

void delete_directory(const std::string& path);
int create_gif(int num_frames, const std::string& input_dir, const std::string &output_dir);
int create_frames(const std::vector<unsigned char*>& frames, const std::string &output_dir);
int result_png(const unsigned char* start, const unsigned char *result, std::string &output_dir);
#endif //BINARY_IMAGES_GIF_CREATION_H
