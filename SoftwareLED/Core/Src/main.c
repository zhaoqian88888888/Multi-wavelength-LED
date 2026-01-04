/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include "myiic.h"
#include "GP8211S.h"
#include "./delay/core_delay.h"  
#include <stdlib.h>
#include "motor_ctrl.h"
#include "sht30.h"
#include "ina228.h"
#include "spectrum.h"
#include "pid.h"
#include "pid_dowith.h"
#include "pid_shared.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern DMA_HandleTypeDef hdma_usart3_tx;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern int k_zhendang_1,k_zhendang_2,PID_fly;
uint8_t  ina1s_flag,temp_flag,adj_flag,printf_flag;
uint8_t  temp_times;//Current_VALUE,
uint8_t TASK_BUFF[20];
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
float Singer_Temp,Total_Temp,Current_VALUE,Temp,Hum;			//声明变量存放温湿度数据
uint8_t str_len;
uint8_t motor_flag,light_flag,reset_flag=0,ina_flag=0,baseflag=0,mode_flag=0,arrive_flag=1,Nchange_flag=0,rx_flag=0,get_bc_flag=0,get_bt_flag=0,temp_printf_flag=0;

int LED_Val;//LED的占空比
int	LED_Val_1=0,LED_Val_2=0,LED_Val_3=0,LED_Val_4=0;
//extern float LED_spec[6];

int32_t Current_Reg_VALUE,Bus_V_Reg_VALUE;

float LED_temp,LED_current;
float VBUS_VALUE,BASE_Current,BASE_VBUS,BASE_TEMP,ADJ_TEMP;
float adj_current;
__IO float TARGET;
float USER_SETDC=0,NOW_SETDC;
char CURRENT_BUFF[9];
char CMP_BUFF[10];//触摸屏发送报文
uint8_t BUSY_BUFF[3]={0x30,0xEE,0xEE};
uint8_t ERROR_BUFF[3]={0x30,0xFF,0xFF};

#define 	NOISE_VBUS 10//10mV电压噪声
#define   SHUNT_X    0.963//0.963//电流系数
#define   ADJ_L8		 7.317//LED8的校准因子
#define   K7			-0.54

#define   K_385   -0.4632	 //
#define   K_435   -0.1276		 //合丰435
#define   K_488		-0.1387  //合丰488nm
#define   K_520   -0.1741	 //yf520
//#define   K_520   -0.1498	 //散热80%测得
#define   K_560   -0.2231	 //553nm
#define		K_635		-0.6727
#define   K_805   -0.323	 //盈峰805nm

#define   current_key    0//维持电流稳定
#define		adj_key				 1//维持功率稳定
#define		NONE				 2//无任何调节
uint8_t		MODE=NONE ;//默认维持电流稳定
//电机结构体
typedef struct	
{
	uint32_t	 CLK;//电机移动需要的脉冲
	uint32_t	 NOW_POS;//电机现在位置
	uint32_t	 TARGET_POS;//电机目标位置
	uint8_t  	 DIR;//电机移动的方向
	uint8_t  	 STATE;//电机的状态
}motor;
motor	MOTOR  = {0,0,0,0,0};
//电机状态
#define MOTOR_STOP		0x00			//电机停止
#define MOTOR_RUN			0x01			//电机运行

#define MOTOR_ADD 		0x01			//电机地址
//电机移动方向
#define RESET_DIR			0x00			//复位方向
#define MOVE_UP				0x00      //向上移动
#define MOVE_DOWN 		0x01 			//向下移动
//电机速度
#define RESET_SPEED 	30 				//复位速度30RPM
#define CHANGE_SPEED  600			//LED切换速度
//加减速时间,加减速时间计算公式：t2 - t1 = (256 - acc) * 50(us)，Vt2 = Vt1 + 1(RPM)（注：acc 为加速度档位，Vt1为 t1 时刻的转速，Vt2为 t2 时刻的转速）
#define RESET_ACC 		0xFC 			//复位加减速
#define CHANGE_ACC		0xFF			//LED切换速度
//细分
#define MSETP					256
//脉冲数
#define RESET_CLK			900*MSETP	//复位最大脉冲
#define MOVE_1LED			100*MSETP	//移动一个LED脉冲
#define MOVE_2LED			200*MSETP	
#define MOVE_3LED			300*MSETP	
#define MOVE_4LED			400*MSETP	
#define MOVE_5LED			500*MSETP	
#define MOVE_6LED			600*MSETP	
#define MOVE_7LED			700*MSETP	
#define MOVE_8LED			800*MSETP	
//移动模式，相位/绝对模式，false为相对运动，true为绝对运动
#define REL_MODE			false
#define ABS_MODE			true
//多机同步标志 ，false为不启用，true为启用
#define SINGLE_MODE		false

