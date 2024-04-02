#include <cmath>
#include <iostream>
#include <string>
#include <algorithm>
#include "../energyTypes/energyfunctions.h"
#include "../utilities/globals.h"
#include "../utilities/plot_drawing.h"
#include "../utilities/gif_creation.h"


void generate_binary_image(float p, unsigned char* array, int size) {
    for (int i = 0; i < size*size; ++i) {
        // Generate a random number between 0 and 1
        float randomValue = static_cast<float>(rand()) / RAND_MAX;
        //array[i] = (i%size < size/2) ? 0 : 255;

        array[i] = (randomValue <= p) ? 0 : 255;
    }
}

//Helper function, currently useless.
void print_image(const unsigned char* array, int size){
    int col;
    int count = 0;

    for(int i=0; i< size*size; ++i){
        col = i%size;
        if(array[i]==0){
            std::cout << 0;
            count++;
        }
        else{
            std::cout << 1;
        }
        if(col==size-1){
            std::cout << std::endl;
        }
    }
    std::cout << "count: " << count << std::endl;
}


float calculate_energy(const unsigned char* array, int size){
    float result = 0.0f;
    for(int i=0;i<size*size; ++i){
        result +=ENERGY(array,size,i);
    }
    return result;
}

float  p_accept(const float curr_cost, const float nghb_cost,  const double temperature) {
    return std::exp(-std::abs(nghb_cost - curr_cost) / temperature);
}

void generate_neighbour(unsigned char* current, const int size, float* current_cost, const double temperature){
    auto* nghb = new unsigned char[size*size];
    std::copy(current, current+size*size,nghb);
    int x1,x2;
    unsigned char tmp;
    for(int k=0;k<1; k++) {
        while (true) {
            x1 = rand() % (size * size);
            x2 = rand() % (size * size);
            if (nghb[x1] != nghb[x2]) {
                break;
            }
        }
        tmp = nghb[x1];
        nghb[x1] = nghb[x2];
        nghb[x2] = tmp;
    }
    float nghb_cost = ENERGY_UPDATE(current,nghb,*current_cost,size,x1,x2);
    if(nghb_cost <= *current_cost){
        //std::cout << "new is always better! x1,x2: " << x1 << " " << x2 << std::endl;
        tmp = current[x1];
        current[x1] = current[x2];
        current[x2] = tmp;
        *current_cost = nghb_cost;
    }
    else if(p_accept(*current_cost,nghb_cost,temperature) > static_cast<float>(rand()) / RAND_MAX){
        //std:: cout << "worse but still! x1,x2: "<<x1 <<" "<< x2 << std::endl;
        tmp = current[x1];
        current[x1] = current[x2];
        current[x2] = tmp;
        *current_cost = nghb_cost;

    }
    delete[] nghb;
}


int annealing_simulation(const std::string &current_directory){
    srand(time(NULL));

    double  temperature = INITIAL_TEMPERATURE;

    std::filesystem::create_directory(current_directory + OUTPUT_DIR);
    std::string real_output = current_directory + OUTPUT_DIR + RESULT_DIR;
    std::string real_frame_dir = current_directory + OUTPUT_DIR + FRAMES_DIR;
    std::filesystem::create_directory(real_output);
    std::filesystem::create_directory(real_frame_dir);


    auto* current = new unsigned char[IMAGE_SIZE*IMAGE_SIZE];
    auto* best = new unsigned  char[IMAGE_SIZE*IMAGE_SIZE];
    auto* start = new unsigned char[IMAGE_SIZE*IMAGE_SIZE];

    auto* annealing_plot = new float[MAX_ITER/PLOT_DIVISOR];
    int plot_idx = 0;

    std::vector<unsigned char*> frames;  //for gif purposes

    generate_binary_image(PERCENTAGE_OF_BLACK,current,IMAGE_SIZE);
    std::copy(current,current+IMAGE_SIZE*IMAGE_SIZE,best);
    std::copy(current,current+IMAGE_SIZE*IMAGE_SIZE,start);


    //print_image(current,size);
    float current_cost = calculate_energy(current, IMAGE_SIZE);
    float lowest_cost = current_cost;
    std:: cout << "starting cost: " << current_cost << std::endl;

    for(int iter=0;iter <MAX_ITER;++iter) {

        generate_neighbour(current, IMAGE_SIZE, &current_cost, temperature);
        if (iter % FRAMES_DIVISOR == 0) {
            std::cout << "iter: " << iter << std::endl;
            auto* frame = new unsigned char[IMAGE_SIZE * IMAGE_SIZE];
            std::copy(current, current + IMAGE_SIZE * IMAGE_SIZE, frame);
            frames.push_back(frame);
        }
        if(iter%PLOT_DIVISOR==0){
            annealing_plot[plot_idx] = current_cost;
            plot_idx++;
        }

        if(current_cost < lowest_cost){
            std::copy(current,current+IMAGE_SIZE*IMAGE_SIZE,best);
            lowest_cost = current_cost;
        }
        temperature *=ALPHA_COEFFICIENT;
    }
    std::cout << "wow, finally" << std::endl;
    std:: cout << "final cost: " << current_cost << std::endl;

    std::cout<< "plotting..." << std::endl;
    plot_array(annealing_plot,MAX_ITER/PLOT_DIVISOR, real_output);
    result_png(start,best,real_output);

    std::cout<< "creating gif..." << std::endl;
    create_frames(frames,real_frame_dir);
    create_gif(MAX_ITER/FRAMES_DIVISOR,real_frame_dir,real_output);


    for (unsigned char* frame : frames) {
        delete[] frame;
    }
    frames.clear();
    delete[] annealing_plot;
    delete[] current;
    return 0;
}
