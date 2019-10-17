#include "gt9147.h"
#include "gti2c.h"	   
#include "string.h" 

unsigned short TP_P[5][2] = {0};  				 
unsigned char GT9147_Init(void);
unsigned char GT9147_Init(void);
unsigned char GT9147_Scan(void);

void GT_SDA_OUT(void);
void GT_SDA_IN(void);
void delay_us(__IO uint32_t nTime);

void GT_SDA_IN(void)  
{
  GPIO_InitTypeDef  GPIO_InitStructure;  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	     
  GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN_FLOATING; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void GT_SDA_OUT(void) {
  GPIO_InitTypeDef  GPIO_InitStructure; 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP ;//  
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}


const unsigned char GT9147_CONFIG_TB[]=
{ 
	0X60,0XE0,0X01,0X20,0X03,0X05,0X35,0X00,0X02,0X08,
	0X1E,0X08,0X50,0X3C,0X0F,0X05,0X00,0X00,0XFF,0X67,
	0X50,0X00,0X00,0X18,0X1A,0X1E,0X14,0X89,0X28,0X0A,
	0X30,0X2E,0XBB,0X0A,0X03,0X00,0X00,0X02,0X33,0X1D,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X32,0X00,0X00,
	0X2A,0X1C,0X5A,0X94,0XC5,0X02,0X07,0X00,0X00,0X00,
	0XB5,0X1F,0X00,0X90,0X28,0X00,0X77,0X32,0X00,0X62,
	0X3F,0X00,0X52,0X50,0X00,0X52,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,
	0X0F,0X03,0X06,0X10,0X42,0XF8,0X0F,0X14,0X00,0X00,
	0X00,0X00,0X1A,0X18,0X16,0X14,0X12,0X10,0X0E,0X0C,
	0X0A,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X29,0X28,0X24,0X22,0X20,0X1F,0X1E,0X1D,
	0X0E,0X0C,0X0A,0X08,0X06,0X05,0X04,0X02,0X00,0XFF,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
	0XFF,0XFF,0XFF,0XFF,
}; 
/****************************************************************************
* ��    �ƣ�void I2C_delay(void)
* ��    �ܣ�I2C ������ʱ����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
void delay_us(__IO uint32_t nTime)
{	
  
   while(nTime) 
   { 
     nTime--; 
   } 
}
//��GT9147д��һ������
//reg:��ʼ�Ĵ�����ַ
//buf:���ݻ�������
//len:д���ݳ���
//����ֵ:0���ɹ�;1��ʧ��.
unsigned char GT9147_WR_Reg(unsigned short reg,unsigned char *buf,unsigned char len)
{
	unsigned char i;
	unsigned char res=0;
	GT_I2C_Start();	
 	GT_I2C_Send_Byte(GT_CMD_WR);   	//����д���� 	 
	GT_I2C_Wait_Ack();
	GT_I2C_Send_Byte(reg>>8);   	//���͸�8λ��ַ
	GT_I2C_Wait_Ack(); 	 										  		   
	GT_I2C_Send_Byte(reg&0XFF);   	//���͵�8λ��ַ
	GT_I2C_Wait_Ack();  
	for(i=0;i<len;i++)
	{	   
    GT_I2C_Send_Byte(buf[i]);  	//������
		res=GT_I2C_Wait_Ack();
		if(res) break;  
	}
  GT_I2C_Stop();					//����һ��ֹͣ����	    
	return res; 
}
///////////////////////////////////////////////
//��GT9147������
//reg: �Ĵ�����ַ
//buf: ���ݻ�������
//len: �����ݳ���	
///////////////////////////////////////////////		  
void GT9147_RD_Reg(unsigned short reg,unsigned char *buf,unsigned char len)
{
	unsigned char i=0; 
 	GT_I2C_Start();	
 	GT_I2C_Send_Byte(GT_CMD_WR);    //����дָ�� 	 
	GT_I2C_Wait_Ack();
 	GT_I2C_Send_Byte(reg>>8);   	//���͸�8λ��ַ
	GT_I2C_Wait_Ack(); 	 										  		   
 	GT_I2C_Send_Byte(reg&0XFF);   	//���͵�8λ��ַ
	GT_I2C_Wait_Ack();  
 	GT_I2C_Start();  	 	   
	GT_I2C_Send_Byte(GT_CMD_RD);    //���Ͷ�ָ��		   
	GT_I2C_Wait_Ack();	   
	for(i=0;i<len;i++)
	{	   
    buf[i]=GT_I2C_Read_Byte(i==(len-1)?0:1); //������	
	} 
  GT_I2C_Stop();                  //����һ��ֹͣ����    
} 

//////////////////////////////////////////////
//��ʼ��GT9147���ݴ�����
//����ֵ:0���ɹ�;1��ʧ��
//////////////////////////////////////////////
unsigned char GT9147_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;				                 //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	 
	GPIO_SetBits(GPIOB, GPIO_Pin_6);	
	GPIO_SetBits(GPIOB, GPIO_Pin_7); //GT9147 ��λ����  

	GT_I2C_Init();      	//��ʼ����������I2C����  
	GT_RST_L;				//���ݴ���оƬ��λ
	delay_us(1000000);
 	GT_RST_H;				//�ͷŸ�λ 
	delay_us(1000000);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	        //GT9147�ж��ź��������			               
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;         
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	 
 	delay_us(1000000);

	return 0;
}

////////////////////////////////////////////////////////////////
//ɨ�败����(��ѯ��ʽ)
//mode:0 ����ɨ��
//����ֵ:��ǰ����״̬ 0�������޴���;1�������д���
////////////////////////////////////////////////////////////////
unsigned char GT9147_Scan(void)
{	
	unsigned char buf[7]={0};
	unsigned char res=0;
	 
	GT9147_RD_Reg(GT_TP_REG,buf,7);	        //��ȡXY����ֵ
	if(buf[2] == 1)
	{
		TP_P[0][0] = ((unsigned short)(buf[3]&0x01)<<8) + buf[4];  //X����
		TP_P[0][1] = ((unsigned short)buf[5]<<8) + buf[6];  //Y����	
		res = 1;	
	}
	else res = 0;

	return res;
}
 
