#include  "stm32f10x.h"
#include  "stm32f10x_tim.h"
#include  "mySystem.h"

unsigned int capture2=0;
unsigned int capture3=0;
unsigned int capture4=0;
unsigned int CCR2_Val = 11250;
extern void TFT_ShowChar(u16 x,u16 y,u8 num,u16 COLOR);
extern void TFT_ShowNum(u16 x,u16 y,u32 num,u16 COLOR);
extern void TFT_ShowString(u16 x,u16 y,const u8 *p,u16 COLOR);
extern void FillColor(u16 x,u16 y, u16 x1, u16 y1, u16 z);
extern u16 POINT_COLOR;                //默认笔画颜色
extern TIM_OCInitTypeDef  TIM3_OCInitStructure;
extern TIM_OCInitTypeDef  TIM2_OCInitStructure;
extern int Num_Pulse[];
extern int Frequency[];
extern int TimCount;
extern vu16 ADC_ConvertedValue;
unsigned char buf[16];
float tempFloat,tempInt;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : NMIException
* Description    : This function handles NMI exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NMIException(void)
{
 
}

/*******************************************************************************
* Function Name  : HardFaultException
* Description    : This function handles Hard Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HardFaultException(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : MemManageException
* Description    : This function handles Memory Manage exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MemManageException(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : BusFaultException
* Description    : This function handles Bus Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BusFaultException(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : UsageFaultException
* Description    : This function handles Usage Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UsageFaultException(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : DebugMonitor
* Description    : This function handles Debug Monitor exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DebugMonitor(void)
{
}

/*******************************************************************************
* Function Name  : SVCHandler
* Description    : This function handles SVCall exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SVCHandler(void)
{
}

/*******************************************************************************
* Function Name  : PendSVC
* Description    : This function handles PendSVC exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PendSVC(void)
{
}

/*******************************************************************************
* Function Name  : SysTickHandler
* Description    : This function handles SysTick Handler.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SysTickHandler(void)
{
   
     
}


/*******************************************************************************
* Function Name  : WWDG_IRQHandler
* Description    : This function handles WWDG interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void WWDG_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : PVD_IRQHandler
* Description    : This function handles PVD interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PVD_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TAMPER_IRQHandler
* Description    : This function handles Tamper interrupt request. 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TAMPER_IRQHandler(void)
{
}



/*******************************************************************************
* Function Name  : FLASH_IRQHandler
* Description    : This function handles Flash interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void FLASH_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : RCC_IRQHandler
* Description    : This function handles RCC interrupt request. 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI0_IRQHandler
* Description    : This function handles External interrupt Line 0 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI0_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI1_IRQHandler
* Description    : This function handles External interrupt Line 1 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI2_IRQHandler
* Description    : This function handles External interrupt Line 2 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

////////////////键盘行线2中断
void EXTI2_IRQHandler(void)
{
 
}
/*******************************************************************************
* Function Name  : EXTI3_IRQHandler
* Description    : This function handles External interrupt Line 3 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI3_IRQHandler(void)
{  
}

/*******************************************************************************
* Function Name  : EXTI4_IRQHandler
* Description    : This function handles External interrupt Line 4 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI4_IRQHandler(void)
{  
}

/*******************************************************************************
* Function Name  : DMA1_Channel1_IRQHandler
* Description    : This function handles DMA1 Channel 1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMAChannel1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel2_IRQHandler
* Description    : This function handles DMA1 Channel 2 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMAChannel2_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel3_IRQHandler
* Description    : This function handles DMA1 Channel 3 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMAChannel3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel4_IRQHandler
* Description    : This function handles DMA1 Channel 4 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMAChannel4_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel5_IRQHandler
* Description    : This function handles DMA1 Channel 5 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMAChannel5_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel6_IRQHandler
* Description    : This function handles DMA1 Channel 6 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMAChannel6_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel7_IRQHandler
* Description    : This function handles DMA1 Channel 7 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMAChannel7_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : ADC1_2_IRQHandler
* Description    : This function handles ADC1 and ADC2 global interrupts requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : USB_HP_CAN_TX_IRQHandler
* Description    : This function handles USB High Priority or CAN TX interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_HP_CAN_TX_IRQHandler(void)
{
 
}

/*******************************************************************************
* Function Name  : USB_LP_CAN_RX0_IRQHandler
* Description    : This function handles USB Low Priority or CAN RX0 interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_LP_CAN_RX0_IRQHandler(void)
{
 
}

/*******************************************************************************
* Function Name  : CAN_RX1_IRQHandler
* Description    : This function handles CAN RX1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAN_RX1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : CAN_SCE_IRQHandler
* Description    : This function handles CAN SCE interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAN_SCE_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI9_5_IRQHandler
* Description    : This function handles External lines 9 to 5 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI9_5_IRQHandler(void)
{ 
}

/*******************************************************************************
* Function Name  : TIM1_BRK_IRQHandler
* Description    : This function handles TIM1 Break interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_BRK_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM1_UP_IRQHandler
* Description    : This function handles TIM1 overflow and update interrupt 
*                  request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_UP_IRQHandler(void)
{
  /* Clear the TIM1 Update pending bit */
 

}