uint8_t WPID_BUFF[17]={0x01 ,0x4A ,0xC3 ,0x010 ,0x00 ,0x00 ,0x7D ,0x00 ,0x00 ,0x00 ,0x00 ,0x0A ,0x00 ,0x00 ,0x7D ,0x00 ,0x6B};
uint8_t RPID_BUFF[3]={0x01 ,0x21 ,0x6B};
//10WLED位置宏定义八波长
//#define POS_ORIGIN	MSETP*(105   )
//#define POS_LED1		MSETP*(105   )
//#define POS_LED2		MSETP*(196   )
//#define POS_LED3		MSETP*(291   )
//#define POS_LED4		MSETP*(385     )
//#define POS_LED5		MSETP*(481      )
//#define POS_LED6		MSETP*(577       )
//#define POS_LED7		MSETP*(670     )
//#define POS_LED8		MSETP*(768	   )
//自用3WLED位置宏定义八波长
//#define POS_ORIGIN	MSETP*(100   )
//#define POS_LED1		MSETP*(100   )
//#define POS_LED2		MSETP*(195   )
//#define POS_LED3		MSETP*(286   )
//#define POS_LED4		MSETP*(383     )
//#define POS_LED5		MSETP*(480      )
//#define POS_LED6		MSETP*(572       )
//#define POS_LED7		MSETP*(670     )
//#define POS_LED8		MSETP*(760	   )
////魏老师红外3WLED位置宏定义八波长
//#define POS_ORIGIN	MSETP*(98   )
//#define POS_LED1		MSETP*(98   )
//#define POS_LED2		MSETP*(198   )
//#define POS_LED3		MSETP*(295   )
//#define POS_LED4		MSETP*(387     )
//#define POS_LED5		MSETP*(483      )
//#define POS_LED6		MSETP*(579       )
//#define POS_LED7		MSETP*(670     )
//#define POS_LED8		MSETP*(760	   )
////小论文四波长，LED位置宏定义
#define POS_ORIGIN	MSETP*(148   )
#define POS_LED1		MSETP*(148   )
#define POS_LED2		MSETP*(332   )
#define POS_LED3		MSETP*(528   )
#define POS_LED4		MSETP*(720     )
#define POS_LED5		MSETP*(483      )
#define POS_LED6		MSETP*(579       )
#define POS_LED7		MSETP*(670     )
#define POS_LED8		MSETP*(768	   )
//#define POS_ORIGIN	MSETP*(100  -0   )
//#define POS_LED1		MSETP*(100  -0   )
//#define POS_LED2		MSETP*(200  -0   )
//#define POS_LED3		MSETP*(300  -0   )
//#define POS_LED4		MSETP*(400    -0   )
//#define POS_LED5		MSETP*(500    -0   )
//#define POS_LED6		MSETP*(600    -0   )
//#define POS_LED7		MSETP*(700 -0   )
//#define POS_LED8		MSETP*(800    -0   )
//LED开关宏定义
#define	ON_LED1			HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET)
#define ON_LED2			HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_SET)
#define	ON_LED3			HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_SET)
#define	ON_LED4     HAL_GPIO_WritePin(LED4_GPIO_Port,LED4_Pin,GPIO_PIN_SET)
#define	ON_LED5     HAL_GPIO_WritePin(LED5_GPIO_Port,LED5_Pin,GPIO_PIN_SET)
#define	ON_LED6     HAL_GPIO_WritePin(LED6_GPIO_Port,LED6_Pin,GPIO_PIN_SET)
#define	ON_LED7     HAL_GPIO_WritePin(LED7_GPIO_Port,LED7_Pin,GPIO_PIN_SET)
#define	ON_LED8     HAL_GPIO_WritePin(LED8_GPIO_Port,LED8_Pin,GPIO_PIN_SET)
 
