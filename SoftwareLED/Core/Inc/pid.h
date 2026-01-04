#ifndef PID_H
#define PID_H

#include "main.h" 
#include "stdlib.h"
#include <stdint.h>


struct PID  //此处的PID和struct共同组成struct PID类型，类比于：uint为unsigned int
{
	float Kp;
	float Ki;
	float Kd;
	
  float error;  
	float last_error;  //上次误差
	float last_last_error;//上上次误差
	
	float out;
	
};

float Lux_control(float Target_Lux,uint16_t TIM_CCRx);
float Lux_controlv(float Target_Lux,float TIM_CCRx);
float Incremental_PID(struct PID *sptr,float  Get_Value,float  Target_Value);//增量式PID
float Incremental_PIDv(struct PID *sptr,float  Get_Value,float  Target_Value);
extern float getValue,ina226temp_value,real_getValue;


float Incremental_PID1(struct PID *sptr,float  Get_Value,float  Target_Value);
float Incremental_PID2(struct PID *sptr,float  Get_Value,float  Target_Value);
float Incremental_PID3(struct PID *sptr,float  Get_Value,float  Target_Value);



#endif // PID_H