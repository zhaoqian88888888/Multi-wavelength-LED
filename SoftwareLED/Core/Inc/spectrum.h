#ifndef SPECTRUM_H
#define SPECTRUM_H

#include <stdint.h>

#define NUM_POINTS 451

extern const uint16_t X_array[NUM_POINTS];
extern float Y_array[NUM_POINTS];
extern float Z_array[];
extern float y_result, z_result;


float calculate_a(float I, float T);
float calculate_b(float I, float T);
float calculate_A1(float I, float T);
float calculate_A2(float I, float T);
float calculate_x1(float I, float T);
float calculate_c(float I, float T);
float calculate_spectrum(float I, float T, float X);

void update_spectrum_array(float I, float T, const uint16_t* X_array, float* Y_array, uint16_t size);

void compute_integrals(float* y_result, float* z_result);

void update_and_use_spectrum(float intensity, float temperature);



#endif // SPECTRUM_FIT_H