#define	OFF_LED1		HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_RESET)
#define OFF_LED2		HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_RESET)
#define	OFF_LED3		HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_RESET)
#define	OFF_LED4    HAL_GPIO_WritePin(LED4_GPIO_Port,LED4_Pin,GPIO_PIN_RESET)
#define	OFF_LED5    HAL_GPIO_WritePin(LED5_GPIO_Port,LED5_Pin,GPIO_PIN_RESET)
#define	OFF_LED6    HAL_GPIO_WritePin(LED6_GPIO_Port,LED6_Pin,GPIO_PIN_RESET)
#define	OFF_LED7    HAL_GPIO_WritePin(LED7_GPIO_Port,LED7_Pin,GPIO_PIN_RESET)
#define	OFF_LED8    HAL_GPIO_WritePin(LED8_GPIO_Port,LED8_Pin,GPIO_PIN_RESET)

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint16_t temp_counter,current_counter,printf_couner;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM3)
	{
		temp_counter++;current_counter++;printf_couner++;
		if(temp_counter==100)
		{
		temp_flag=1;
		temp_counter=0;
		}
		if(current_counter==50)
		{
		ina1s_flag=1;
		current_counter=0;
		}
		if(printf_couner==1000)
		{
		printf_flag=1;
		printf_couner=0;
		}
	}
}
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
		  if(huart->Instance == USART1)
			{
				__HAL_UART_CLEAR_IDLEFLAG(&huart1);
				HAL_UART_DMAStop(&huart1);
				memcpy(TASK_BUFF,RX_BUFF,Size);
				rx_flag=1;
				str_len = Size;
				HAL_UARTEx_ReceiveToIdle_DMA(&huart1, RX_BUFF, 100);
				//__HAL_DMA_DISABLE_IT(&hdma_usart1_rx,DMA_IT_HT);//关闭DMA半满中断
			}
			if(huart->Instance == USART3)
			{
				__HAL_UART_CLEAR_IDLEFLAG(&huart3);
				HAL_UART_DMAStop(&huart3);
			  rx_flag=1;
				str_len = Size;
				if(Size==4)
				{
					rx_flag=1;//接收完成
					str_len = Size;//数据长度
					memcpy(TASK_BUFF,RX_BUFF,Size);
				}
				if(Size<4)
				{
					static uint8_t offset_len=0;//存储上次的长度
					memcpy(TASK_BUFF+offset_len,RX_BUFF,Size);
					offset_len+=Size;
					if(offset_len<4)
					{
						rx_flag=0;
					}
					else if(offset_len==4)
					{
						rx_flag=1;
						str_len=4;
						offset_len=0;
					}
					
					else if(offset_len>4)
					{
						rx_flag=0;
						offset_len=0;
					}
				}
				HAL_UARTEx_ReceiveToIdle_DMA(&huart3, RX_BUFF, 100);
				//__HAL_DMA_DISABLE_IT(&hdma_usart3_rx,DMA_IT_HT);
			}
}
int fputc(int ch, FILE * f){
		HAL_UART_Transmit_DMA(&huart1, (uint8_t *)&ch, 1);///<普通串口发送数据
  while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET){}///<等待发送完成
  return ch;
}
void reset_task(void)
{
	OFF_LED1;
  OFF_LED2;
	OFF_LED3;
	OFF_LED4;
	OFF_LED5;
	OFF_LED6;
	OFF_LED7;
	OFF_LED8; 
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
	HAL_Delay(1000);
}
void light_off(void)
{
		HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_3);
	  OFF_LED1;OFF_LED2;OFF_LED3;OFF_LED4;OFF_LED5;OFF_LED6;OFF_LED7;OFF_LED8;
}

