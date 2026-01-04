#ifndef SPECTRUM3_H
#define SPECTRUM3_H

#include <stdint.h>

#define NUM_POINTS 451

extern const uint16_t X_array3[NUM_POINTS];
extern float Y_array3[NUM_POINTS];
extern float Z_array3[];
extern float y_result3, z_result3;


float calculate_a_3(float I, float T);
float calculate_b_3(float I, float T);
float calculate_A1_3(float I, float T);
float calculate_A2_3(float I, float T);
float calculate_x1_3(float I, float T);
float calculate_c_3(float I, float T);
float calculate_spectrum_3(float I, float T, float X);

void update_spectrum_array_3(float I, float T, const uint16_t* X_array3, float* Y_array3, uint16_t size);

void compute_integrals_3(float* y_result3, float* z_result3);

void update_and_use_spectrum_3(float intensity, float temperature);



#endif // SPECTRUM_FIT_H
