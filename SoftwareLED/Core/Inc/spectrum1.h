#ifndef SPECTRUM1_H
#define SPECTRUM1_H

#include <stdint.h>

#define NUM_POINTS 451

extern const uint16_t X_array1[NUM_POINTS];
extern float Y_array1[NUM_POINTS];
extern float Z_array1[];
extern float y_result1, z_result1;


float calculate_a_1(float I, float T);
float calculate_b_1(float I, float T);
float calculate_A1_1(float I, float T);
float calculate_A2_1(float I, float T);
float calculate_x1_1(float I, float T);
float calculate_c_1(float I, float T);
float calculate_spectrum_1(float I, float T, float X);

void update_spectrum_array_1(float I, float T, const uint16_t* X_array1, float* Y_array1, uint16_t size);

void compute_integrals_1(float* y_result1, float* z_result1);

void update_and_use_spectrum_1(float intensity, float temperature);



#endif // SPECTRUM_FIT_H