void motor_task()
{
	if(MOTOR.NOW_POS>MOTOR.TARGET_POS)//电机需要向上移
	{
		light_off();//移动前先关闭LED
		MOTOR.DIR=MOVE_UP	;//向上移动		
		MOTOR.CLK=MOTOR.NOW_POS-MOTOR.TARGET_POS;		//计算相对移动脉冲	
		Emm_V5_Pos_Control(MOTOR_ADD, MOTOR.DIR, CHANGE_SPEED, CHANGE_ACC, MOTOR.CLK, REL_MODE, SINGLE_MODE);//控制电机移动
		MOTOR.NOW_POS=MOTOR.TARGET_POS;//电机现在位置变为目标位置
		arrive_flag=0;//电机移动中，到位标志位清零
	}
	else if(MOTOR.NOW_POS<MOTOR.TARGET_POS)//电机需要向下移
	{	
		light_off();
		MOTOR.DIR=MOVE_DOWN;//向下移动				
		MOTOR.CLK=MOTOR.TARGET_POS-MOTOR.NOW_POS;		
		Emm_V5_Pos_Control(MOTOR_ADD, MOTOR.DIR, CHANGE_SPEED, CHANGE_ACC, MOTOR.CLK, REL_MODE, SINGLE_MODE);
		MOTOR.NOW_POS=MOTOR.TARGET_POS;
		arrive_flag=0;
	}
	else//电机无需移动
	{
		arrive_flag=1;//电机到位
	}
}



//LED控制任务
// 去掉 motor_task() 的调用和 arrive_flag 逻辑
// 在 UART 解析里：
// if (valid_cmd) { light_flag = 1; /* 不再设置 motor_flag */ }

// 在全局范围声明
uint8_t channel;  // 1–8 为 LED，9 为风扇，0x0A 为模式切换
uint8_t action;   // 0x10 开，0x01 关
uint8_t level;    // 0–100

 //重写 light_task：
