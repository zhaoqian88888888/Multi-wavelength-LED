#include "sht30.h"
#include "main.h"
#include "./delay/core_delay.h"   
/*SHT3x地址*/
#define SHT3x_ADDRESS 0x44<<1	//从机地址是7位，所以左移一位

/*引脚配置*/
#define SCL_Pin GPIO_PIN_2
#define SCL_GPIO_Port GPIOD
#define SDA_Pin GPIO_PIN_12
#define SDA_GPIO_Port GPIOC
#define SHT3x_W_SCL(x) 	HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, (GPIO_PinState)(x))
#define SHT3x_W_SDA(x) 	HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, (GPIO_PinState)(x))
#define SHT3x_R_SDA() 	HAL_GPIO_ReadPin(SDA_GPIO_Port, SDA_Pin)
#define SHT3x_R_SCL() 	HAL_GPIO_ReadPin(SCL_GPIO_Port, SCL_Pin)
/*当 STM32 的 GPIO 配置成开漏输出模式时，它仍然可以通过读取
GPIO 的输入数据寄存器获取外部对引脚的输入电平，也就是说它同时具有浮空输入模式的功能*/


/**
  * @brief  CRC校验，CRC多项式为：x^8+x^5+x^4+1，即0x31
  * @param  DAT 要校验的数据
  * @retval 校验码
  */
uint8_t SHT3x_CRC_CAL(uint16_t DAT)
{
	uint8_t i,t,temp;
	uint8_t CRC_BYTE;

	CRC_BYTE = 0xFF;
	temp = (DAT>>8) & 0xFF;

	for(t = 0; t < 2; t++)
	{
		CRC_BYTE ^= temp;
		for(i = 0;i < 8;i ++)
		{
			if(CRC_BYTE & 0x80)
			{
				CRC_BYTE <<= 1;
				CRC_BYTE ^= 0x31;
			}
			else
			{
				CRC_BYTE <<= 1;
			}
		}

		if(t == 0)
		{
			temp = DAT & 0xFF;
		}
	}

	return CRC_BYTE;
}

/**
 * @brief  微秒延时函数
 * @param  us 要延时多少微秒
 * @retval 无
 */
void HAL_Delay_us(uint32_t n)
{      uint32_t ticks;
    uint32_t told;
    uint32_t tnow;
    uint32_t tcnt = 0;
    uint32_t reload;
       
	reload = SysTick->LOAD;                
    ticks = n * (SystemCoreClock / 1000000);	 /* 需要的节拍数 */  
    
    tcnt = 0;
    told = SysTick->VAL;             /* 刚进入时的计数器值 */

    while (1)
    {
        tnow = SysTick->VAL;    
        if (tnow != told)
        {    
            /* SYSTICK是一个递减的计数器 */    
            if (tnow < told)
            {
                tcnt += told - tnow;    
            }
            /* 重新装载递减 */
            else
            {
                tcnt += reload - tnow + told;    
            }        
            told = tnow;

            /* 时间超过/等于要延迟的时间,则退出 */
            if (tcnt >= ticks)
            {
            	break;
            }
        }  
    }
}

/*发送起始信号*/
void SHT3x_I2C_START(){
    SHT3x_W_SDA(1);
	SHT3x_W_SCL(1);
	//CPU_TS_Tmr_Delay_US(1);
	SHT3x_W_SDA(0);
	//CPU_TS_Tmr_Delay_US(1);
	SHT3x_W_SCL(0);
	//CPU_TS_Tmr_Delay_US(1);
}

/*发送停止信号*/
void SHT3x_I2C_STOP(){
    SHT3x_W_SDA(0);
	SHT3x_W_SCL(1);
	//CPU_TS_Tmr_Delay_US(1);
	SHT3x_W_SDA(1);
	//CPU_TS_Tmr_Delay_US(1);
}

/**
 * @brief  等待应答信号
 * @param  无
 * @retval 1-非应答信号，0-应答信号
 */
uint8_t WaitAck(void)
{
	uint8_t ret;
	
	SHT3x_W_SCL(1);
	//CPU_TS_Tmr_Delay_US(1);
	if(SHT3x_R_SDA()){
		ret=1;
	}
	else{
		ret=0;
	}
	SHT3x_W_SCL(0);
	//CPU_TS_Tmr_Delay_US(1);
  	return ret;
}

/**
 * @brief  I2C发送一个字节
 * @param  Byte 要发送的一个字节
 * @retval 无
 */
void SHT3x_I2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		SHT3x_W_SDA((Byte<<i) & 0x80 );
		SHT3x_W_SCL(1);
		//CPU_TS_Tmr_Delay_US(1);
		SHT3x_W_SCL(0);
		//CPU_TS_Tmr_Delay_US(1);
	}
	SHT3x_W_SDA(1);//释放SDA总线
}

