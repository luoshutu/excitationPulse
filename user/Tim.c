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
* 名    称：void time_ini(void)
* 功    能：TIM初始化
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void Time_init(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);		    //定时器3 时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
  /* Time Base configuration */
  /*-------------------------------------------------------------------
  TIMCLK=72MHz  预分频系数Prescaler=63 经过分频 定时器时钟为1.125MHz
  捕获/比较寄存器2 TIM_CCR2= CCR2_Val 
  2通道产生的更新频率是=1.125MHz/CCR2_Val=100Hz 
  -------------------------------------------------------------------*/
  TIM3_TimeBaseStructure.TIM_Prescaler = 63;						//预分频器TIM3_PSC=63	 
  TIM3_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//计数器向上计数模式 TIM3_CR1[4]=0
  TIM3_TimeBaseStructure.TIM_Period =0xffff;				        //自动重装载寄存器TIM3_APR      		     
  TIM3_TimeBaseStructure.TIM_ClockDivision = 0x0;					//时钟分频因子 TIM3_CR1[9:8]=00

  TIM_TimeBaseInit(TIM3,&TIM3_TimeBaseStructure);					//写TIM3各寄存器参数
 
  TIM_Cmd(TIM3,ENABLE);											//启动定时器3 TIM3_CR1[0]=1;  
  TIM_ITConfig(TIM3,TIM_IT_CC3,ENABLE); 							//TIM3_DIER[2]=1  允许捕获/比较2中断
	
	//TIM2配置
	TIM2_TimeBaseStructure.TIM_Prescaler = 63;					
  TIM2_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		
  TIM2_TimeBaseStructure.TIM_Period =0xffff;				       
  TIM2_TimeBaseStructure.TIM_ClockDivision = 0x0;				

  TIM_TimeBaseInit(TIM2,&TIM2_TimeBaseStructure);				
 
  TIM_Cmd(TIM2,ENABLE);										
  TIM_ITConfig(TIM2,TIM_IT_CC2,ENABLE); 
	
	//TIM4配置
	TIM4_TimeBaseStructure.TIM_Prescaler = 2;					
  TIM4_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		
  
	TIM4_TimeBaseStructure.TIM_Period =2;				       
  TIM4_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM4_TimeBaseStructure.TIM_RepetitionCounter = 0x0;	
  TIM_TimeBaseInit(TIM4,&TIM4_TimeBaseStructure);				
 
  TIM4_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 			    //PWM模式2 TIM4_CCMR1[14:12]=111 在向上计数时，
  																    //一旦TIMx_CNT<TIMx_CCR1时通道1为无效电平，否则为有效电平
  TIM4_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //输入/捕获2输出允许  OC2信号输出到对应的输出引脚PB9
  TIM4_OCInitStructure.TIM_Pulse = 1; 					    //确定占空比，这个值决定了有效电平的时间。
  TIM4_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 	    //输出极性  低电平有效 TIM3_CCER[5]=1;

  TIM_OC4Init(TIM4, &TIM4_OCInitStructure);
  TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
 
  TIM_Cmd(TIM4,ENABLE);										
  TIM_ITConfig(TIM4,TIM_IT_CC4,ENABLE); 
}