void light_task(void)
{
	    // ★ 新增：锁 & 记忆当前通道
    static uint8_t user_setdc_locked = 0;   // 0 可更新；1 锁定，不再被报文覆盖
    static uint8_t last_led_channel  = 0;   // 上一次工作的 LED 通道 (1~8
	
     channel = TASK_BUFF[1];  // 1–8 为 LED，9 为风扇，0x0A 为模式切换
     action  = TASK_BUFF[3];  // 0x10 开，0x01 关
     level   = TASK_BUFF[2];  // 0–100

    // --------- 模式切换 ---------
    if (channel == 0x0A) {
        mode_flag = (level == 1) ? 1 : 0;  // 0：模拟调光，1：PWM 调光
        return;
    }

    // --------- 风扇控制 ---------
    if (channel == 9) {
        if (action == 0x10) { // 开风扇
            uint16_t pwm = (level == 100)
                ? (level * 10 - 1)
                : (level > 0 ? level * 10 : 0);
            TIM2->CCR1 = pwm;
            if (pwm) HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
            else     HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
        } else if (action == 0x01) { // 关风扇
            TIM2->CCR1 = 0;
            HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
        }
        return;
    }

    // --------- LED 调光 ---------
    if (channel >= 1 && channel <= 8)
    {
//			        // ★ 若切换了 LED 通道，允许下一次 ON 更新 USER_SETDC
//        if (channel != last_led_channel) {
//            user_setdc_locked = 0;
//            last_led_channel  = channel;
//        }
        if (action == 0x10) // 开灯并调光
        {
            // ★ 仅在“未锁定”时，从报文写入一次 USER_SETDC，并同步初始化 NOW_SETDC
         //   if (!user_setdc_locked) {
                USER_SETDC = level;     // 只在首次 ON 或切换通道后的首次 ON 覆盖
                NOW_SETDC  = USER_SETDC; // 初始化一次，之后由你在别处实时调整
         //       user_setdc_locked = 1;   // 上锁：后续同通道的 ON 报文不再覆盖
         //   }
            if (mode_flag == 0)  // 模拟调光 (DAC)
            {
                if (USER_SETDC == 0) {
                    TIM4->CCR3 = 0;
                } else {
                    //float voltage = (USER_SETDC) / 10.0f;
                    GP8211S_Set_Value(GP8211S_Add, (NOW_SETDC)/100.0f*10);								
                    TARGET = (NOW_SETDC)/100.0f*10;
                    TIM4->CCR3 = 4500;
                }
            }
            else                // PWM 调光
            {
                GP8211S_Set_Value(GP8211S_Add, 10.0f);
                if (USER_SETDC == 0)      TIM4->CCR3 = 0;
                else if (USER_SETDC == 100) TIM4->CCR3 = 4500 - 1;
                else                      TIM4->CCR3 = USER_SETDC * 45; 
            }

		if(MOTOR.TARGET_POS==POS_LED1)		 {	OFF_LED2;OFF_LED3;OFF_LED4;OFF_LED5;OFF_LED6;OFF_LED7;OFF_LED8;ON_LED1; }//目标LED是1号，关闭其他LED，仅开启1号LED
		else if(MOTOR.TARGET_POS==POS_LED2){	OFF_LED1;OFF_LED3;OFF_LED4;OFF_LED5;OFF_LED6;OFF_LED7;OFF_LED8;ON_LED2; }
		else if(MOTOR.TARGET_POS==POS_LED3){	OFF_LED1;OFF_LED2;OFF_LED4;OFF_LED5;OFF_LED6;OFF_LED7;OFF_LED8;ON_LED3; }
		else if(MOTOR.TARGET_POS==POS_LED4){	OFF_LED1;OFF_LED2;OFF_LED3;OFF_LED5;OFF_LED6;OFF_LED7;OFF_LED8;ON_LED4; }
		else if(MOTOR.TARGET_POS==POS_LED5){	OFF_LED1;OFF_LED2;OFF_LED3;OFF_LED4;OFF_LED6;OFF_LED7;OFF_LED8;ON_LED5; }
		else if(MOTOR.TARGET_POS==POS_LED6){	OFF_LED1;OFF_LED2;OFF_LED3;OFF_LED4;OFF_LED5;OFF_LED7;OFF_LED8;ON_LED6; }
		else if(MOTOR.TARGET_POS==POS_LED7){	OFF_LED1;OFF_LED2;OFF_LED3;OFF_LED4;OFF_LED5;OFF_LED6;OFF_LED8;ON_LED7; }
		else if(MOTOR.TARGET_POS==POS_LED8){	OFF_LED1;OFF_LED2;OFF_LED3;OFF_LED4;OFF_LED5;OFF_LED6;OFF_LED7;ON_LED8; }
//						
//            // 先关闭所有 LED，再打开指定通道
//            OFF_LED1; OFF_LED2; OFF_LED3; OFF_LED4;
//            OFF_LED5; OFF_LED6; OFF_LED7; OFF_LED8;

//						switch (channel) {
//                case 1: ON_LED1; break;//yingguangqd(); 
//                case 2: ON_LED2; break;
//                case 3: ON_LED3; break;
//                case 4: ON_LED4; break;
//                case 5: ON_LED5; break;
//                case 6: ON_LED6; break;
//                case 7: ON_LED7; break;
//                case 8: ON_LED8; break;
//            }
					

            HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
            HAL_Delay(100);
            ina_flag  = 1;
            baseflag  = 1;
        }
        else if (action == 0x01) // 关灯
        {
            light_off();
            ina_flag     = 0;
            Current_VALUE= 0;
            VBUS_VALUE   = 0;
					
//					  // ★ 关灯时解锁：允许下一次 ON 用新 level 覆盖 USER_SETDC
//            user_setdc_locked = 0;
        }
    }
}







//电流测量任务
void ina_task()
{
		if(baseflag==1)
		{
			//HAL_Delay(100);
			BASE_Current=GET_CURRENT()*SHUNT_X;//初始电流
			BASE_VBUS=GET_VBUS()-NOISE_VBUS;//初始电压
			//pri【ntf("BV%.3f,BC:mA%.3f\r\n",BASE_VBUS,BASE_Current);
			get_bc_flag=1;
			if(get_bc_flag==1&&get_bt_flag==1)//得到初始电流和初始温度
			{
				baseflag=0;//调节基准标志位设置为0
				get_bc_flag=0;//初始电流获取标志位设置为0
				get_bt_flag=0;//初始温度获取标志位设置为0
			}
		}
		else
		{
			Current_VALUE=GET_CURRENT()*SHUNT_X;//当前电流
			VBUS_VALUE=GET_VBUS()-NOISE_VBUS;//当前电压
			adj_flag=1;//完成电流测量，调节任务可开启
		}			
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin==GPIO_PIN_11)
	{
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_11)==GPIO_PIN_SET)
		{
			__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_11);//清除到位检测外部中断标志位，避免误触发外部中断
			reset_flag=1;
		}
	}
		 if(GPIO_Pin==GPIO_PIN_5)
		{
			if(TASK_BUFF[0]!=0)
			{
				//电机已经切换到位
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)==GPIO_PIN_SET)
				{
					__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_5);//清除到位检测外部中断标志位，避免误触发外部中断					
					arrive_flag=1;	
				}
			}
		}
}


