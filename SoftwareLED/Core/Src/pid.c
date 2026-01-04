#include "pid.h"
#include <string.h>   // memset
#include <stdlib.h>   // qsort (用于中位数排序)
#include <stdint.h>
#include <stdio.h>
#include "spectrum.h"
#include "spectrum1.h"
#include "spectrum2.h"
#include "spectrum3.h"
#include "main.h"
#include "sht30.h"
#include "ina228.h"
#include "usart.h" 
#include "pid_shared.h"
#include "pid_dowith.h"
#include <stdbool.h>  // 引入 bool 类型的定义

static struct PID mypid;

// PID核心变量（误差、输出）
static float error,last_error,sum_error;
float Lux_OutPWM;        // 最终PID输出（PWM值）
float Lux_OutPWM0;       // PID增量输出
float real_getValue;     // 外部传入的实际测量值（备用）

// 计数与使能变量
int count_pid = 0, count_PWM0 = 0;
extern int PIDcontrol_ENABLE;
extern int LED_Val;//LED的占空比

// 增量式PID震荡保护与参数切换变量
extern int flag_PI, flag_PI_2;
int k_zhendang_1 = 0, k_zhendang_2 = 0, PID_fly = 0;
float zhendang_1[10];
int flag_pidchange = 0;
float get_Lux,t;

float Lux_control(float Target_Lux,uint16_t TIM_CCRx)
{
	if(channel == 1)
	{
	get_Lux=y_result*k3;    // 1. 计算实际测量值（y_result * k2 为校正后的光照值）
	t = Target_Lux-get_Lux; // 2. 计算误差（目标值 - 实际值）
	static int err_count = 0;
	}
	if(channel == 2)
	{
	get_Lux=y_result1*k3;    // 1. 计算实际测量值（y_result * k2 为校正后的光照值）
	t = Target_Lux-get_Lux; // 2. 计算误差（目标值 - 实际值）
	static int err_count = 0;
	}
	if(channel == 3)
	{
	get_Lux=y_result2*k3;    // 1. 计算实际测量值（y_result * k2 为校正后的光照值）
	t = Target_Lux-get_Lux; // 2. 计算误差（目标值 - 实际值）
	static int err_count = 0;
	}	

	if(channel == 4)
	{
	get_Lux=y_result3*k3;    // 1. 计算实际测量值（y_result * k2 为校正后的光照值）
	t = Target_Lux-get_Lux; // 2. 计算误差（目标值 - 实际值）
	static int err_count = 0;
	}
	// -------------------------- 调试信息：打印关键变量 --------------------------
	//printf("差值t: %.2f (目标值: %.2f, 实际值: %.2f)\r\n", t, Target_Lux, get_Lux);
	//printf("差值t: %.2f\r\n",t);
	
	if((t<0.2)&&(t>-0.2))//差值太小，当做偶然误差舍去
		{
			Lux_OutPWM0=0;
		}
		else
		{
			if(channel == 1)
			{
				Lux_OutPWM0=Incremental_PID(&mypid,get_Lux,Target_Lux);//PID增量值
				count_PWM0++;
				if((count_PWM0<30)&&(t>3))//LED刚开启(30*0.3内)，需要大幅调节
				{
					if(Lux_OutPWM0>5)
						 Lux_OutPWM0=5;
					else if(Lux_OutPWM0<-5)
						 Lux_OutPWM0=-5;
				}
				else
				{
					if(Lux_OutPWM0>3)
					Lux_OutPWM0=3;
					else if(Lux_OutPWM0<-3)
						Lux_OutPWM0=-3;		
				}		
			}
			
			if(channel == 2)
			{
				Lux_OutPWM0=Incremental_PID(&mypid,get_Lux,Target_Lux);//PID增量值
				count_PWM0++;
				if((count_PWM0<30)&&(t>3))//LED刚开启(30*0.3内)，需要大幅调节
				{
					if(Lux_OutPWM0>5)
						 Lux_OutPWM0=5;
					else if(Lux_OutPWM0<-5)
						 Lux_OutPWM0=-5;
				}
				else
				{
					if(Lux_OutPWM0>3)
					Lux_OutPWM0=3;
					else if(Lux_OutPWM0<-3)
						Lux_OutPWM0=-3;		
				}		
			}
			
			if(channel == 3)
			{						
				Lux_OutPWM0=Incremental_PID(&mypid,get_Lux,Target_Lux);//PID增量值
				count_PWM0++;
				if((count_PWM0<30)&&(t>3))//LED刚开启(30*0.3内)，需要大幅调节
				{
					if(Lux_OutPWM0>5)
						 Lux_OutPWM0=5;
					else if(Lux_OutPWM0<-5)
						 Lux_OutPWM0=-5;
				}
				else
				{
					if(Lux_OutPWM0>3)
					Lux_OutPWM0=3;
					else if(Lux_OutPWM0<-3)
						Lux_OutPWM0=-3;		
				}						
			}
			
			if(channel == 4)
			{
				Lux_OutPWM0=Incremental_PID(&mypid,get_Lux,Target_Lux);//PID增量值
				count_PWM0++;
				if((count_PWM0<30)&&(t>3))//LED刚开启(30*0.3内)，需要大幅调节
				{
					if(Lux_OutPWM0>5)
						 Lux_OutPWM0=5;
					else if(Lux_OutPWM0<-5)
						 Lux_OutPWM0=-5;
				}
				else
				{
					if(Lux_OutPWM0>3)
					Lux_OutPWM0=3;
					else if(Lux_OutPWM0<-3)
						Lux_OutPWM0=-3;		
				}		
			}
}
			Lux_OutPWM=TIM_CCRx + Lux_OutPWM0;
			if(	Lux_OutPWM < 0)//输出限幅
			{
				Lux_OutPWM=0;
			}
			else if(Lux_OutPWM > 4500)
			{
				Lux_OutPWM=4500-1;
			}
		//HAL_Delay(5);
		LED_spec[0] = Target_Lux;
		LED_spec[1] = get_Lux;
		LED_spec[4] = Lux_OutPWM0;
		LED_spec[5] = Lux_OutPWM;
		printf("%f,%f\r\n" , Target_Lux,get_Lux);
		printf("Lux_OutPWM0 = %.3f\r , Lux_OutPWM = %.3f\r", Lux_OutPWM0,Lux_OutPWM);
		return Lux_OutPWM; 
}


