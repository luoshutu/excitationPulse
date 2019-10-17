#ifndef __MCP_H
#define __MCP_H

#include "stm32f10x.h"

//MCP I2C通信引脚
#define MCP_I2C_SCL_H    GPIO_SetBits(GPIOE, GPIO_Pin_2)	   //输出SCL高电平     
#define MCP_I2C_SCL_L    GPIO_ResetBits(GPIOE, GPIO_Pin_2)   //输出SCL低电平   	
#define MCP_I2C_SDA_H    GPIO_SetBits(GPIOE, GPIO_Pin_3)	   //输出SDA高电平	 
#define MCP_I2C_SDA_L    GPIO_ResetBits(GPIOE, GPIO_Pin_3)   //输出SDA低电平	

#define MCP_READ_SDA      GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)    //输入SDA 

//I2C读写命令	
#define MCP1_WR 		0X58     	//写命令
#define MCP1_RD 		0X59		  //读命令
#define MCP2_WR 		0X5A     	//写命令
#define MCP2_RD 		0X5B		  //读命令

//Wiper
#define Wiper0_Address	0X00
#define Wiper1_Address	0X11
#define Wiper2_Address	0X61

//I2C函数定义
void MCP_I2C_Init(void);                    //初始化I2C的IO口线				 
void MCP_I2C_Start(void);					          //发送I2C开始信号
void MCP_I2C_Stop(void);	  					      //发送I2C停止信号
void MCP_I2C_Send_Byte(unsigned char txd);	//I2C发送一个字节
u8 MCP_I2C_Read_Byte(unsigned char ack);		//I2C读取一个字节
u8 MCP_I2C_Wait_Ack(void); 					        //I2C等待ACK信号
void MCP_I2C_Ack(void);						          //I2C发送ACK信号
void MCP_I2C_NAck(void);						        //I2C不发送ACK信号
void MCP_ResistanceSet(u8 resistance);                   //MCP芯片的电阻值设置

#endif