//模拟调制方案



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
	{
  /* USER CODE BEGIN 1 */
	
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	IIC_Init();
	INAIIC_Init();
	INA228_Init();
	GP8211S_Init();
	SHT3x_Init();
	
	reset_task();//复位LED，打开风扇
	Emm_V5_Pos_Control(MOTOR_ADD,	MOVE_DOWN, CHANGE_SPEED, RESET_ACC, MOVE_1LED , REL_MODE, SINGLE_MODE);//电机先下移
	HAL_Delay(1000);
	Emm_V5_Pos_Control(MOTOR_ADD,	RESET_DIR, RESET_SPEED, RESET_ACC, RESET_CLK , REL_MODE, SINGLE_MODE);//电机向上移动
	while(reset_flag==0)//等待电机到达原点
	{};
		Emm_V5_Stop_Now(MOTOR_ADD,false);//电机到原点，控制电机急停
		HAL_Delay(50);
//		Emm_V5_Reset_CurPos_To_Zero(MOTOR_ADD);
		MOTOR.NOW_POS = 0;//电机位置设置为0
		Emm_V5_Pos_Control(MOTOR_ADD,	MOVE_DOWN, CHANGE_SPEED, CHANGE_ACC, POS_LED1 , REL_MODE, SINGLE_MODE);//控制电机移动到1号LED位置
		HAL_Delay(50);
		MOTOR.NOW_POS = POS_LED1;//电机位置设置为1号LED
	
		HAL_TIM_Base_Start_IT(&htim3);
		sprintf(CMP_BUFF,"page 1\xff\xff\xff");//发送给串口屏切换页面
		usart3_SendCmd(CMP_BUFF,9 );
		HAL_Delay(50);
		TIM2->CCR1=0;
		HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
		HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
		
		HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	//接收到一帧数据
		if(rx_flag==1) 
		{
			//printf("%s\r\n",RX_BUFF);
			//判断报文是否正确
			if(TASK_BUFF[0]==0x30&&TASK_BUFF[1]>=0x00&&TASK_BUFF[1]<=0x0d&&TASK_BUFF[2]>=0x00&&TASK_BUFF[2]<=0xB4&&(TASK_BUFF[3]==0x01||TASK_BUFF[3]==0x10))
			{
				//判断是否空闲(完成上一次切换或要进行首次切换)
				if(arrive_flag==1)//电机空闲
				{
				switch(TASK_BUFF[1])//判断要切换的通道号
				{
					case 0x01: MOTOR.TARGET_POS = POS_LED1;motor_flag=1;break;
					case 0x02: MOTOR.TARGET_POS = POS_LED2;motor_flag=1;break;
					case 0x03: MOTOR.TARGET_POS = POS_LED3;motor_flag=1;break;
					case 0x04: MOTOR.TARGET_POS = POS_LED4;motor_flag=1;break;
					case 0x05: MOTOR.TARGET_POS = POS_LED5;motor_flag=1;break;
					case 0x06: MOTOR.TARGET_POS = POS_LED6;motor_flag=1;break;
					case 0x07: MOTOR.TARGET_POS = POS_LED7;motor_flag=1;break;
					case 0x08: MOTOR.TARGET_POS = POS_LED8;motor_flag=1;break;
					case 0x09: //控制风扇
					if(TASK_BUFF[3]==0x10)//开启风扇
					{
						if(TASK_BUFF[2]<=20&&TASK_BUFF[2]>0)
						{
							TIM2->CCR1 = 200;
							HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
						}
						else if(TASK_BUFF[2]==100)
						{
							TIM2->CCR1 = TASK_BUFF[2]*10-1;
							HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
						}
						else if(TASK_BUFF[2]==0)
						{
							TIM2->CCR1 = 0;
							HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_2);
						}
						else
						{
							TIM2->CCR1 = TASK_BUFF[2]*10;
							HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
						}
					}
					if(TASK_BUFF[3]==0x01){TIM2->CCR1 = 0;}//关闭风扇
					break;
					case 0x0a://控制模式
					{
						if(TASK_BUFF[2]==0)//模拟调光
						{
							mode_flag=0;
						}
						if(TASK_BUFF[2]==1)//PWM调光
						{
							mode_flag=1;
						}
					}
					default:break;
				}
			}
				else
				{
					HAL_UART_Transmit_DMA(&huart1,BUSY_BUFF,3);//光源繁忙中
				}
		 }
			else//接收到的指令不正确
			{
				//HAL_UART_Transmit_DMA(&huart1,ERROR_BUFF,3);//发送错误报文
			}
			memset(RX_BUFF,0,str_len);//清除接收字符
			rx_flag=0;//接收完一帧数据
		}
		if(motor_flag==1)//电机需要切换通道
		{
			motor_task();
			motor_flag=0;
		  light_flag=1;
		}
		if(light_flag==1&&arrive_flag==1)//LED需要控制，并且电机已经完成切换
		{
			light_task();//执行LED任务
			HAL_UART_Transmit_DMA(&huart1,TASK_BUFF,4);//回传任务报文给上位机，表示任务已经执行
			light_flag=0;
		}
		if(temp_flag==1)//温度需要测量
		{
			ReadSHT3x(&Hum,&Temp);	//读取温湿度数据
			temp_printf_flag=1;//温度获取到，可打印到串口进行观测
			if(baseflag==1)//初始测量
			{
				BASE_TEMP=Temp;//初始温度
				get_bt_flag=1;//已经获取到初始温度
				if(get_bc_flag==1&&get_bt_flag==1)//初始温度和初始电流都获取到，标志位清零
				{
					baseflag=0;
					get_bc_flag=0;
					get_bt_flag=0;
				}
			}
			temp_flag=0;//完成温度测量，标志位清零
		}
		if(ina_flag==1&&ina1s_flag==1)
		{
			ina_task();
			ina1s_flag=0;
			
		}
		if(adj_flag==1)
		{
			//adj_task();
			adj_flag=0;
		}
		if(printf_flag==1&&temp_printf_flag==1)//1S打印一次信息。printf_flag为1表示一秒时间到，temp_printf_flag为1表示温度已经获取，到可打印串口
		{
//			char temp_buff[64];
			char buf[300]; 
//			light_task();//执行LED任务
//			HAL_UART_Transmit_DMA(&huart1,TASK_BUFF,4);
			LuxValueCorrect();

//						snprintf(buf, sizeof buf,
//             "%.2f %.3f\r\n",
//             (double)LED_spec[0], (double)LED_spec[1]);
//						
						snprintf(buf, sizeof buf,
             "   %.2f %.3f\r\n",
             (double)LED_spec[2], (double)LED_spec[3]);
												
//						snprintf(buf, sizeof buf,
//             "%.2f %.3f\r\n",
//             (double)LED_spec[4], (double)LED_spec[5]);
						
//					snprintf(buf, sizeof buf,
//         "%.2f %.3f\r%.2f %.3f\r%.2f %.3f\r\n",
//         (double)LED_spec[0], (double)LED_spec[1],
//         (double)LED_spec[2], (double)LED_spec[3],
//         (double)LED_spec[4], (double)LED_spec[5]);
//			
		//char buf[160];  // 够大，避免溢出
//    snprintf(buf, sizeof(buf),
//                     "T=%.2fC, I=%.3fmA, V=%.3fmV, P=%.2f%%, X=%.3f, Y=%.3f\r\n",
//                     (double)LED_spec[0], (double)LED_spec[1], (double)LED_spec[2],
//                     (double)LED_spec[3], (double)LED_spec[4], (double)LED_spec[5]);
			HAL_UART_Transmit_DMA(&huart1,(uint8_t*) buf,strlen( buf));//串口发送
			HAL_Delay(3);
			memset(buf,0,sizeof(buf));//清空buff
			printf_flag=0;
			temp_printf_flag=0;
		}
    /* USER CODE END WHILE */

		
		
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV2;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL15;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
