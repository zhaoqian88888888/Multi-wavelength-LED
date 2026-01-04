#include "ina228.h"
#include "./delay/core_delay.h"   


/*引脚配置*/
#define INA_SCL_Pin GPIO_PIN_8
#define INA_SCL_GPIO_Port GPIOC
#define SDA_Pin GPIO_PIN_7
#define SDA_GPIO_Port GPIOC
#define INA_W_SCL(x) 	HAL_GPIO_WritePin(INA_SCL_GPIO_Port, INA_SCL_Pin, (GPIO_PinState)(x))
#define INA_W_SDA(x) 	HAL_GPIO_WritePin(INA_SDA_GPIO_Port, INA_SDA_Pin 	, (GPIO_PinState)(x))
#define INA_R_SDA() 	HAL_GPIO_ReadPin(INA_SDA_GPIO_Port, INA_SDA_Pin)
#define INA_R_SCL() 	HAL_GPIO_ReadPin(INA_SCL_GPIO_Port, INA_SCL_Pin)

void INAIIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOC_CLK_ENABLE();   //使能GPIOB时钟
    
    //PB3,4初始化设置
    GPIO_Initure.Pin=GPIO_PIN_7|GPIO_PIN_8;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
    
    INAIIC_SDA=1;
    INAIIC_SCL=1;  
}
//产生IIC起始信号
void INAIIC_Start(void)
{
	INASDA_OUT();     //sda线输出
	INAIIC_SDA=1;	  	  
	INAIIC_SCL=1;
	CPU_TS_Tmr_Delay_US(4);
 	INAIIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	CPU_TS_Tmr_Delay_US(4);
	INAIIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void INAIIC_Stop(void)
{
	INASDA_OUT();//sda线输出
	INAIIC_SCL=0;
	INAIIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	CPU_TS_Tmr_Delay_US(4);
	INAIIC_SCL=1; 
	INAIIC_SDA=1;//发送I2C总线结束信号
	CPU_TS_Tmr_Delay_US(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 INAIIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	INASDA_IN();      //SDA设置为输入  
	INAIIC_SDA=1;CPU_TS_Tmr_Delay_US(1);	   
	INAIIC_SCL=1;CPU_TS_Tmr_Delay_US(1);	 
	while(INAREAD_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			INAIIC_Stop();
			return 1;
		}
	}
	INAIIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void INAIIC_Ack(void)
{
	INAIIC_SCL=0;
	INASDA_OUT();
	INAIIC_SDA=0;
	CPU_TS_Tmr_Delay_US(2);
	INAIIC_SCL=1;
	CPU_TS_Tmr_Delay_US(2);
	INAIIC_SCL=0;
}
//不产生ACK应答		    
void INAIIC_NAck(void)
{
	INAIIC_SCL=0;
	INASDA_OUT();
	INAIIC_SDA=1;
	CPU_TS_Tmr_Delay_US(2);
	INAIIC_SCL=1;
	CPU_TS_Tmr_Delay_US(2);
	INAIIC_SCL=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void INAIIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	INASDA_OUT(); 	    
    INAIIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        INAIIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		CPU_TS_Tmr_Delay_US(2);   //对TEA5767这三个延时都是必须的
		INAIIC_SCL=1;
		CPU_TS_Tmr_Delay_US(2); 
		INAIIC_SCL=0;	
		CPU_TS_Tmr_Delay_US(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 INAIIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	INASDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
       INAIIC_SCL=0; 
        CPU_TS_Tmr_Delay_US(2);
		INAIIC_SCL=1;
        receive<<=1;
        if(INAREAD_SDA)receive++;   
		CPU_TS_Tmr_Delay_US(1); 
    }					 
    if (!ack)
        INAIIC_NAck();//发送nACK
    else
        INAIIC_Ack(); //发送ACK   
    return receive;
}
u16 INA228_Read2Byte(u8 reg_addr)
{
	u16 reg_data=0;
	u16 temp=0;
	INAIIC_Start();
	INAIIC_Send_Byte(WRITE_ADDR);
	if(INAIIC_Wait_Ack())return 0;
	INAIIC_Send_Byte(reg_addr);   
	if(INAIIC_Wait_Ack())return 0;
	INAIIC_Start();
	INAIIC_Send_Byte(READ_ADDR);
	if(INAIIC_Wait_Ack())return 0;
	reg_data= INAIIC_Read_Byte(1);
	reg_data=(reg_data<<8)&0xFF00;
	temp=INAIIC_Read_Byte(0);
	INAIIC_Stop();
	reg_data|=temp;
	return reg_data;
}
u32 INA228_Read3Byte(u8 reg_addr)
{
	u32 reg_data=0;
	u16 temp=0;
	INAIIC_Start();
	INAIIC_Send_Byte(WRITE_ADDR);
	if(INAIIC_Wait_Ack())return 0;
	INAIIC_Send_Byte(reg_addr);   
	if(INAIIC_Wait_Ack())return 0;
	INAIIC_Start();
	INAIIC_Send_Byte(READ_ADDR);
	if(INAIIC_Wait_Ack())return 0;
	reg_data= INAIIC_Read_Byte(1);
	reg_data=(reg_data<<8);
	reg_data|=INAIIC_Read_Byte(1);
	reg_data=(reg_data<<8);
	reg_data|=INAIIC_Read_Byte(0);
	INAIIC_Stop();
	return reg_data>>4;
}
u8 INA228_Write2Byte(u8 reg_addr,u16 reg_data)
{        
	u8 data_high=(u8)((reg_data&0xFF00)>>8);
	u8 data_low=(u8)reg_data&0x00FF;
	INAIIC_Start();
	INAIIC_Send_Byte(WRITE_ADDR);   
	if(INAIIC_Wait_Ack())return 0;
	INAIIC_Send_Byte(reg_addr );    
	if(INAIIC_Wait_Ack())return 0;        
	INAIIC_Send_Byte(data_high);
	if(INAIIC_Wait_Ack())return 0;        
	INAIIC_Send_Byte(data_low);
	if(INAIIC_Wait_Ack())return 0;                 
	INAIIC_Stop();
	HAL_Delay(2);
	return 1;
}

void INA228_Init(void)
{
	INA228_Write2Byte(Config_Reg, 0x0000);//0000_0000_0111_0000 ,ADC转换延时2ms,启用分流温度补偿,IN+和IN-分流满量程±40.96mV
	INA228_Write2Byte(ADC_Config_Reg, 0xB103);//1010_0001_0000_0011,连续总线电压、分流电压和温度,总线电压测量的转换时间：4.12ms,分流电压测量的转换时间：4.12ms,温度测量的转换时间：4.12ms,128次平均
	INA228_Write2Byte(SHUNT_CAL_Reg,0x1000);//0000_0100_0000_0000
}
float GET_CURRENT()
{
	float temp;
	int32_t CURRENT_DATA;
	CURRENT_DATA=INA228_Read3Byte(CURRENT_Reg);
	temp = ((float)CURRENT_DATA)*0.003125;//采样电阻0.02Ω：CURRENT_LSB=15.625uA/bit, 采样电阻0.1Ω：CURRENT_LSB=3.125uA/bit  0.003125
		//temp = ((float)CURRENT_DATA)*0.015625;//采样电阻0.02Ω：CURRENT_LSB=15.625uA/bit, 
	return temp;//单位mA
}

float GET_VBUS()
{
	uint32_t VBUS_DATA;
	VBUS_DATA=INA228_Read3Byte(VBUS_Reg);
	return VBUS_DATA*0.1953125;//单位mV
}