float Lux_controlv(float Target_Lux,float TIM_CCRx)
{
	
	if(channel == 1){
	get_Lux=y_result*k3;    // 1. 计算实际测量值（y_result * k2 为校正后的光照值）
	t = Target_Lux-get_Lux; // 2. 计算误差（目标值 - 实际值）
	static int err_count = 0;
	}	
	if(channel == 2){
	get_Lux=y_result1*k3;    // 1. 计算实际测量值（y_result * k2 为校正后的光照值）
	t = Target_Lux-get_Lux; // 2. 计算误差（目标值 - 实际值）
	static int err_count = 0;
	}
		if(channel == 3){
	get_Lux=y_result2*k3;    // 1. 计算实际测量值（y_result * k2 为校正后的光照值）
	t = Target_Lux-get_Lux; // 2. 计算误差（目标值 - 实际值）
	static int err_count = 0;
	}	if(channel == 4){
	get_Lux=y_result3*k3;    // 1. 计算实际测量值（y_result * k2 为校正后的光照值）
	t = Target_Lux-get_Lux; // 2. 计算误差（目标值 - 实际值）
	static int err_count = 0;
	}
	
	// -------------------------- 调试信息：打印关键变量 --------------------------
	//printf("差值t: %.2f (目标值: %.2f, 实际值: %.2f)\r\n", t, Target_Lux, get_Lux);
	//printf("差值t: %.2f\r",t);
	
	   // ★新增：仅首次进入该通道时，用 DC 对齐一次初值；之后都从上次输出继续累加
//    if (!inited[channel]) {
//        u_prev[channel] = (float)USER_SETDC;   // 59 -> 59.00 初始
//        inited[channel] = 1;
//    }
	
	if((t<0.001)&&(t>-0.001))//差值太小，当做偶然误差舍去
		{
			Lux_OutPWM0=0;
		}
		else
		{
			
			if(channel == 1)//488
			{
			Lux_OutPWM0=Incremental_PIDv(&mypid,get_Lux,Target_Lux);//PID增量值
				
if (t > 0.5) {
    Lux_OutPWM0 = 0.05;
} else if (t < -0.5) {
    Lux_OutPWM0 = -0.05;
}
// 判断 t 在 (0.1, 0.5) 之间（大于0.1且小于0.5）
else if (t > 0.1 && t < 0.5) {
    Lux_OutPWM0 = 0.02;
}
// 判断 t 在 (-0.05, -0.01) 之间（大于-0.05且小于-0.01）
else if (t > -0.05 && t < -0.01) {
    Lux_OutPWM0 = -0.02;
}
//			count_PWM0++;
//			if(count_PWM0<10)//LED刚开启(30*0.3内)，需要大幅调节
//				{
//					if(t>0.5)
//						 Lux_OutPWM0=0.05;
//					else if(t<-0.5)
//						 Lux_OutPWM0=-0.05;
//				}
//				else
//				{
//					if(t>0.1)
//					Lux_OutPWM0=0.01;
//					else if(Lux_OutPWM0<-0.01)
//						Lux_OutPWM0=-0.01;		
//				}	
			}
			
			if(channel == 2)//500
			{
			Lux_OutPWM0=Incremental_PIDv(&mypid,get_Lux,Target_Lux);//PID增量值
				
if (t > 0.5) {
    Lux_OutPWM0 = 0.05;
} else if (t < -0.5) {
    Lux_OutPWM0 = -0.05;
}
// 判断 t 在 (0.1, 0.5) 之间（大于0.1且小于0.5）
else if (t > 0.1 && t < 0.5) {
    Lux_OutPWM0 = 0.02;
}
// 判断 t 在 (-0.05, -0.01) 之间（大于-0.05且小于-0.01）
else if (t > -0.5 && t < -0.1) {
    Lux_OutPWM0 = -0.01;
}
else if (t > 0 && t < 0.1) {
    Lux_OutPWM0 = 0.01;
}
//			count_PWM0++;
//			if((count_PWM0<40))//LED刚开启(30*0.3内)，需要大幅调节
//				{
//					if(Lux_OutPWM0>0.3)
//						 Lux_OutPWM0=0.3;
//					else if(Lux_OutPWM0<-0.3)
//						 Lux_OutPWM0=-0.3;
//				}
//				else
//				{
//					if(Lux_OutPWM0>0.1)
//					Lux_OutPWM0=0.1;
//					else if(Lux_OutPWM0<-0.1)
//						Lux_OutPWM0=-0.1;		
//				}	
				
//				{
//					if(t>0.5)
//						 Lux_OutPWM0=0.05;
//					else if(t<-0.5)
//						 Lux_OutPWM0=-0.05;
//				}
//				else
//				{
//					if(t>0.1)
//					Lux_OutPWM0=0.01;
//					else if(Lux_OutPWM0<-0.01)
//						Lux_OutPWM0=-0.01;		
//				}	

			}
			
			if(channel == 3)//515
			{
			Lux_OutPWM0=Incremental_PIDv(&mypid,get_Lux,Target_Lux);//PID增量值
			count_PWM0++;
		//	if((count_PWM0<30))//LED刚开启(30*0.3内)，需要大幅调节
////				{
////					if(Lux_OutPWM0>0.3)
////						 Lux_OutPWM0=0.3;
////					else if(Lux_OutPWM0<-0.3)
////						 Lux_OutPWM0=-0.3;
////				}
////				else
////				{
////					if(Lux_OutPWM0>0.1)
////					Lux_OutPWM0=0.1;
////					else if(Lux_OutPWM0<-0.1)
////						Lux_OutPWM0=-0.1;		
////				}	
							
if (t > 1) {
    Lux_OutPWM0 = 0.1;
} 
else if(t > 0.5) {
    Lux_OutPWM0 = 0.05;
} else if (t < -0.5) {
    Lux_OutPWM0 = -0.03;
}
// ?D?? t ?ú (0.1, 0.5) ????￡¨′óóú0.1?òD?óú0.5￡?
else if (t > 0.1 && t < 0.5) {
    Lux_OutPWM0 = 0.02;
}
// ?D?? t ?ú (-0.05, -0.01) ????￡¨′óóú-0.05?òD?óú-0.01￡?
else if (t > -0.5 && t < -0.1) {
    Lux_OutPWM0 = -0.015;
}
else if (t > 0 && t < 0.1) {
    Lux_OutPWM0 = 0.01;
}
			}

			if(channel == 4)//530
			{
			Lux_OutPWM0=Incremental_PIDv(&mypid,get_Lux,Target_Lux);//PID增量值
			count_PWM0++;
		//	if((count_PWM0<30)&&(t>1))//LED刚开启(30*0.3内)，需要大幅调节
if (t > 0.5) {
    Lux_OutPWM0 = 0.05;
} else if (t < -0.5) {
    Lux_OutPWM0 = -0.05;
}
// ?D?? t ?ú (0.1, 0.5) ????￡¨′óóú0.1?òD?óú0.5￡?
else if (t > 0.1 && t < 0.5) {
    Lux_OutPWM0 = 0.02;
}
// ?D?? t ?ú (-0.05, -0.01) ????￡¨′óóú-0.05?òD?óú-0.01￡?
else if (t > -0.5 && t < -0.1) {
    Lux_OutPWM0 = -0.03;
}
else if (t > 0 && t < 0.1) {
    Lux_OutPWM0 = 0.01;
}
			}
		}
		//printf("Lux_OutPWM0 = %.3f\r , Lux_OutPWM = %.3f\r", Lux_OutPWM0,Lux_OutPWM);
		Lux_OutPWM=TIM_CCRx + Lux_OutPWM0;

		if(	Lux_OutPWM < 0)//输出限幅
		{
			Lux_OutPWM=0;
		}
		else if(Lux_OutPWM > 100)
		{
			Lux_OutPWM=100;
		}
		//HAL_Delay(5);
		LED_spec[0] = Target_Lux;
		LED_spec[1] = get_Lux;
		LED_spec[4] = Lux_OutPWM0;
		LED_spec[5] = Lux_OutPWM;
		printf(" %f, %f\r\n" , Target_Lux,get_Lux);
		printf(" Lux_OutPWM0 = %.3f\r , Lux_OutPWM = %.3f\r", Lux_OutPWM0,Lux_OutPWM);
		return Lux_OutPWM; 
}



float Incremental_PID(struct PID *sptr,float  Get_Value,float  Target_Value)
{ 	
		if(channel == 1)//488
	{
		if(flag_PI_2 == 0)
	 { sptr->Kp=0.2;
	   sptr->Ki=0.2;
	   sptr->Kd=0;
	 }
	 else	if(flag_PI_2 == 1)
	 { sptr->Kp=0.2;
	   sptr->Ki=0.2;
	   sptr->Kd=0;
	 }	  			
	}
	
		if(channel == 2)//500
	{
		if(flag_PI_2 == 0)
	 { sptr->Kp=0.45;
	   sptr->Ki=0.22;
	   sptr->Kd=0;
	 }
	 else	if(flag_PI_2 == 1)
	 { sptr->Kp=0.45;
	   sptr->Ki=0.22;
	   sptr->Kd=0;
	 }	  			
	}
	
	if(channel == 3)//520
	{
		if(flag_PI_2 == 0)
	 { sptr->Kp=0.3;
	   sptr->Ki=0.24;
	   sptr->Kd=0;
	 }
	 else	if(flag_PI_2 == 1)
	 { sptr->Kp=0.3;
	   sptr->Ki=0.24;
	   sptr->Kd=0;
	 }	  			
	}

		if(channel == 4)//560
	{
		if(flag_PI_2 == 0)
	 { sptr->Kp=0.45;
	   sptr->Ki=0.22;
	   sptr->Kd=0;
	 }
	 else	if(flag_PI_2 == 1)
	 { sptr->Kp=0.45;
	   sptr->Ki=0.22;
	   sptr->Kd=0;
	 }	  			
	}
		sptr->error=Target_Value - Get_Value;   //计算偏差 
		if(( sptr->error >-1)&&( sptr->error < 1))
		 sptr->error = 0;//死区

		sptr->out = sptr->Kp * (sptr->error - sptr->last_error) +            //比例计算值
				   sptr->Ki * sptr->error +                              //积分 
				   sptr->Kd * (sptr->error - 2*sptr->last_error + sptr->last_last_error);//微分

		sptr->last_last_error=sptr->last_error;
		sptr->last_error=sptr->error;	                   //保存上一次偏差 
				
	
		if(flag_pidchange <30)
		    flag_pidchange++;
				 if(flag_PI == 1)//防止变化过大引起震荡
		{
				//调节后期震荡过大时，5次大于10，失能PID,恢复默认
				if((sptr->out>20) || (sptr->out<-20))
				{
					k_zhendang_1++;
				}
				if(k_zhendang_1 == 20)
			  {
					PID_fly=1;//跑飞标志位
					PIDcontrol_ENABLE=0;//PID失能
					k_zhendang_1=0;
				}
				
				if(flag_PI_2==0)
			  {
					k_zhendang_2++;
				}
				if(k_zhendang_2 == 500)//2.5分钟,调节后期对PID输出进行限制，每次最多调1/1000
				{
				  k_zhendang_2=0;
					flag_PI_2=1;
				}
				 
		 }
		
		 return  sptr->out;
		 
}
float Incremental_PIDv(struct PID *sptr,float  Get_Value,float  Target_Value)
{ 	
	if(channel == 1)//488
	{
		if(flag_PI_2 == 0)
	 { sptr->Kp=0.02;
	   sptr->Ki=0.05;
	   sptr->Kd=0;
	 }
	 else	if(flag_PI_2 == 1)
	 { sptr->Kp=0.1;
	   sptr->Ki=0.1;
	   sptr->Kd=0;
	 }	  			
	}
	
	if(channel == 2)//500
	{
		if(flag_PI_2 == 0)
	 { sptr->Kp=0.015;
	   sptr->Ki=0.07;
	   sptr->Kd=0;
	 }
	 else	if(flag_PI_2 == 1)
	 { sptr->Kp=0.015;
	   sptr->Ki=0.05;
	   sptr->Kd=0;
	 }	  			
	}
	
	if(channel == 3)//520
	{
		if(flag_PI_2 == 0)
	 { sptr->Kp=0.012;
	   sptr->Ki=0.06;
	   sptr->Kd=0;
	 }
	 else	if(flag_PI_2 == 1)
	 { sptr->Kp=0.05;
	   sptr->Ki=0.05;
	   sptr->Kd=0;
	 }	  			
	}
	
			if(channel == 4)//560
	{
		if(flag_PI_2 == 0)
	 { sptr->Kp=0.01;
	   sptr->Ki=0.01;
	   sptr->Kd=0;
	 }
	 else	if(flag_PI_2 == 1)
	 { sptr->Kp=0.01;
	   sptr->Ki=0.01;
	   sptr->Kd=0;
	 }	  			
	}

		sptr->error=Target_Value - Get_Value;   //计算偏差 
		if(( sptr->error >-1)&&( sptr->error < 1))
		 sptr->error = 0;//死区

		sptr->out = sptr->Kp * (sptr->error - sptr->last_error) +            //比例计算值
				   sptr->Ki * sptr->error +                              //积分 
				   sptr->Kd * (sptr->error - 2*sptr->last_error + sptr->last_last_error);//微分

		sptr->last_last_error=sptr->last_error;
		sptr->last_error=sptr->error;	                   //保存上一次偏差 
				
	
		if(flag_pidchange <30)
		    flag_pidchange++;
				 if(flag_PI == 1)//防止变化过大引起震荡
		{
				//调节后期震荡过大时，5次大于10，失能PID,恢复默认
				if((sptr->out>20) || (sptr->out<-20))
				{
					k_zhendang_1++;
				}
				if(k_zhendang_1 == 20)
			  {
					PID_fly=1;//跑飞标志位
					PIDcontrol_ENABLE=0;//PID失能
					k_zhendang_1=0;
				}
				
				if(flag_PI_2==0)
			  {
					k_zhendang_2++;
				}
				if(k_zhendang_2 == 500)//2.5分钟,调节后期对PID输出进行限制，每次最多调1/1000
				{
				  k_zhendang_2=0;
					flag_PI_2=1;
				}
				 
		 }
		
		 return  sptr->out;
		 
}