/**
  * @brief  发送两个字节数据
  * @param  MSB 高8位
  * @param  LSB 低8位
  * @retval 无
  */
void SHT3x_WriteByte(uint8_t MSB,uint8_t LSB)
{
	SHT3x_I2C_START();  //发送起始信号
	
	SHT3x_I2C_SendByte(SHT3x_ADDRESS);    //发送设备写地址
	WaitAck();

    SHT3x_I2C_SendByte(MSB);//发送高8位数据
	WaitAck();
   
	SHT3x_I2C_SendByte(LSB);//发送低8位数据	
	WaitAck();

	SHT3x_I2C_STOP();
}

/**
 * @brief  I2C读取一个字节
 * @param  NACK 1-非应答信号，0-应答信号
 * @retval 读取到的字节数据
 */
uint8_t SHT3x_ReadData(uint8_t NACK)
{
    uint8_t i,Byte=0;
	SHT3x_W_SDA(1);//释放SDA总线
	for (i = 0; i < 8; i++)
	{
		SHT3x_W_SCL(1);
		//CPU_TS_Tmr_Delay_US(1);
		Byte=Byte|(SHT3x_R_SDA()<<(7-i));
		//CPU_TS_Tmr_Delay_US(1);
		SHT3x_W_SCL(0);
		//CPU_TS_Tmr_Delay_US(1);
	}
    SHT3x_W_SDA(NACK);//发送应答/非应答信号
	SHT3x_W_SCL(1);
	//CPU_TS_Tmr_Delay_US(1);
	SHT3x_W_SCL(0);
	//CPU_TS_Tmr_Delay_US(1);
	SHT3x_W_SDA(1);//释放SDA总线
	return Byte;
}	

/*软件复位SHT3x*/
void SHT3x_SoftReset(void)                    
{
    SHT3x_WriteByte(0x30,0xA2);    //重新初始化SHT3x
}

/*引脚初始化*/
void SHT3x_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_SET);
	/*Configure GPIO pins : PCPin PCPin */
	GPIO_InitStruct.Pin = SCL_Pin ;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);
		/*Configure GPIO pins : PDPin PCPin */
	GPIO_InitStruct.Pin = SDA_Pin ;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);

	SHT3x_W_SCL(1);
	SHT3x_W_SDA(1);

	SHT3x_WriteByte(0X27,0X37); //周期数据采集模式（每秒10次，Medium Repeatability）

	CPU_TS_Tmr_Delay_US(300);//延时300微秒
}

/**
  * @brief  读取SHT3x数据
  * @param  *Hum 湿度
  * @param  *Temp 温度
  * @retval 1 - 读取成功；0 - 读取失败
  */
uint8_t ReadSHT3x(float *Hum,float *Temp)
{
    uint16_t HumData,TempData,HumCRC,TempCRC;//声明变量存放读取的数据
    
    SHT3x_WriteByte(0XE0,0X00);   //发送指令，获取数据，周期数据采集模式用

    //SHT3x_WriteByte(0X2C,0X0D); //发送单次测量指令（启动时钟延伸，Medium Repeatability）
	
    SHT3x_I2C_START();	//发送起始信号
    
	SHT3x_I2C_SendByte(SHT3x_ADDRESS|0x01);//发送设备读地址
	WaitAck();
	SHT3x_W_SCL(1);	//释放SCL总线
	while (!SHT3x_R_SCL());	//等待从机释放SCL总线

    TempData = SHT3x_ReadData(0);	//读取温度高8位数据
    TempData=TempData<<8;       	//左移8位
	TempData |= SHT3x_ReadData(0);	//读取温度低8位数据
    TempCRC = SHT3x_ReadData(0); 	//读取温度CRC校验数据
	
    HumData = SHT3x_ReadData(0); 	//读取湿度高8位数据
    HumData=HumData<<8;         	//左移8位
	HumData |= SHT3x_ReadData(0);	//读取湿度低8位数据
    HumCRC = SHT3x_ReadData(1);  	//读取湿度CRC校验数据

    SHT3x_I2C_STOP();   //发送停止信号
		

    if( SHT3x_CRC_CAL(HumData)==HumCRC && SHT3x_CRC_CAL(TempData)==TempCRC ){   //对接收到数据进行CRC校验
       *Hum = (float)HumData*100/(65536-1);        //将接收的16位二进制数据转换为10进制湿度数据
       *Temp = (float)TempData*175/(65536-1)-45;   //将接收的16位二进制数据转换为10进制温度数据
       return 1;
    }
    return 0;
}

float GetTemp(void)
{	
	float tempValue=0,humValue=0;
	if(ReadSHT3x(&humValue,&tempValue)==1)
	{
		//ReadSHT3x(&tempValue,&humValue);
		//usartflag=1;
		//myprintf("temp:  %.3f \r\n",tempValue);
		//delay_ms(10);
	}
	return tempValue;

}

