#include "gti2c.h"

extern void delay_us(__IO uint32_t nTime);
extern void GT_SDA_OUT(void);
extern void GT_SDA_IN(void);

//����I2C�ٶȵ���ʱ
void GT_Delay(void)
{
	delay_us(1);
} 

//���ݴ���оƬGT9147 ��I2C�ӿڳ�ʼ��
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
//����I2C��ʼ�ź�
void GT_I2C_Start(void)
{
	GT_SDA_OUT();     //sda�����
	GT_I2C_SDA_H;	  	  
	GT_I2C_SCL_H;
	delay_us(3);
 	GT_I2C_SDA_L;     
	GT_Delay();
	GT_I2C_SCL_L;     
}	  
//����I2Cֹͣ�ź�
void GT_I2C_Stop(void)
{
	GT_SDA_OUT();//sda�����
	GT_I2C_SCL_H;
	delay_us(3);
	GT_I2C_SDA_L;
	GT_Delay();
	GT_I2C_SDA_H;
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 GT_I2C_Wait_Ack(void)
{
	u8 ucErrTime=0;
	GT_SDA_IN();      //SDA����Ϊ����  
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
	GT_I2C_SCL_L;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//I2C����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void GT_I2C_Send_Byte(u8 txd)
{                        
  u8 t;   
	GT_SDA_OUT(); 	    
  GT_I2C_SCL_L;//����ʱ�ӿ�ʼ���ݴ���
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
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 GT_I2C_Read_Byte(unsigned char ack)
{
	u8 i,receive=0;
 	GT_SDA_IN();//SDA����Ϊ����
	delay_us(1);
	for(i=0;i<8;i++ )
	{ 
		GT_I2C_SCL_L; 	    	   
		GT_Delay();
		GT_I2C_SCL_H;	 
		receive<<=1;
		if(GT_READ_SDA)receive++;   
	}	  				 
	if (!ack)GT_I2C_NAck();   //����nACK
	else GT_I2C_Ack();        //����ACK   
 	return receive;
}
/******************* (C) COPYRIGHT 2015 �ܶ�STM32 *****END OF FILE****/
