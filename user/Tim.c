#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include  <stm32f10x.h>

void Time_init(void);
TIM_TimeBaseInitTypeDef  TIM3_TimeBaseStructure;
TIM_BDTRInitTypeDef TIM3_BDTRInitStructure;

TIM_TimeBaseInitTypeDef  TIM2_TimeBaseStructure;
TIM_BDTRInitTypeDef TIM2_BDTRInitStructure;

TIM_TimeBaseInitTypeDef  TIM4_TimeBaseStructure;
TIM_BDTRInitTypeDef TIM4_BDTRInitStructure;
TIM_OCInitTypeDef  TIM4_OCInitStructure;
/****************************************************************************
* ��    �ƣ�void time_ini(void)
* ��    �ܣ�TIM��ʼ��
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void Time_init(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);		    //��ʱ��3 ʱ��ʹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
  /* Time Base configuration */
  /*-------------------------------------------------------------------
  TIMCLK=72MHz  Ԥ��Ƶϵ��Prescaler=63 ������Ƶ ��ʱ��ʱ��Ϊ1.125MHz
  ����/�ȽϼĴ���2 TIM_CCR2= CCR2_Val 
  2ͨ�������ĸ���Ƶ����=1.125MHz/CCR2_Val=100Hz 
  -------------------------------------------------------------------*/
  TIM3_TimeBaseStructure.TIM_Prescaler = 63;						//Ԥ��Ƶ��TIM3_PSC=63	 
  TIM3_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//���������ϼ���ģʽ TIM3_CR1[4]=0
  TIM3_TimeBaseStructure.TIM_Period =0xffff;				        //�Զ���װ�ؼĴ���TIM3_APR      		     
  TIM3_TimeBaseStructure.TIM_ClockDivision = 0x0;					//ʱ�ӷ�Ƶ���� TIM3_CR1[9:8]=00

  TIM_TimeBaseInit(TIM3,&TIM3_TimeBaseStructure);					//дTIM3���Ĵ�������
 
  TIM_Cmd(TIM3,ENABLE);											//������ʱ��3 TIM3_CR1[0]=1;  
  TIM_ITConfig(TIM3,TIM_IT_CC3,ENABLE); 							//TIM3_DIER[2]=1  ������/�Ƚ�2�ж�
	
	//TIM2����
	TIM2_TimeBaseStructure.TIM_Prescaler = 63;					
  TIM2_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		
  TIM2_TimeBaseStructure.TIM_Period =0xffff;				       
  TIM2_TimeBaseStructure.TIM_ClockDivision = 0x0;				

  TIM_TimeBaseInit(TIM2,&TIM2_TimeBaseStructure);				
 
  TIM_Cmd(TIM2,ENABLE);										
  TIM_ITConfig(TIM2,TIM_IT_CC2,ENABLE); 
	
	//TIM4����
	TIM4_TimeBaseStructure.TIM_Prescaler = 2;					
  TIM4_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		
  
	TIM4_TimeBaseStructure.TIM_Period =2;				       
  TIM4_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM4_TimeBaseStructure.TIM_RepetitionCounter = 0x0;	
  TIM_TimeBaseInit(TIM4,&TIM4_TimeBaseStructure);				
 
  TIM4_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 			    //PWMģʽ2 TIM4_CCMR1[14:12]=111 �����ϼ���ʱ��
  																    //һ��TIMx_CNT<TIMx_CCR1ʱͨ��1Ϊ��Ч��ƽ������Ϊ��Ч��ƽ
  TIM4_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //����/����2�������  OC2�ź��������Ӧ���������PB9
  TIM4_OCInitStructure.TIM_Pulse = 1; 					    //ȷ��ռ�ձȣ����ֵ��������Ч��ƽ��ʱ�䡣
  TIM4_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 	    //�������  �͵�ƽ��Ч TIM3_CCER[5]=1;

  TIM_OC4Init(TIM4, &TIM4_OCInitStructure);
  TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
 
  TIM_Cmd(TIM4,ENABLE);										
  TIM_ITConfig(TIM4,TIM_IT_CC4,ENABLE); 
}