/*******************************************************************************
* Function Name  : TIM1_TRG_COM_IRQHandler
* Description    : This function handles TIM1 Trigger and commutation interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_TRG_COM_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM1_CC_IRQHandler
* Description    : This function handles TIM1 capture compare interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_CC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : This function handles TIM2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM2_IRQHandler(void)
{  
	static int i1 = 0;    //进入中断次数
	static int j1 = 1;    //电平反转次数
	static int i2 = 0;    //进入中断次数
	static int j2 = 1;    //电平反转次数
	static int i3 = 0;    //进入中断次数
	static int j3 = 1;    //电平反转次数
	static int i4 = 0;    //进入中断次数
	static int j4 = 1;    //电平反转次数
	
	if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)	    //判断状态寄存器 TIM3_SR[2] 是否发生了捕获/比较2 中断
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);			//软件清除状态寄存器 TIM3_SR[2] 捕获/比较2 中断中断标志

		capture2 = TIM_GetCapture2(TIM2);					//获取TIM3_CCR2的预装值
		TIM_SetCompare2(TIM2, capture2+ CCR2_Val);			//和CCR2_Val累加后装入TIM3_CCR2	 这是为了配合计数器值的递增。以在下一次作出比较。
	}
	
	TimCount++;             //倒计时计时
	
	i1++;
	i2++;
	i3++;
	i4++;

	if(i1<(int)(100/Frequency[0]))  //
	{
		j1++;
		if(j1<(2*Num_Pulse[0] + 2))   //当时间超过脉冲个数所占的时间时不再进行电平翻转
		{
			if(j1%2 == 0)
			{
				ChannelOneSet;
			}
			else
			{
				ChannelOneReset;
			}
		}
	}
	else
	{
		i1 = 0;
		j1 = 1;
	}
	
	if(i2<(int)(100/Frequency[1]))
	{
		j2++;
		if(j2<(2*Num_Pulse[1] + 2))
		{
			if(j2%2 == 0)
			{
				ChannelTwoSet;
			}
			else
			{
				ChannelTwoReset;
			}
		}
	}
	else
	{
		i2 = 0;
		j2 = 1;
	}
	
	if(i3<(int)(100/Frequency[2]))
	{
		j3++;
		if(j3<(2*Num_Pulse[2] + 2))
		{
			if(j3%2 == 0)
			{
				ChannelThreeSet;
			}
			else
			{
				ChannelThreeReset;
			}
		}
	}
	else
	{
		i3 = 0;
		j3 = 1;
	}
	
	if(i4<(int)(100/Frequency[3]))
	{
		j4++;
		if(j4<(2*Num_Pulse[3] + 2))
		{
			if(j4%2 == 0)
			{
				ChannelFourSet;
			}
			else
			{
				ChannelFourReset;
			}
		}
	}
	else
	{
		i4 = 0;
		j4 = 1;
	}
}

/*******************************************************************************
* Function Name  : TIM3_IRQHandler
* Description    : This function handles TIM3 global interrupt request.	   产生12.5HZ的频率
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM3_IRQHandler(void)
{ 
	/*
	static int i5 = 0;    //进入中断次数
	static int j5 = 1;    //电平反转次数
	static int i6 = 0;    //进入中断次数
	static int j6 = 1;    //电平反转次数
	static int i7 = 0;    //进入中断次数
	static int j7 = 1;    //电平反转次数
	static int i8 = 0;    //进入中断次数
	static int j8 = 1;    //电平反转次数
	*/
	
	static int TimCount_AD = 0;
	
	if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)	    //判断状态寄存器 TIM3_SR[2] 是否发生了捕获/比较2 中断
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);			//软件清除状态寄存器 TIM3_SR[2] 捕获/比较2 中断中断标志

		capture3 = TIM_GetCapture3(TIM3);					//获取TIM3_CCR2的预装值
		TIM_SetCompare3(TIM3, capture3+ 1125);			//和CCR2_Val累加后装入TIM3_CCR2	 这是为了配合计数器值的递增。以在下一次作出比较。

	}

	TimCount_AD++;
	if(TimCount_AD > 100)
	{
		TimCount_AD = 0;
		POINT_COLOR=0;

		if(ADC_ConvertedValue >= 3010)
		{
			FillColor(441,12,463,20,0xffff);
			FillColor(459,12,463,20,0x001f);
			FillColor(453,12,457,20,0x001f);
			FillColor(447,12,451,20,0x001f);
			FillColor(441,12,445,20,0x001f);
		}
		else if(ADC_ConvertedValue >= 2960 && ADC_ConvertedValue < 3010)
		{
			FillColor(459,12,463,20,0x001f);
			FillColor(453,12,457,20,0x001f);
			FillColor(447,12,451,20,0x001f);		
		}
		else if(ADC_ConvertedValue >= 2910 && ADC_ConvertedValue < 2960)
		{
			FillColor(441,12,463,20,0xffff);
			FillColor(459,12,463,20,0x001f);
			FillColor(453,12,457,20,0x001f);
			FillColor(447,12,451,20,0x001f);
		}
		else if(ADC_ConvertedValue >= 2860 && ADC_ConvertedValue < 2910)
		{
			FillColor(459,12,463,20,0x001f);
			FillColor(453,12,457,20,0x001f);
		}
		else if(ADC_ConvertedValue >= 2810 && ADC_ConvertedValue < 2860)
		{
			FillColor(441,12,463,20,0xffff);
			FillColor(459,12,463,20,0x001f);
			FillColor(453,12,457,20,0x001f);
		}
		else if(ADC_ConvertedValue >= 2760 && ADC_ConvertedValue < 2810)
		{
			FillColor(459,12,463,20,0x001f);
		}	
		else if(ADC_ConvertedValue < 2760)
		{
			FillColor(441,12,463,20,0xffff);
			FillColor(459,12,463,20,0x001f);
		}	
		
		
//		TFT_ShowNum(200,230,ADC_ConvertedValue,0x001f);
//		TFT_ShowChar(208,250,'.',0x001f);
//		tempFloat = (float)ADC_ConvertedValue/412 - 3.30485;
//		
//		tempInt = (int)(tempFloat*100)%100;
//		TFT_ShowNum(216,250,tempInt,0x001f);
//		
//		tempInt = (int)tempFloat;
//		TFT_ShowNum(200,250,tempInt,0x001f);
	}
