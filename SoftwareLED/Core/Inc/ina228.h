#ifndef __INA226_H
#define __INA226_H

#include "main.h"
#include "sys.h"
#include "delay.h"
#define READ_ADDR                  0x81	 //A0=GND，A1=GND // R=1, W=0
#define WRITE_ADDR                 0x80

#define Config_Reg                 0x00		//配置
#define ADC_Config_Reg             0x01		//ADC配置
#define SHUNT_CAL_Reg              0x02		//分流校准
#define SHUNT_TEMPCO_Reg           0x03		//分流温度稀释
#define VSHUNT_Reg        	       0x04		//分流电压测量
#define VBUS_Reg                   0x05		//总线电压测量
#define DIETEMP_Reg                0x06		//温度测量
#define CURRENT_Reg                0x07		//电流测量
#define POWER_Reg                	 0xFE   //功率测量


uint16_t INA228_Read2Byte(uint8_t reg_addr);
uint8_t INA228_Write2Byte(uint8_t reg_addr,uint16_t reg_data);
u32 INA228_Read3Byte(u8 reg_addr);
void INA228_Init(void);
#define INASDA_IN()  {GPIOC->CRL&=0X0FFFFFFF;GPIOC->CRL|=(u32)8<<28;}	//PA7输入模式
#define INASDA_OUT() {GPIOC->CRL&=0X0FFFFFFF;GPIOC->CRL|=(u32)3<<28;} 	//PA7输出模式

//IO操作	
#define INAIIC_SCL   PCout(8) //SCL
#define INAIIC_SDA   PCout(7) //SDA
#define INAREAD_SDA  PCin(7)  //输入SDA

//IIC所有操作函数
		 
void INAIIC_Start(void);				//发送IIC开始信号
void INAIIC_Stop(void);	  			//发送IIC停止信号
void INAIIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 INAIIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 INAIIC_Wait_Ack(void); 				//IIC等待ACK信号
void INAIIC_Ack(void);					//IIC发送ACK信号
void INAIIC_NAck(void);				//IIC不发送ACK信号
void INAIIC_Init(void);
void INAIIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 INAIIC_Read_One_Byte(u8 daddr,u8 addr);	 
float GET_VBUS();
float GET_CURRENT();
#endif
