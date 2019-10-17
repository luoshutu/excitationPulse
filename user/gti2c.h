
#include "stm32f10x_conf.h"  


//IO操作
	 
#define GT_I2C_SCL_H    GPIO_SetBits(GPIOA, GPIO_Pin_5)	   //输出SCL高电平     
#define GT_I2C_SCL_L    GPIO_ResetBits(GPIOA, GPIO_Pin_5)  //输出SCL低电平   	
#define GT_I2C_SDA_H    GPIO_SetBits(GPIOA, GPIO_Pin_6)	   //输出SDA高电平	 
#define GT_I2C_SDA_L    GPIO_ResetBits(GPIOA, GPIO_Pin_6)  //输出SDA低电平	

#define GT_READ_SDA      GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)    //输入SDA 

//I2C函数定义
void GT_I2C_Init(void);                		//初始化I2C的IO口线				 
void GT_I2C_Start(void);					//发送I2C开始信号
void GT_I2C_Stop(void);	  					//发送I2C停止信号
void GT_I2C_Send_Byte(unsigned char txd);	//I2C发送一个字节
u8 GT_I2C_Read_Byte(unsigned char ack);		//I2C读取一个字节
u8 GT_I2C_Wait_Ack(void); 					//I2C等待ACK信号
void GT_I2C_Ack(void);						//I2C发送ACK信号
void GT_I2C_NAck(void);						//I2C不发送ACK信号








