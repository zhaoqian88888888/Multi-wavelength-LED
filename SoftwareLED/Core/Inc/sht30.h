#ifndef __SHT30_H
#define __SHT30_H

#include "main.h"

void SHT3x_Init(void);
uint8_t ReadSHT3x(float *Hum,float *Temp);
float GetTemp(void);

#endif
