#include "AD.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"

vu16 ADC_ConvertedValue;

/****************************************************************************
* 名    称：void ADC_Configuration(void)
* 功    能：ADC 配置函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：
****************************************************************************/ 
void ADC_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

    //设置AD模拟输入端口为输入 1路AD 规则通道
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Enable DMA clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

   /* Enable ADC1 and GPIOC clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);

  	/* DMA channel1 configuration ----------------------------------------------*/
	//使能DMA
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;			            //DMA通道1的地址 
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;	            //DMA传送地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;					            //传送方向
	DMA_InitStructure.DMA_BufferSize = 1;								            //传送内存大小，100个16位
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;				            //传送内存地址递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;		//ADC1转换的数据是16位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;				//传送的目的地址是16位宽度
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;									//循环
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    
	/* 允许DMA1通道1传输结束中断 */
	//DMA_ITConfig(DMA1_Channel1,DMA_IT_TC, ENABLE);


	//使能DMA通道1
	DMA_Cmd(DMA1_Channel1, ENABLE); 
  
  
	//ADC配置
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC1工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;		//模数转换工作在扫描模式（多通道）还是单次（单通道）模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//模数转换工作在扫描模式（多通道）还是单次（单通道）模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;               //规定了顺序进行规则转换的ADC通道的数目。这个数目的取值范围是1到16
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/* ADC1 regular channels configuration [规则模式通道配置]*/ 

	//ADC1 规则通道配置
  	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_55Cycles5);	  //通道11采样时间 55.5周期
  	

	//使能ADC1 DMA 
	ADC_DMACmd(ADC1, ENABLE);
	//使能ADC1
	ADC_Cmd(ADC1, ENABLE);	
	
	// 初始化ADC1校准寄存器
	ADC_ResetCalibration(ADC1);
	//检测ADC1校准寄存器初始化是否完成
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	//开始校准ADC1
	ADC_StartCalibration(ADC1);
	//检测是否完成校准
	while(ADC_GetCalibrationStatus(ADC1));
	
	//ADC1转换启动
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	 
}

