#include "gti2c.h"

extern void delay_us(__IO uint32_t nTime);
extern void GT_SDA_OUT(void);
extern void GT_SDA_IN(void);

//控制I2C速度的延时
void GT_Delay(void)
{
	delay_us(1);
} 

//电容触摸芯片GT9147 的I2C接口初始化
void GT_I2C_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;				                 //LED1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	 

	GPIO_SetBits(GPIOA, GPIO_Pin_5);	   //
	GPIO_SetBits(GPIOA, GPIO_Pin_6);
}
//产生I2C起始信号
void GT_I2C_Start(void)
{
	GT_SDA_OUT();     //sda线输出
	GT_I2C_SDA_H;	  	  
	GT_I2C_SCL_H;
	delay_us(3);
 	GT_I2C_SDA_L;     
	GT_Delay();
	GT_I2C_SCL_L;     
}	  
//产生I2C停止信号
void GT_I2C_Stop(void)
{
	GT_SDA_OUT();//sda线输出
	GT_I2C_SCL_H;
	delay_us(3);
	GT_I2C_SDA_L;
	GT_Delay();
	GT_I2C_SDA_H;
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 GT_I2C_Wait_Ack(void)
{
	u8 ucErrTime=0;
	GT_SDA_IN();      //SDA设置为输入  
	GT_I2C_SDA_H;	   
	GT_I2C_SCL_H;
	GT_Delay();
	while(GT_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			GT_I2C_Stop();
			return 1;
		} 
		GT_Delay();
	}
	GT_I2C_SCL_L;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void GT_I2C_Ack(void)
{
	GT_I2C_SCL_L;
	GT_SDA_OUT();
	GT_Delay();
	GT_I2C_SDA_L;
	GT_Delay();
	GT_I2C_SCL_H;
	GT_Delay();
	GT_I2C_SCL_L;
}
//不产生ACK应答		    
void GT_I2C_NAck(void)
{
	GT_I2C_SCL_L;
	GT_SDA_OUT();
	GT_Delay();
	GT_I2C_SDA_H;
	GT_Delay();
	GT_I2C_SCL_H;
	GT_Delay();
	GT_I2C_SCL_L;
}					 				     
//I2C发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void GT_I2C_Send_Byte(u8 txd)
{                        
  u8 t;   
	GT_SDA_OUT(); 	    
  GT_I2C_SCL_L;//拉低时钟开始数据传输
	GT_Delay();
	for(t=0;t<8;t++)
    {              
			if(txd&0x80) GT_I2C_SDA_H;
			else GT_I2C_SDA_L;
			txd<<=1; 	      
			GT_I2C_SCL_H; 
			GT_Delay();
			GT_I2C_SCL_L;	
			GT_Delay();
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 GT_I2C_Read_Byte(unsigned char ack)
{
	u8 i,receive=0;
 	GT_SDA_IN();//SDA设置为输入
	delay_us(1);
	for(i=0;i<8;i++ )
	{ 
		GT_I2C_SCL_L; 	    	   
		GT_Delay();
		GT_I2C_SCL_H;	 
		receive<<=1;
		if(GT_READ_SDA)receive++;   
	}	  				 
	if (!ack)GT_I2C_NAck();   //发送nACK
	else GT_I2C_Ack();        //发送ACK   
 	return receive;
}
/******************* (C) COPYRIGHT 2015 奋斗STM32 *****END OF FILE****/
