#include "MCP.h"
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "includes.h"
#include "misc.h"

void delay(__IO uint32_t nTime)
{
	nTime = 16*nTime;
	while(nTime) 
	{ 
	 nTime--; 
	} 
}

//����I2C�ٶȵ���ʱ
void MCP_Delay(void)
{
	delay(1);
} 

//MCPоƬI2C�ӿڳ�ʼ�� 
void MCP_I2C_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;				       
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);	 

	GPIO_SetBits(GPIOE, GPIO_Pin_2);	   //
	GPIO_SetBits(GPIOE, GPIO_Pin_3);
}

void MCP_SDA_IN(void)  
{
  GPIO_InitTypeDef  GPIO_InitStructure;  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	     
  GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN_FLOATING; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void MCP_SDA_OUT(void) {
  GPIO_InitTypeDef  GPIO_InitStructure; 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP ;//  
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}

//����I2C��ʼ�ź�
void MCP_I2C_Start(void)
{

	MCP_SDA_OUT();     //sda�����
	MCP_I2C_SDA_H;	  	  
	MCP_I2C_SCL_H;
	delay(3);
 	MCP_I2C_SDA_L;     
	MCP_Delay();
	MCP_I2C_SCL_L;     
}	  
//����I2Cֹͣ�ź�
void MCP_I2C_Stop(void)
{
	MCP_SDA_OUT();//sda�����
	MCP_I2C_SDA_L;
	MCP_Delay();
	MCP_I2C_SCL_H;
	delay(3);
	MCP_I2C_SDA_H;
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 MCP_I2C_Wait_Ack(void)
{
	u8 ucErrTime=0;
	MCP_SDA_IN();      //SDA����Ϊ����  
	MCP_I2C_SDA_H;	   
	MCP_I2C_SCL_H;
	MCP_Delay();
	while(MCP_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			MCP_I2C_Stop();
			return 1;
		} 
		MCP_Delay();
	}
	MCP_I2C_SCL_L;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void MCP_I2C_Ack(void)
{
	MCP_I2C_SCL_L;
	MCP_SDA_OUT();
	MCP_Delay();
	MCP_I2C_SDA_L;
	MCP_Delay();
	MCP_I2C_SCL_H;
	MCP_Delay();
	MCP_I2C_SCL_L;
}
//������ACKӦ��		    
void MCP_I2C_NAck(void)
{
	MCP_I2C_SCL_L;
	MCP_SDA_OUT();
	MCP_Delay();
	MCP_I2C_SDA_H;
	MCP_Delay();
	MCP_I2C_SCL_H;
	MCP_Delay();
	MCP_I2C_SCL_L;
}					 				     
//I2C����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void MCP_I2C_Send_Byte(u8 txd)
{                        
  u8 t;   
	MCP_SDA_OUT(); 	    
  MCP_I2C_SCL_L;//����ʱ�ӿ�ʼ���ݴ���
	MCP_Delay();
	for(t=0;t<8;t++)
	{              
		if(txd&0x80) MCP_I2C_SDA_H;
		else MCP_I2C_SDA_L;
		txd<<=1;
		MCP_Delay();		
		MCP_I2C_SCL_H; 
		MCP_Delay();
		MCP_I2C_SCL_L;	
		MCP_Delay();
	}	 
} 	
    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 MCP_I2C_Read_Byte(unsigned char ack)
{
	u8 i,receive=0;
 	MCP_SDA_IN();//SDA����Ϊ����
	delay(1);
	for(i=0;i<8;i++ )
	{ 
		MCP_I2C_SCL_L; 	    	   
		MCP_Delay();
		MCP_I2C_SCL_H;	 
		receive<<=1;
		if(MCP_READ_SDA)receive++;   
	}	  				 
	if (!ack)MCP_I2C_NAck();   //����nACK
	else MCP_I2C_Ack();        //����ACK   
 	return receive;
}

void MCP_ResistanceSet(u8 resistance)
{
	MCP_I2C_Start();	
	MCP_I2C_Send_Byte(MCP1_WR);   	    //���Ͷ�оƬ1д������� 	 
	MCP_I2C_Wait_Ack();
	MCP_I2C_Send_Byte(Wiper0_Address);  //�Ĵ���Wiper0�ĵ�ַ
	MCP_I2C_Wait_Ack();  
	MCP_I2C_Send_Byte(resistance);            //����ֵ���ã�0x00~0x80
	MCP_I2C_Wait_Ack(); 
	MCP_I2C_Stop();					            //����һ��ֹͣ����
}