/*	i5++;
	i6++;
	i7++;
	i8++;
	
	if(i5<(int)(100/Frequency[4]))
	{
		j5++;
		if(j5<(2*Num_Pulse[4] + 2))
		{
			if(j5%2 == 0)
			{
				ChannelFiveSet;
			}
			else
			{
				ChannelFiveReset;
			}
		}
	}
	else
	{
		i5 = 0;
		j5 = 1;
	}
	
	if(i6<(int)(100/Frequency[5]))
	{
		j6++;
		if(j6<(2*Num_Pulse[5] + 2))
		{
			if(j6%2 == 0)
			{
				ChannelSixSet;
			}
			else
			{
				ChannelSixReset;
			}
		}
	}
	else
	{
		i6 = 0;
		j6 = 1;
	}
	
	if(i7<(int)(100/Frequency[6]))
	{
		j7++;
		if(j7<(2*Num_Pulse[6] + 2))
		{
			if(j7%2 == 0)
			{
				ChannelSevenSet;
			}
			else
			{
				ChannelSevenReset;
			}
		}
	}
	else
	{
		i7 = 0;
		j7 = 1;
	}
	
	if(i8<(int)(100/Frequency[7]))
	{
		j8++;
		if(j8<(2*Num_Pulse[7] + 2))
		{
			if(j8%2 == 0)
			{
				ChannelEightSet;
			}
			else
			{
				ChannelEightReset;
			}
		}
	}
	else
	{
		i8 = 0;
		j8 = 1;
	}*/
}

