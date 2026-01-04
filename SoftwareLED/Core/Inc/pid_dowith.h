#ifndef PID_DOWITH_H
#define PID_DOWITH_H

#include <stdint.h>


void LuxValueCorrect(void);
void PID_LightControl(void);
float Median_Filter(float arr[], int k);
extern float k,k0,k1,k2,k3;


#endif // PID_H