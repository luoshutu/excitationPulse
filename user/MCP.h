#ifndef __MCP_H
#define __MCP_H

#include "stm32f10x.h"

//MCP I2Cͨ������
#define MCP_I2C_SCL_H    GPIO_SetBits(GPIOE, GPIO_Pin_2)	   //���SCL�ߵ�ƽ     
#define MCP_I2C_SCL_L    GPIO_ResetBits(GPIOE, GPIO_Pin_2)   //���SCL�͵�ƽ   	
#define MCP_I2C_SDA_H    GPIO_SetBits(GPIOE, GPIO_Pin_3)	   //���SDA�ߵ�ƽ	 
#define MCP_I2C_SDA_L    GPIO_ResetBits(GPIOE, GPIO_Pin_3)   //���SDA�͵�ƽ	

#define MCP_READ_SDA      GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)    //����SDA 

//I2C��д����	
#define MCP1_WR 		0X58     	//д����
#define MCP1_RD 		0X59		  //������
#define MCP2_WR 		0X5A     	//д����
#define MCP2_RD 		0X5B		  //������

//Wiper
#define Wiper0_Address	0X00
#define Wiper1_Address	0X11
#define Wiper2_Address	0X61

//I2C��������
void MCP_I2C_Init(void);                    //��ʼ��I2C��IO����				 
void MCP_I2C_Start(void);					          //����I2C��ʼ�ź�
void MCP_I2C_Stop(void);	  					      //����I2Cֹͣ�ź�
void MCP_I2C_Send_Byte(unsigned char txd);	//I2C����һ���ֽ�
u8 MCP_I2C_Read_Byte(unsigned char ack);		//I2C��ȡһ���ֽ�
u8 MCP_I2C_Wait_Ack(void); 					        //I2C�ȴ�ACK�ź�
void MCP_I2C_Ack(void);						          //I2C����ACK�ź�
void MCP_I2C_NAck(void);						        //I2C������ACK�ź�
void MCP_ResistanceSet(u8 resistance);                   //MCPоƬ�ĵ���ֵ����

#endif
