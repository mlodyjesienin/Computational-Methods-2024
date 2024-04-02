#ifndef GLOBALS_H
#define GLOBALS_H
typedef float (*EnergyFunction)(const unsigned char*, int, int);
typedef float (*EnergyUpdate)(const unsigned char*, const unsigned  char*, float, int, int, int);

extern const std::string FRAMES_DIR;
extern const std::string OUTPUT_DIR;
extern const std::string RESULT_DIR;
extern const int NUM_NGHB;
extern const int NEIGHBOURHOOD[];
extern const float PERCENTAGE_OF_BLACK;

extern const int MAX_ITER;
extern const int IMAGE_SIZE;

extern const EnergyFunction ENERGY;
extern const EnergyUpdate ENERGY_UPDATE;

extern const double INITIAL_TEMPERATURE;
extern const double ALPHA_COEFFICIENT;

extern const int FRAMES_DIVISOR;
extern const int PLOT_DIVISOR;
#endif // GLOBALS_H
