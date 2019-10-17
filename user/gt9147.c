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
* 名    称：void I2C_delay(void)
* 功    能：I2C 控制延时函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
void delay_us(__IO uint32_t nTime)
{	
  
   while(nTime) 
   { 
     nTime--; 
   } 
}
//向GT9147写入一次数据
//reg:起始寄存器地址
//buf:数据缓缓存区
//len:写数据长度
//返回值:0：成功;1：失败.
unsigned char GT9147_WR_Reg(unsigned short reg,unsigned char *buf,unsigned char len)
{
	unsigned char i;
	unsigned char res=0;
	GT_I2C_Start();	
 	GT_I2C_Send_Byte(GT_CMD_WR);   	//发送写命令 	 
	GT_I2C_Wait_Ack();
	GT_I2C_Send_Byte(reg>>8);   	//发送高8位地址
	GT_I2C_Wait_Ack(); 	 										  		   
	GT_I2C_Send_Byte(reg&0XFF);   	//发送低8位地址
	GT_I2C_Wait_Ack();  
	for(i=0;i<len;i++)
	{	   
    GT_I2C_Send_Byte(buf[i]);  	//发数据
		res=GT_I2C_Wait_Ack();
		if(res) break;  
	}
  GT_I2C_Stop();					//产生一个停止条件	    
	return res; 
}
///////////////////////////////////////////////
//从GT9147读数据
//reg: 寄存器地址
//buf: 数据缓缓存区
//len: 读数据长度	
///////////////////////////////////////////////		  
void GT9147_RD_Reg(unsigned short reg,unsigned char *buf,unsigned char len)
{
	unsigned char i=0; 
 	GT_I2C_Start();	
 	GT_I2C_Send_Byte(GT_CMD_WR);    //发送写指令 	 
	GT_I2C_Wait_Ack();
 	GT_I2C_Send_Byte(reg>>8);   	//发送高8位地址
	GT_I2C_Wait_Ack(); 	 										  		   
 	GT_I2C_Send_Byte(reg&0XFF);   	//发送低8位地址
	GT_I2C_Wait_Ack();  
 	GT_I2C_Start();  	 	   
	GT_I2C_Send_Byte(GT_CMD_RD);    //发送读指令		   
	GT_I2C_Wait_Ack();	   
	for(i=0;i<len;i++)
	{	   
    buf[i]=GT_I2C_Read_Byte(i==(len-1)?0:1); //发数据	
	} 
  GT_I2C_Stop();                  //产生一个停止条件    
} 

//////////////////////////////////////////////
//初始化GT9147电容触摸屏
//返回值:0：成功;1：失败
//////////////////////////////////////////////
unsigned char GT9147_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;				                 //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	 
	GPIO_SetBits(GPIOB, GPIO_Pin_6);	
	GPIO_SetBits(GPIOB, GPIO_Pin_7); //GT9147 复位引脚  

	GT_I2C_Init();      	//初始化电容屏的I2C总线  
	GT_RST_L;				//电容触摸芯片复位
	delay_us(1000000);
 	GT_RST_H;				//释放复位 
	delay_us(1000000);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	        //GT9147中断信号输出引脚			               
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;         
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	 
 	delay_us(1000000);

	return 0;
}

////////////////////////////////////////////////////////////////
//扫描触摸屏(查询方式)
//mode:0 正常扫描
//返回值:当前触屏状态 0：触屏无触摸;1：触屏有触摸
////////////////////////////////////////////////////////////////
unsigned char GT9147_Scan(void)
{	
	unsigned char buf[7]={0};
	unsigned char res=0;
	 
	GT9147_RD_Reg(GT_TP_REG,buf,7);	        //读取XY坐标值
	if(buf[2] == 1)
	{
		TP_P[0][0] = ((unsigned short)(buf[3]&0x01)<<8) + buf[4];  //X坐标
		TP_P[0][1] = ((unsigned short)buf[5]<<8) + buf[6];  //Y坐标	
		res = 1;	
	}
	else res = 0;

	return res;
}
 
