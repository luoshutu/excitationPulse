
#include "stm32f10x_conf.h"  


//IO����
	 
#define GT_I2C_SCL_H    GPIO_SetBits(GPIOA, GPIO_Pin_5)	   //���SCL�ߵ�ƽ     
#define GT_I2C_SCL_L    GPIO_ResetBits(GPIOA, GPIO_Pin_5)  //���SCL�͵�ƽ   	
#define GT_I2C_SDA_H    GPIO_SetBits(GPIOA, GPIO_Pin_6)	   //���SDA�ߵ�ƽ	 
#define GT_I2C_SDA_L    GPIO_ResetBits(GPIOA, GPIO_Pin_6)  //���SDA�͵�ƽ	

#define GT_READ_SDA      GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)    //����SDA 

//I2C��������
void GT_I2C_Init(void);                		//��ʼ��I2C��IO����				 
void GT_I2C_Start(void);					//����I2C��ʼ�ź�
void GT_I2C_Stop(void);	  					//����I2Cֹͣ�ź�
void GT_I2C_Send_Byte(unsigned char txd);	//I2C����һ���ֽ�
u8 GT_I2C_Read_Byte(unsigned char ack);		//I2C��ȡһ���ֽ�
u8 GT_I2C_Wait_Ack(void); 					//I2C�ȴ�ACK�ź�
void GT_I2C_Ack(void);						//I2C����ACK�ź�
void GT_I2C_NAck(void);						//I2C������ACK�ź�








