#include "pid_dowith.h"
#include "pid.h"
#include <string.h>   // memset
#include <stdlib.h>   // qsort (用于中位数排序)
#include <stdint.h>
#include "spectrum.h"
#include "spectrum1.h"
#include "spectrum2.h"
#include "spectrum3.h"
#include "main.h"
#include "sht30.h"
#include "ina228.h"
#include "usart.h"
#include "pid_shared.h"
#include "GP8211S.h"

int	flag_findtarget=0;//标志位：开始启动找反馈目标值时为1，否则为0
int	flag_target=0;//标志位：找到了反馈的目标值时为1，否则为0
int flag_PI,flag_PI_2,k_flagPI;   //用来反馈一段时间后，对调结进行限幅
float i,Val,last_Val,Val_1[20];
int k_target=0;
int getValue_send;
int k_zhongzhifilter;
int PIDcontrol_ENABLE=1;//默认不开启反馈
int z_init_count = 0;

float getValue_1[20],temp_1;//用于中值滤波
extern float LED_temp,LED_current;
float target;
//float y_result, z_result;
float k,k1,k2;
float k0=0;
float dc;
//float Temp,Current_VALUE;
//uint8_t Current_VALUE;

extern float real_getValue;
extern int flag_LED;
extern uint8_t flag_A,flag_B,flag_C,flag_D;
//float LED_spec[6];
float z1,y1,k3;
void LuxValueCorrect(void)
{
	channel = TASK_BUFF[1];
	if(channel == 1)
	 {
			LED_temp = Temp;//获取温度
		  LED_current = Current_VALUE;//获取电流

      //计算激发效率
			//update_spectrum_array(LED_current, LED_temp, X_array, Y_array, NUM_POINTS);
			
			update_and_use_spectrum(LED_current, LED_temp);
			compute_integrals(&y_result, &z_result);
//			LED_spec[0] = y_result;
//			LED_spec[1] = z_result;
			LED_spec[2] = LED_temp;
			LED_spec[3] = LED_current;
		if(k0==0){
			z_init_count ++;
			k3=1;
			if(z_init_count == 3){
			 z1=z_result;
			 y1=y_result;
			 k0=1;
		 }
	 }else
		 {
			 k1=z_result/z1;
			 k2=y_result/y1;
			 k3=k1/k2;
		 }
				//printf(" K1= %f  K2= %f k3= %f y_result= %f s= %f\r",k1, k2,  k3,y_result,y_result*k3);
		//printf(" K1= %f K2= %f k3= %f y_result= %f\r",k1, k2, k3, y_result);
		 		printf(" K1= %f K2= %f k3= %f y_result= %f z_result %f\r",k1, k2, k3, y_result, z_result);
//			printf("LED_temp = %.3f\r , LED_current = %.3f\r\n", LED_temp , LED_current);
//			printf("z_result= %f\n", z_result);    // 打印第一次的比值 K
//			printf("K1= %f\n", k1);  // 打印当前的比值 K1
//			printf(" K2= %f\r", k2);  // 打印 K1 / K 的比值 K2
//			printf(" y_result= %f\r", y_result);  
//			printf(" s= %f\r", y_result*k2);  
		PID_LightControl();
				
			
	 } 
	if(channel == 2)
	 {
			LED_temp = Temp;//获取温度
		  LED_current = Current_VALUE;//获取电流

      //计算激发效率
			//update_spectrum_array(LED_current, LED_temp, X_array, Y_array, NUM_POINTS);
			
			update_and_use_spectrum_1(LED_current, LED_temp);
			compute_integrals_1(&y_result1, &z_result1);
//			LED_spec[0] = y_result;
//			LED_spec[1] = z_result;
			LED_spec[2] = LED_temp;
			LED_spec[3] = LED_current;

	if(k0==0){
			z_init_count ++;
			k3=1;
			if(z_init_count == 3){
			 z1=z_result1;
			 y1=y_result1;
			 k0=1;
		 }
	 }else
		 {
			 k1=z_result1/z1;
			 k2=y_result1/y1;
			 k3=k1/k2;
		 }
		 	printf(" K1= %f K2= %f k3= %f y_result= %f z_result %f\r",k1, k2, k3, y_result1, z_result1);
		 	//	printf(" K1= %f, K2= %f,  k3= %f\n",k1, k2,  k3);
		// printf(" K1= %f K2= %f k3= %f y_result= %f\r",k1, k2, k3, y_result1);
//			printf("LED_temp = %.3f\r , LED_current = %.3f\r\n", LED_temp , LED_current);
//						printf("z_result= %f, K1= %f, K2= %f, y_result= %f, s= %f\r",
//       z_result1, k1, k2, y_result1, y_result1 * k2);
//			printf("z_result= %f\n", z_result1);    // 打印第一次的比值 K
//			printf("K1= %f\n", k1);  // 打印当前的比值 K1
//			printf(" K2= %f\r", k2);  // 打印 K1 / K 的比值 K2
//			printf(" y_result= %f\r", y_result1);  
//			printf(" s= %f\r", y_result1*k2);  
		PID_LightControl();
		 
	 } 
	 
	 	if(channel == 3)
	 {
			LED_temp = Temp;//获取温度
		  LED_current = Current_VALUE;//获取电流

      //计算激发效率
			//update_spectrum_array(LED_current, LED_temp, X_array, Y_array, NUM_POINTS);
			
			update_and_use_spectrum_2(LED_current, LED_temp);
			compute_integrals_2(&y_result2, &z_result2);
//			LED_spec[0] = y_result;
//			LED_spec[1] = z_result;
			LED_spec[2] = LED_temp;
			LED_spec[3] = LED_current;
	if(k0==0){
			z_init_count ++;
			k3=1;
			if(z_init_count == 3){
			 z1=z_result2;
			 y1=y_result2;
			 k0=1;
		 }
	 }else
		 {
			 k1=z_result2/z1;
			 k2=y_result2/y1;
			 k3=k1/k2;
		 }
		printf(" K1= %f K2= %f k3= %f y_result= %f z_result %f\r",k1, k2, k3, y_result2, z_result2);
		// 		printf(" K1= %f, K2= %f,  k3= %f\n",k1, k2,  k3);
		//	printf(" K1= %f  K2= %f k3= %f y_result= %f s= %f\r",k1, k2,  k3,y_result2,y_result2*k3);
//			printf("LED_temp = %.3f\r , LED_current = %.3f\r\n", LED_temp , LED_current);
//			printf("z_result= %f, K1= %f, K2= %f, y_result= %f, s= %f\n",
//       z_result2, k1, k2, y_result2, y_result2 * k2);
			//printf("z_result= %f\n", z_result2);    // 打印第一次的比值 K
//			printf("K1 = %f\n", k1);  // 打印当前的比值 K1
//			printf(" K2 = %f\r", k2);  // 打印 K1 / K 的比值 K2
//			printf(" y_result = %f\r", y_result2);  
//			printf(" s = %f\r", y_result2*k2);  
		PID_LightControl();
	
	//	 	printf(" K1= %f  K2= %f k3= %f y_result= %f s= %f\r",k1, k2,  k3,y_result2,y_result2*k3);

	 } 
	 

	 
	 	if(channel == 4)
	 {
			LED_temp = Temp;//获取温度
		  LED_current = Current_VALUE;//获取电流

      //计算激发效率
			//update_spectrum_array(LED_current, LED_temp, X_array, Y_array, NUM_POINTS);
			
			update_and_use_spectrum_3(LED_current, LED_temp);
			compute_integrals_3(&y_result3, &z_result3);
//			LED_spec[0] = y_result;
//			LED_spec[1] = z_result;
			LED_spec[2] = LED_temp;
			LED_spec[3] = LED_current;
			if(k0==0){
				z_init_count ++;
				k3=1;
				if(z_init_count == 3){
				 z1=z_result3;
				 y1=y_result3;
				 k0=1;
			 }
		 }else
			 {
				 k1=z_result3/z1;
				 k2=y_result3/y1;
				 k3=k1/k2;
			 }
			 printf(" K1= %f K2= %f k3= %f y_result= %f z_result %f\r",k1, k2, k3, y_result3, z_result3);
		// 		printf(" K1= %f, K2= %f,  k3= %f\n",k1, k2,  k3);
		// printf(" K1= %f K2= %f k3= %f y_result= %f\r",k1, k2, k3, y_result3);
//			if(k0==0){
//				k=z_result3/y_result3;
//				k0=k/k;
//			}else
//			{
//				k1=z_result3/y_result3;
//				k2=k1/k;
//			}
//			printf("LED_temp = %.3f\r , LED_current = %.3f\r\n", LED_temp , LED_current);
//						printf("z_result= %f, K1= %f, K2= %f, y_result= %f, s= %f\r",
//       z_result3, k1, k2, y_result3, y_result3 * k2);
//			printf("z_result= %f\n", z_result3);    // 打印第一次的比值 K
//			printf("K1= %f\n", k1);  // 打印当前的比值 K1
//			printf(" K2= %f\r", k2);  // 打印 K1 / K 的比值 K2
//			printf(" y_result= %f\r", y_result3);  
//			printf(" s= %f\r", y_result3*k2);  
		PID_LightControl();
	 } 
	
}