float Incremental_PID1(struct PID *sptr,float  Get_Value,float  Target_Value)
{ 	
	if(channel == 3)//488
	{
		if(flag_PI_2 == 0)
	 { sptr->Kp=0.45;
	   sptr->Ki=0.22;
	   sptr->Kd=0;
	 }
	 else	if(flag_PI_2 == 1)
	 { sptr->Kp=0.45;
	   sptr->Ki=0.22;
	   sptr->Kd=0;
	 }	  			
	}

		sptr->error=Target_Value - Get_Value;   //计算偏差 
		if(( sptr->error >-1)&&( sptr->error < 1))
		 sptr->error = 0;//死区

		sptr->out = sptr->Kp * (sptr->error - sptr->last_error) +            //比例计算值
				   sptr->Ki * sptr->error +                              //积分 
				   sptr->Kd * (sptr->error - 2*sptr->last_error + sptr->last_last_error);//微分

		sptr->last_last_error=sptr->last_error;
		sptr->last_error=sptr->error;	                   //保存上一次偏差 
				
	
		if(flag_pidchange <30)
		    flag_pidchange++;
				 if(flag_PI == 1)//防止变化过大引起震荡
		{
				//调节后期震荡过大时，5次大于10，失能PID,恢复默认
				if((sptr->out>20) || (sptr->out<-20))
				{
					k_zhendang_1++;
				}
				if(k_zhendang_1 == 20)
			  {
					PID_fly=1;//跑飞标志位
					PIDcontrol_ENABLE=0;//PID失能
					k_zhendang_1=0;
				}
				
				if(flag_PI_2==0)
			  {
					k_zhendang_2++;
				}
				if(k_zhendang_2 == 500)//2.5分钟,调节后期对PID输出进行限制，每次最多调1/1000
				{
				  k_zhendang_2=0;
					flag_PI_2=1;
				}
				 
		 }
		
		 return  sptr->out;
		 
}


