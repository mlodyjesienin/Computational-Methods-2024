//
// Created by jesienin on 01.04.24.
//

#ifndef BINARY_IMAGES_NEIGHBOURHOOD_TYPES_H
#define BINARY_IMAGES_NEIGHBOURHOOD_TYPES_H

const int PLUS_NEIGHBOURHOOD[] = {1,0,    -1,0,    0,1,    0,-1};
const int PLUS_NEIGHBOURHOOD_NUM = 4;

const int EIGHT_NEIGHBOURHOOD[] = {0,1,    1,1,    1,0,    1,-1,    0,-1,    -1,-1,    -1,0,    -1,1};
const int EIGHT_NEIGHBOURHOOD_NUM = 8;

const int DIAGONAL_NEIGHBOURHOOD[] = {1,1,   1,-1,   -1,-1,    -1,1 };
const int DIAGONAL_NEIGHBOURHOOD_NUM = 4;

const int FIFTEEN_NEIGHBOURHOOD[] = {0, 1, 1, 1, 1, 0, 1, -1, 0, -1, -1, -1, -1, 0, -1, 1, 0, 2, 1, 2, 2, 1, 2, 0, 2, -1, 1, -2, 0, -2};
const int FIFTEEN_NEIGHBOURHOOD_NUM = 15;
#endif //BINARY_IMAGES_NEIGHBOURHOOD_TYPES_H