uint16_t TIM_GetCapture3(TIM_TypeDef* TIMx) {
    // 直接返回 TIMx 的 CCR3 寄存器值
    return TIMx->CCR3;
}

void PID_LightControl(void)
{
	
	//第一步：找目标
	if(flag_target==0)
	{
		if(flag_findtarget == 0)
		{
			k_target++;
		}
		if(k_target == 1)
		{
			k_target=0;
			flag_findtarget=1;
		}else{
			if(channel == 2){
			getValue_send=y_result1;//getValue_send
			}
					if(channel == 3){
			getValue_send=y_result2;//getValue_send
			}
							if(channel == 1){
			getValue_send=y_result;//getValue_send
			}
									if(channel == 4){
			getValue_send=y_result3;//getValue_send
			}
		}
	}
	
		if(flag_findtarget==1)//找目标值
		{			
			if(channel == 1)
			{
			Val_1[k_zhongzhifilter]=y_result*k3;
			k_zhongzhifilter++;
			if(k_zhongzhifilter == 5)//515:300ms*6 即可找到最高点
			{
				k_zhongzhifilter=0;
				target=Median_Filter(Val_1,5);
				flag_target=1;
				flag_findtarget=0;
				memset(getValue_1,0,sizeof(getValue_1));
			}
//				printf("找到目标值: target = %.3f\r\n", target);
			}
			
			if(channel == 2)
			{
			Val_1[k_zhongzhifilter]=y_result1*k3;
			k_zhongzhifilter++;
			if(k_zhongzhifilter == 5)//435:300ms*6 即可找到最高点
			{
				k_zhongzhifilter=0;
				target=Median_Filter(Val_1,5);
				flag_target=1;
				flag_findtarget=0;
				memset(getValue_1,0,sizeof(getValue_1));
				
			}
				
//				printf("找到目标值: target = %.3f\r\n", target);
			}
						
			if(channel == 3)
			{
			Val_1[k_zhongzhifilter]=y_result2*k3;
			k_zhongzhifilter++;
			if(k_zhongzhifilter == 5)//488:300ms*6 即可找到最高点
			{
				k_zhongzhifilter=0;
				target=Median_Filter(Val_1,5);
				flag_target=1;
				flag_findtarget=0;
				memset(getValue_1,0,sizeof(getValue_1));
				
			}
				
//				printf("找到目标值: target = %.3f\r\n", target);
			}
			

							
			if(channel == 4)
			{
			Val_1[k_zhongzhifilter]=y_result3*k3;
			k_zhongzhifilter++;
			if(k_zhongzhifilter == 5)//560:300ms*6 即可找到最高点
			{
				k_zhongzhifilter=0;
				target=Median_Filter(Val_1,5);
				flag_target=1;
				flag_findtarget=0;
				memset(getValue_1,0,sizeof(getValue_1));
				
			}
				
//				printf("找到目标值: target = %.3f\r\n", target);
			}
		}
		
		
	//第二步：PID反馈
	if(flag_target==1)
	{
//		 if(flag_PI==0)
//		{
//			k_flagPI++;
//		}
//		 if(k_flagPI ==100 )   //调节300ms*20=6s后 ，对PID进行限幅
//		{
//			k_flagPI=0;
//			flag_PI=1;
//		}
//			getValue_send=y_result*k2;
		if(mode_flag == 0)
		{
    NOW_SETDC = (float)Lux_controlv(target, NOW_SETDC);
    GP8211S_Set_Value(GP8211S_Add, (NOW_SETDC / 100.0f * 10));
		}
		else if(mode_flag == 1)
	{
		if(channel == 1)
		{
			TIM4->CCR3 = (float)Lux_control(target,TIM4->CCR3); 
			Val= (float)(y_result*k3);
		}
		else if(channel == 2)
		{
			TIM4->CCR3 = (float)Lux_control(target,TIM4->CCR3); 
			Val= (float)(y_result1*k3);
		}
		else if(channel == 3)
		{
			TIM4->CCR3 = (float)Lux_control(target,TIM4->CCR3); 
			Val= (float)(y_result2*k3);
		}
		else if(channel == 4)
		{
			TIM4->CCR3 = (float)Lux_control(target,TIM4->CCR3); 
			Val= (float)(y_result3*k3);
		}
		
	}
		//Val= (int)(y_result*k2);
//		printf("通道%d: target=%.3f, CCR3=%d, Val=%d\r\n", 
//               channel, target, TIM4->CCR3, Val);
	}
	
}


float Median_Filter(float array[], int len)
{
    int i = 0;
    int j = 0;
    int t = 0;
    float tmp = 0.0f;    // 
    for (i = 0; i < len - 1; i++)
    {
        for (j = 0; j < len - 1 - i; j++)
        {
            if (array[j] > array[j + 1])
            {
                tmp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = tmp;
            }
        }
    }

    t = len - 3;
    return array[t];     // 
}


