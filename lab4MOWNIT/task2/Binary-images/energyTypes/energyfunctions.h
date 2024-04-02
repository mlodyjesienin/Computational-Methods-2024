#ifndef ENERGYF_H
#define ENERGYF_H

float normal_energy(const unsigned  char*, int , int );

float update_normal_energy(const unsigned  char*, const unsigned char*, float , int , int , int );


float unity_energy(const unsigned  char*, int , int );

float update_unity_energy(const unsigned  char*, const unsigned char*, float , int , int , int );

float circle_energy(const unsigned  char*, int , int );

float update_circle_energy(const unsigned  char*, const unsigned char*, float , int , int , int );

float center_energy(const unsigned  char*, int , int );

float update_center_energy(const unsigned  char*, const unsigned char*, float , int , int , int );

float diff_energy(const unsigned  char*, int , int );

float update_diff_energy(const unsigned  char*, const unsigned char*, float , int , int , int );

float parabola_energy(const unsigned  char*, int , int );

float update_parabola_energy(const unsigned  char*, const unsigned char*, float , int , int , int );
#endif // ENERGYF_H