/*******************************************************************************
* Function Name  : TIM4_IRQHandler
* Description    : This function handles TIM4 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM4_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : I2C1_EV_IRQHandler
* Description    : This function handles I2C1 Event interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C1_EV_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : I2C1_ER_IRQHandler
* Description    : This function handles I2C1 Error interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C1_ER_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : I2C2_EV_IRQHandler
* Description    : This function handles I2C2 Event interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C2_EV_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : I2C2_ER_IRQHandler
* Description    : This function handles I2C2 Error interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C2_ER_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : SPI1_IRQHandler
* Description    : This function handles SPI1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : SPI2_IRQHandler
* Description    : This function handles SPI2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI2_IRQHandler(void)
{
 
}

/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : This function handles USART1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART1_IRQHandler(void)
{

   
}

/*******************************************************************************
* Function Name  : USART2_IRQHandler
* Description    : This function handles USART2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART2_IRQHandler(void)
{	
	
}

/*******************************************************************************
* Function Name  : USART3_IRQHandler
* Description    : This function handles USART3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART3_IRQHandler(void)
{
 
}

/*******************************************************************************
* Function Name  : EXTI15_10_IRQHandler
* Description    : This function handles External lines 15 to 10 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI15_10_IRQHandler(void)
{
 
}

/*******************************************************************************
* Function Name  : RTC_IRQHandler
* Description    : This function handles RTC global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_IRQHandler(void)
{

}


/*******************************************************************************
* Function Name  : USBWakeUp_IRQHandler
* Description    : This function handles USB WakeUp interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USBWakeUp_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM8_BRK_IRQHandler
* Description    : This function handles TIM8 Break interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM8_BRK_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM8_UP_IRQHandler
* Description    : This function handles TIM8 overflow and update interrupt 
*                  request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM8_UP_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM8_TRG_COM_IRQHandler
* Description    : This function handles TIM8 Trigger and commutation interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM8_TRG_COM_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM8_CC_IRQHandler
* Description    : This function handles TIM8 capture compare interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM8_CC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : ADC3_IRQHandler
* Description    : This function handles ADC3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADC3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : FSMC_IRQHandler
* Description    : This function handles FSMC global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void FSMC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : SDIO_IRQHandler
* Description    : This function handles SDIO global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SDIO_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM5_IRQHandler
* Description    : This function handles TIM5 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM5_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : SPI3_IRQHandler
* Description    : This function handles SPI3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : UART4_IRQHandler
* Description    : This function handles UART4 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART4_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : UART5_IRQHandler
* Description    : This function handles UART5 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART5_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM6_IRQHandler
* Description    : This function handles TIM6 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM6_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM7_IRQHandler
* Description    : This function handles TIM7 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM7_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA2_Channel1_IRQHandler
* Description    : This function handles DMA2 Channel 1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA2_Channel1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA2_Channel2_IRQHandler
* Description    : This function handles DMA2 Channel 2 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA2_Channel2_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA2_Channel3_IRQHandler
* Description    : This function handles DMA2 Channel 3 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA2_Channel3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA2_Channel4_5_IRQHandler
* Description    : This function handles DMA2 Channel 4 and DMA2 Channel 5
*                  interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA2_Channel4_5_IRQHandler(void)
{
}


/*******************************************************************************
* Function Name  : RTCAlarm_IRQHandler
* Description    : This function handles RTC Alarm interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTCAlarm_IRQHandler(void)
{
 
}
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
