#include "includes.h"
#include "misc.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_adc.h"
#include "mySystem.h"
#include "MCP.h"
#include "AD.h"
#include "demo.h"

extern void Time_init(void);
extern void FSMC_LCD_Init(void); 
extern void LCD_Init_SD(void);
extern unsigned char GT9147_Init(void);
extern unsigned char GT9147_Scan(void);
extern void SD_TEST(void);
extern void OutPutFile(char ImageName);
extern void TFT_ShowString(u16 x,u16 y,const u8 *p,u16 Color);
extern void FillColor(u16 x,u16 y, u16 x1, u16 y1, u16 z);
extern void Delay(__IO uint32_t nCount);

void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);

extern unsigned short TP_P[5][2];          //触摸点坐标值
extern int Channel; 
int TimCount = 0;                          //工作时间记录
char Page = '1';
	
/****************************************************************************
* 名    称：int main(void)
* 功    能：奋斗板初始化函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/  
int main(void)
{
	unsigned char res     = 0;		 //触摸标志位
	unsigned char setFlag = 0;		 //脉冲设置标志位
	
	RCC_Configuration();  	       //系统时钟初始化及端口外设时钟使能
	NVIC_Configuration();					 //中断源配置
	Time_init();                   //定时器初始化
  GPIO_Configuration();			     //GPIO设置
  RCC_MCOConfig(RCC_MCO_PLLCLK_Div2); 	
	MCP_I2C_Init();                //MCP IIC初始化
  FSMC_LCD_Init();				       //FSMC TFT接口初始化 
  LCD_Init_SD();					       //LCD初始化代码
	GT9147_Init();                 //触摸芯片初始化
	ADC_Configuration();           //AD初始化

	SD_TEST();                     //SD卡检测
	OutPutFile('2');               //显示界面图形
	SysInit();                     //系统输出初始化
	ShowInterfaceTwo();
	
  while(1)                              
  { 
		//POINT_COLOR = 0;
		//TFT_ShowString(396,250," SET ",WHITE);
		res = GT9147_Scan();        //触摸检测
		if(res) 
		{
			if(Page == '0')
				TouchHandle_PageOne(TP_P[0][0],TP_P[0][1]);  //检测到触摸则进行处理
			else if(Page == '1')
				TouchHandle_PageTwo(TP_P[0][0],TP_P[0][1]);
			//防止误触
			do{
			res = GT9147_Scan();
			}while(res);
			Delay(0xaaaa); 
		}
		setFlag = set_flag();
		if(!setFlag);
		else if(KeyStart)   //Start
		{		  
	  	Delay(0x3ffff);										  
			if(KeyStart)
			{		  //按键消抖动
				while(KeyStart);//是否松开按键
				//POINT_COLOR = 0;
				//TFT_ShowString(396,250," WORK ",WHITE);
				TimCount = 0;        //倒计时开始
				Clear();
				WorkMark();
				PowerEnable;
				Delay(0x3ffff);
				while(1)
				{	
					if(TimCount>600000)    //判断工作时间是否到达30min，若是则停止脉冲发射
					{
						TimCount = 0;
						PowerNEnable;
						Clear();
						if(Page == '0')
						{
							ReadyMark();
							TouchHandle_PageOne(100,60);
							FillColor(52,56,65,70,BLUE);
						}
						else if(Page == '1')
						{
							ReadyMark();
						}					
						//TFT_ShowString(396,250," SET ",WHITE);
						break;
					}					
					else if(KeyStart)  //Stop
					{		   
						Delay(0x3ffff);										  
						if(KeyStart)
						{		  //按键消抖动
							while(KeyStart);//是否松开按键
							PowerNEnable;
							Clear();
							if(Page == '0')
							{
								ReadyMark();
								TouchHandle_PageOne(100,60);
								FillColor(52,56,65,70,BLUE);
							}
							else if(Page == '1')
							{
								ReadyMark();
							}					
							//TFT_ShowString(396,250," SET ",WHITE);
							break;
						}
					}				
				}
			}
	  }
		if(PageTurning)  //Stop
		{		   
			Delay(0x3ffff);										  
			if(PageTurning)
			{		  //按键消抖动
				while(PageTurning);//是否松开按键
				SysInit();
				if(Page == '0')
					Page = '1';
				else
					Page = '0';
				
				if(Page == '0')
				{	
					OutPutFile('1');
					ShowInterfaceOne();
				}
				else if(Page == '1')
				{
					OutPutFile('2');
					ShowInterfaceTwo();	
				}
			}
		}	
  }	
}
/****************************************************************************
* 名    称：void RCC_Configuration(void)
* 功    能：系统时钟配置为72MHZ， 外设时钟配置
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void RCC_Configuration(void){

  SystemInit(); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC 
  						| RCC_APB2Periph_GPIOD| RCC_APB2Periph_GPIOE , ENABLE);

} 
/****************************************************************************
* 名    称：void NVIC_Configuration(void)
* 功    能：中断源配置
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure_TIM3;
	NVIC_InitTypeDef NVIC_InitStructure_TIM2;
	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

  NVIC_InitStructure_TIM3.NVIC_IRQChannel =TIM3_IRQn ;			   //配置定时器中断 
	NVIC_InitStructure_TIM2.NVIC_IRQChannel =TIM2_IRQn ;
	
  NVIC_InitStructure_TIM3.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure_TIM3.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure_TIM3.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure_TIM3);  

  NVIC_InitStructure_TIM2.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure_TIM2.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure_TIM2.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure_TIM2);
}
/****************************************************************************
* 名    称：void GPIO_Configuration(void)
* 功    能：通用IO口配置
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：
****************************************************************************/ 
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			 //通用推挽输出模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //输出模式最大速度50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_8 | GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_15;				    
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			 //通用推挽输出模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //输出模式最大速度50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | 
	GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;				   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			 //通用推挽输出模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //输出模式最大速度50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			 //通用推挽输出模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //输出模式最大速度50MHz
  GPIO_Init(GPIOD, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			 //通用推挽输出模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //输出模式最大速度50MHz
  GPIO_Init(GPIOE, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				    
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			 //复用推挽输出模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //输出模式最大速度50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3 , ENABLE);		
	
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;				 //触摸检测引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;				
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//浮空输入
  GPIO_Init(GPIOE, &GPIO_InitStructure); 
}

