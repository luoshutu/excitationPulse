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

extern unsigned short TP_P[5][2];          //����������ֵ
extern int Channel; 
int TimCount = 0;                          //����ʱ���¼
char Page = '1';
	
/****************************************************************************
* ��    �ƣ�int main(void)
* ��    �ܣ��ܶ����ʼ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/  
int main(void)
{
	unsigned char res     = 0;		 //������־λ
	unsigned char setFlag = 0;		 //�������ñ�־λ
	
	RCC_Configuration();  	       //ϵͳʱ�ӳ�ʼ�����˿�����ʱ��ʹ��
	NVIC_Configuration();					 //�ж�Դ����
	Time_init();                   //��ʱ����ʼ��
  GPIO_Configuration();			     //GPIO����
  RCC_MCOConfig(RCC_MCO_PLLCLK_Div2); 	
	MCP_I2C_Init();                //MCP IIC��ʼ��
  FSMC_LCD_Init();				       //FSMC TFT�ӿڳ�ʼ�� 
  LCD_Init_SD();					       //LCD��ʼ������
	GT9147_Init();                 //����оƬ��ʼ��
	ADC_Configuration();           //AD��ʼ��

	SD_TEST();                     //SD�����
	OutPutFile('2');               //��ʾ����ͼ��
	SysInit();                     //ϵͳ�����ʼ��
	ShowInterfaceTwo();
	
  while(1)                              
  { 
		//POINT_COLOR = 0;
		//TFT_ShowString(396,250," SET ",WHITE);
		res = GT9147_Scan();        //�������
		if(res) 
		{
			if(Page == '0')
				TouchHandle_PageOne(TP_P[0][0],TP_P[0][1]);  //��⵽��������д���
			else if(Page == '1')
				TouchHandle_PageTwo(TP_P[0][0],TP_P[0][1]);
			//��ֹ��
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
			{		  //����������
				while(KeyStart);//�Ƿ��ɿ�����
				//POINT_COLOR = 0;
				//TFT_ShowString(396,250," WORK ",WHITE);
				TimCount = 0;        //����ʱ��ʼ
				Clear();
				WorkMark();
				PowerEnable;
				Delay(0x3ffff);
				while(1)
				{	
					if(TimCount>600000)    //�жϹ���ʱ���Ƿ񵽴�30min��������ֹͣ���巢��
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
						{		  //����������
							while(KeyStart);//�Ƿ��ɿ�����
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
			{		  //����������
				while(PageTurning);//�Ƿ��ɿ�����
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
* ��    �ƣ�void RCC_Configuration(void)
* ��    �ܣ�ϵͳʱ������Ϊ72MHZ�� ����ʱ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
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
* ��    �ƣ�void NVIC_Configuration(void)
* ��    �ܣ��ж�Դ����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure_TIM3;
	NVIC_InitTypeDef NVIC_InitStructure_TIM2;
	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

  NVIC_InitStructure_TIM3.NVIC_IRQChannel =TIM3_IRQn ;			   //���ö�ʱ���ж� 
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
* ��    �ƣ�void GPIO_Configuration(void)
* ��    �ܣ�ͨ��IO������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/ 
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			 //ͨ���������ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //���ģʽ����ٶ�50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_8 | GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_15;				    
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			 //ͨ���������ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //���ģʽ����ٶ�50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | 
	GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;				   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			 //ͨ���������ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //���ģʽ����ٶ�50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			 //ͨ���������ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //���ģʽ����ٶ�50MHz
  GPIO_Init(GPIOD, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			 //ͨ���������ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //���ģʽ����ٶ�50MHz
  GPIO_Init(GPIOE, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				    
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			 //�����������ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //���ģʽ����ٶ�50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3 , ENABLE);		
	
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;				 //�����������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;				
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//��������
  GPIO_Init(GPIOE, &GPIO_InitStructure); 
}