float Incremental_PID2(struct PID *sptr,float  Get_Value,float  Target_Value)
{ 	
	if(channel == 3)//488
	{
		if(flag_PI_2 == 0)
	 { sptr->Kp=0.45;
	   sptr->Ki=0.22;
	   sptr->Kd=0;
	 }
	 else	if(flag_PI_2 == 1)
	 { sptr->Kp=0.45;
	   sptr->Ki=0.22;
	   sptr->Kd=0;
	 }	  			
	}

		sptr->error=Target_Value - Get_Value;   //计算偏差 
		if(( sptr->error >-1)&&( sptr->error < 1))
		 sptr->error = 0;//死区

		sptr->out = sptr->Kp * (sptr->error - sptr->last_error) +            //比例计算值
				   sptr->Ki * sptr->error +                              //积分 
				   sptr->Kd * (sptr->error - 2*sptr->last_error + sptr->last_last_error);//微分

		sptr->last_last_error=sptr->last_error;
		sptr->last_error=sptr->error;	                   //保存上一次偏差 
				
	
		if(flag_pidchange <30)
		    flag_pidchange++;
				 if(flag_PI == 1)//防止变化过大引起震荡
		{
				//调节后期震荡过大时，5次大于10，失能PID,恢复默认
				if((sptr->out>20) || (sptr->out<-20))
				{
					k_zhendang_1++;
				}
				if(k_zhendang_1 == 20)
			  {
					PID_fly=1;//跑飞标志位
					PIDcontrol_ENABLE=0;//PID失能
					k_zhendang_1=0;
				}
				
				if(flag_PI_2==0)
			  {
					k_zhendang_2++;
				}
				if(k_zhendang_2 == 500)//2.5分钟,调节后期对PID输出进行限制，每次最多调1/1000
				{
				  k_zhendang_2=0;
					flag_PI_2=1;
				}
				 
		 }
		
		 return  sptr->out;
		 
}

