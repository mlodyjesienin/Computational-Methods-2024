#include <iostream>
#include <vector>
#include <algorithm>
#include "../utilities/globals.h"

bool in_boundaries(int row,int col,int size){
    return 0<=row && row <size && 0<=col && col < size;
}

std::vector<int> get_neighbours(int i,int size){
    int d_row, d_col,new_row,new_col,idx;
    int row = i/size;
    int col = i%size;
    std::vector<int> neighbours;
    for(int k=0 ; k< NUM_NGHB; k+=2){
        d_row = NEIGHBOURHOOD[k];
        d_col = NEIGHBOURHOOD[k+1];
        new_row = row +d_row;
        new_col = col + d_col;
        if(in_boundaries(new_row,new_col,size)){
            idx = new_row * size + new_col;
            neighbours.push_back(idx);
        }
    }
    return neighbours;
}

std::vector<int> vector_sum(std::vector<int>* vec1, std::vector<int>* vec2){
    for(int val :*vec2){
        if(std::find(vec1->begin(), vec1->end(),val)==vec1->end()) {
            vec1->push_back(val);
        }
    }
    return *vec1;
}

std::vector<int> cells_affected(int i, int j,int size){
    std::vector<int> i_neighbours = get_neighbours(i,size);
    std::vector<int> j_neighbours = get_neighbours(j,size);
    std::vector<int> affected = vector_sum(&i_neighbours,&j_neighbours);

    if(std::find(affected.begin(), affected.end(),i)==affected.end()) {
        affected.push_back(i);
    }
    if(std::find(affected.begin(), affected.end(),j)==affected.end()) {
        affected.push_back(j);
    }

    return affected;
}

float normal_energy(const unsigned  char *array, int size, int i){
    unsigned char type = array[i];
    float energy =0.0f;
    std::vector<int> neighbours = get_neighbours(i,size);
    for(int neighbour : neighbours){
        if(array[neighbour]==type){
            energy++;
        }
    }
    //std::cout << "idx: " << i << std::endl;
    //std::cout << "energy: " << energy << std::endl;
    return energy;
}

float update_normal_energy(const unsigned  char *old_arr, const unsigned char *new_arr, float old_energy, int size, int i, int j){
    //std:: cout << "start update: " << i <<  " " << j << " old energy:  " <<old_energy << std::endl;
    float new_energy = old_energy;
    std::vector<int> affected = cells_affected(i,j,size);
    for(int idx : affected){
        new_energy-= normal_energy(old_arr,size,idx);
        new_energy+= normal_energy(new_arr,size,idx);
    }
    return new_energy;
}


float unity_energy(const unsigned  char *array, int size, int i){
    unsigned char type = array[i];
    float energy =8.0f;
    std::vector<int> neighbours = get_neighbours(i,size);
    for(int neighbour : neighbours){
        if(array[neighbour]==type){
            energy--;
        }
    }
    return energy;
}

float update_unity_energy(const unsigned  char *old_arr, const unsigned char *new_arr, float old_energy, int size, int i, int j){
    float en=0;
    for(int k=0;k<size*size;k++){
        en+= unity_energy(new_arr,size,k);
    }
    return en;
}

int dist_from_center(int i,int size){
    int row = i/size;
    int col = i%size;
    int center = size/2;
    return (row-center)*(row-center) + (col-center)*(col-center);
}
float circle_energy(const unsigned  char *array, int size, int i){
    float energy = 8;
    unsigned char type = array[i];
    if(type==255){
        return 0.0;
    }
    int dist = dist_from_center(i,size);
    int nghb_dist;
    for(int nghb: get_neighbours(i,size)){
        nghb_dist = dist_from_center(nghb,size);
        if(nghb_dist<dist && array[nghb]==255){
            energy --;
        }
    }
    return energy;
}

float update_circle_energy(const unsigned  char *old_arr, const unsigned char *new_arr, float old_energy, int size, int i, int j){
    float en = 0;
    for(int k=0; k <size*size; k++){
        en+= circle_energy(new_arr,size,k);
    }
    return en;
}

float center_energy(const unsigned  char *array, int size, int i){
    int dist = dist_from_center(i,size);
    float norm = normal_energy(array,size,i);
    if(array[i]==255){
        return size*size - dist + 5*norm*norm*norm*norm;
    }
    else{
        return dist + 5*norm*norm*norm*norm;
    }
}

float update_center_energy(const unsigned  char *old_arr, const unsigned char *new_arr, float old_energy, int size, int i, int j){
    float new_energy = old_energy;
    new_energy-= center_energy(old_arr,size,i);
    new_energy-= center_energy(old_arr,size,j);
    new_energy+= center_energy(new_arr,size,i);
    new_energy+= center_energy(new_arr,size,j);
    return new_energy;
}


float diff_energy(const unsigned  char *array, int size, int i){
    unsigned char type = array[i];
    float en=0.0f;
    for(int nghb: get_neighbours(i,size)){
        if(array[nghb]==type){
            en++;
        }
        else{
            en--;
        }
    }
    return std::abs(en);
}


float update_diff_energy(const unsigned  char *old_arr, const unsigned char *new_arr, float old_energy, int size, int i, int j){
    float en = 0;
    for(int k=0;k<size*size;k++){
        en+= diff_energy(new_arr,size,k);
    }
    return en;
}
float update_diff_energy2(const unsigned  char *old_arr, const unsigned char *new_arr, float old_energy, int size, int i, int j){
    float new_energy = old_energy;
    for(int nghb: cells_affected(i,j,size)) {
        new_energy -= diff_energy(old_arr, size, nghb);
        new_energy += diff_energy(new_arr, size, nghb);
    }
    return new_energy;
}

float parabola_energy(const unsigned  char *array, int size, int i){
    unsigned char type = array[i];
    if(type==255){
        return 0.0f;
    }
    float counter = 0;
    for(int nghb: get_neighbours(i,size)){
        if(array[nghb]==type){
            counter++;
        }
    }
    if(counter < 3){
        return counter;
    }
    if(counter > 10){
        return 15.0f-counter;
    }
    return 2.0f * std::abs(8-counter);
}

float update_parabola_energy(const unsigned  char *old_arr, const unsigned char *new_arr, float old_energy, int size, int i, int j){
    float en = 0;
    for(int k=0;k<size*size;k++){
        en+= parabola_energy(new_arr,size,k);
    }
    return en;
}