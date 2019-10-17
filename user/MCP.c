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

//控制I2C速度的延时
void MCP_Delay(void)
{
	delay(1);
} 

//MCP芯片I2C接口初始化 
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

//产生I2C起始信号
void MCP_I2C_Start(void)
{

	MCP_SDA_OUT();     //sda线输出
	MCP_I2C_SDA_H;	  	  
	MCP_I2C_SCL_H;
	delay(3);
 	MCP_I2C_SDA_L;     
	MCP_Delay();
	MCP_I2C_SCL_L;     
}	  
//产生I2C停止信号
void MCP_I2C_Stop(void)
{
	MCP_SDA_OUT();//sda线输出
	MCP_I2C_SDA_L;
	MCP_Delay();
	MCP_I2C_SCL_H;
	delay(3);
	MCP_I2C_SDA_H;
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 MCP_I2C_Wait_Ack(void)
{
	u8 ucErrTime=0;
	MCP_SDA_IN();      //SDA设置为输入  
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
	MCP_I2C_SCL_L;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
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
//不产生ACK应答		    
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
//I2C发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void MCP_I2C_Send_Byte(u8 txd)
{                        
  u8 t;   
	MCP_SDA_OUT(); 	    
  MCP_I2C_SCL_L;//拉低时钟开始数据传输
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
    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 MCP_I2C_Read_Byte(unsigned char ack)
{
	u8 i,receive=0;
 	MCP_SDA_IN();//SDA设置为输入
	delay(1);
	for(i=0;i<8;i++ )
	{ 
		MCP_I2C_SCL_L; 	    	   
		MCP_Delay();
		MCP_I2C_SCL_H;	 
		receive<<=1;
		if(MCP_READ_SDA)receive++;   
	}	  				 
	if (!ack)MCP_I2C_NAck();   //发送nACK
	else MCP_I2C_Ack();        //发送ACK   
 	return receive;
}

void MCP_ResistanceSet(u8 resistance)
{
	MCP_I2C_Start();	
	MCP_I2C_Send_Byte(MCP1_WR);   	    //发送对芯片1写入的命令 	 
	MCP_I2C_Wait_Ack();
	MCP_I2C_Send_Byte(Wiper0_Address);  //寄存器Wiper0的地址
	MCP_I2C_Wait_Ack();  
	MCP_I2C_Send_Byte(resistance);            //电阻值设置，0x00~0x80
	MCP_I2C_Wait_Ack(); 
	MCP_I2C_Stop();					            //产生一个停止条件
}

