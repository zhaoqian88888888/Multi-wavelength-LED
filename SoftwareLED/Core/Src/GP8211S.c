#include "GP8211S.h"
#include "./delay/core_delay.h"   
/*
函数名：void GP8211S_Init()
函数作用：初始化GP8211S模块
*/
void GP8211S_Init()
{
  IIC_Start();
	IIC_Send_Byte(GP8211S_Add);
	IIC_Ack();
	IIC_Send_Byte(0X01);
	IIC_Ack();
	IIC_Send_Byte(OUT_Mode);
	IIC_Ack();
	IIC_Stop();
}
/*
函数名：void GP8211S_Set_Value(uint16_t DAC_ADD,float Analoge_Value)
函数作用：设置GP8211S的输出电压
参数DAC_ADD： DAC地址
参数Analoge_Value：DAC输出模拟电压值  范围0-5V或0-10V，由输出模式决定
*/
void GP8211S_Set_Value(uint16_t DAC_ADD,float Analoge_Value)
{
	uint16_t Digital_Value;
	uint8_t DATA_LOW,DATA_HIGH;
  Digital_Value=0X007FFF*Analoge_Value/(MOST_OUT);//电压的模拟量转换为对应的数字量
	DATA_LOW=Digital_Value&0X00FF;    //数据低8位
	DATA_HIGH=(Digital_Value>>8)&0x7F;//数据高7位
	IIC_Start();
	IIC_Send_Byte(DAC_ADD);
	IIC_Ack();
	IIC_Send_Byte(0x02);
	IIC_Ack();
	IIC_Send_Byte(DATA_LOW);
	IIC_Ack();
	IIC_Send_Byte(DATA_HIGH);
	IIC_Ack();
	IIC_Stop();
}