float Incremental_PID3(struct PID *sptr,float  Get_Value,float  Target_Value)
{ 	
	if(channel == 3)//488
	{
		if(flag_PI_2 == 0)
	 { sptr->Kp=0.45;
	   sptr->Ki=0.22;
	   sptr->Kd=0;
	 }
	 else	if(flag_PI_2 == 1)
	 { sptr->Kp=0.45;
	   sptr->Ki=0.22;
	   sptr->Kd=0;
	 }	  			
	}

		sptr->error=Target_Value - Get_Value;   //计算偏差 
		if(( sptr->error >-1)&&( sptr->error < 1))
		 sptr->error = 0;//死区

		sptr->out = sptr->Kp * (sptr->error - sptr->last_error) +            //比例计算值
				   sptr->Ki * sptr->error +                              //积分 
				   sptr->Kd * (sptr->error - 2*sptr->last_error + sptr->last_last_error);//微分

		sptr->last_last_error=sptr->last_error;
		sptr->last_error=sptr->error;	                   //保存上一次偏差 
				
	
		if(flag_pidchange <30)
		    flag_pidchange++;
				 if(flag_PI == 1)//防止变化过大引起震荡
		{
				//调节后期震荡过大时，5次大于10，失能PID,恢复默认
				if((sptr->out>20) || (sptr->out<-20))
				{
					k_zhendang_1++;
				}
				if(k_zhendang_1 == 20)
			  {
					PID_fly=1;//跑飞标志位
					PIDcontrol_ENABLE=0;//PID失能
					k_zhendang_1=0;
				}
				
				if(flag_PI_2==0)
			  {
					k_zhendang_2++;
				}
				if(k_zhendang_2 == 500)//2.5分钟,调节后期对PID输出进行限制，每次最多调1/1000
				{
				  k_zhendang_2=0;
					flag_PI_2=1;
				}
				 
		 }
		
		 return  sptr->out;
		 
}