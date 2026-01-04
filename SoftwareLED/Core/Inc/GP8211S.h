#ifndef		__GP8211S_H
#define   __GP8211S_H
#include "myiic.h"
//1,10V输出模式
//0,5V输出模式
#define  OUT_MODE_10V  1
#if OUT_MODE_10V
#define  OUT_Mode 0x77
#define  MOST_OUT 10
#else
#define  OUT_Mode	0x55
#define  MOST_OUT 5.0f
#endif
#define  GP8211S_Add	0xB0 //模块地址
void GP8211S_Init(void);
void GP8211S_Set_Value(uint16_t DAC_ADD,float Analoge_Value);

#endif

