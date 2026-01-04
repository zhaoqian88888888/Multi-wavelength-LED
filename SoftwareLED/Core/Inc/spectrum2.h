#ifndef SPECTRUM2_H
#define SPECTRUM2_H

#include <stdint.h>

#define NUM_POINTS 451

extern const uint16_t X_array2[NUM_POINTS];
extern float Y_array2[NUM_POINTS];
extern float Z_array2[];
extern float y_result2, z_result2;


float calculate_a_2(float I, float T);
float calculate_b_2(float I, float T);
float calculate_A1_2(float I, float T);
float calculate_A2_2(float I, float T);
float calculate_x1_2(float I, float T);
float calculate_c_2(float I, float T);
float calculate_spectrum_2(float I, float T, float X);

void update_spectrum_array_2(float I, float T, const uint16_t* X_array2, float* Y_array2, uint16_t size);

void compute_integrals_2(float* y_result2, float* z_result2);

void update_and_use_spectrum_2(float intensity, float temperature);



#endif // SPECTRUM_FIT_H
