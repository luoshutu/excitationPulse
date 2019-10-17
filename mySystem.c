#include "stm32f10x.h"
#include "demo.h"
#include "mySystem.h"
#include "MCP.h"

extern void TFT_ShowString(u16 x,u16 y,const u8 *Channel,u16 Color);
extern void FillColor(u16 x,u16 y, u16 x1, u16 y1, u16 z);
extern void OutPutFile(char ImageName);

int Val_Level = 0; //电压幅值等级,
int Num_Pulse[8] = {0};	 //脉冲个数
int Frequency[8] = {0};	 //波形频率
int Channel = 1;         //通道号码
int Function = 1;       //功能选择
/****************************************************************************
* 名    称：void SysInit()
* 功    能：初始化系统引脚
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：
****************************************************************************/ 
void SysInit()
{
	ChannelOneReset;        //通道1的控制信号输出引脚，复位
	ChannelTwoReset;				//通道2的控制信号输出引脚，复位
	ChannelThreeReset;      //通道3的控制信号输出引脚，复位
	ChannelFourReset;       //通道4的控制信号输出引脚，复位
	ChannelFiveReset;       //通道5的控制信号输出引脚，复位
	ChannelSixReset;			 	//通道6的控制信号输出引脚，复位
	ChannelSevenReset;      //通道7的控制信号输出引脚，复位
	ChannelEightReset;      //通道8的控制信号输出引脚，复位
	
	Val_Level = 0;
	MCP_ResistanceSet(0x70);                  //初始化电压为8V
	PowerNEnable;	      //电源使能
	GPIO_SetBits(GPIOE, GPIO_Pin_5);          //start按键
	GPIO_SetBits(GPIOE, GPIO_Pin_6);          //stop按键
	
	Num_Pulse[0] = 0;Num_Pulse[1] = 0;Num_Pulse[2] = 0;Num_Pulse[3] = 0;  //初始化脉冲个数与频率
	Num_Pulse[4] = 0;Num_Pulse[5] = 0;Num_Pulse[6] = 0;Num_Pulse[7] = 0;
	Frequency[0] = 1;Frequency[1] = 1;Frequency[2] = 1;Frequency[3] = 1;
	Frequency[4] = 1;Frequency[5] = 1;Frequency[6] = 1;Frequency[7] = 1;
}
/****************************************************************************
* 名    称：void TouchHandle_PageOne(unsigned short X,unsigned short Y)
* 功    能：读取触摸坐标值，判断触摸区域
* 入口参数：触摸点的坐标值
* 出口参数：无
* 说    明：
* 调用方法：
****************************************************************************/ 
void TouchHandle_PageOne(unsigned short X,unsigned short Y)
{	
	int i;
	int tempN,tempF;
	
	//限制通道数为8个
	if(!(0 < Channel < 9)) Channel = 1;						 
				
	//判断触摸点区域
	if(X > 70 && X < 130) //选择通道
	{
		if(Y > 43 && Y < 83)          //选择通道1
		{
			Channel = 1;
			Clear();
			ReadyMark();
			FillColor(52,56,65,70,BLUE); 
		}
		else if(Y > 97 && Y < 137)   //选择通道2
		{
			Channel = 2;
			Clear();
			ReadyMark();
			FillColor(52,110,65,124,BLUE);
		}
		else if(Y > 151 && Y < 191)   //选择通道3
		{
			Channel = 3;
			Clear();
			ReadyMark();
			FillColor(52,164,65,178,BLUE);
		}
		else if(Y > 204 && Y < 245)  //选择通道4
		{
			Channel = 4;
			Clear();
			ReadyMark();
			FillColor(52,218,65,232,BLUE);
		}
		/*
		else if(Y > 140 && Y < 160)  //选择通道5
		{
			Channel = 5;
			Clear();
			ReadyMark();
			FillColor(53,145,65,157,BLUE);
		}
			else if(Y > 171 && Y < 191)  //选择通道6
		{
			Channel = 6;
			Clear();
			ReadyMark();
			FillColor(53,176,65,188,BLUE);
		}
		else if(Y > 203 && Y < 223)   //选择通道7
		{
			Channel = 7;
			Clear();
			ReadyMark();
			FillColor(53,208,65,220,BLUE);
		}
		else if(Y > 234 && Y < 254)    //选择通道8
		{
			Channel = 8;
			Clear();
			ReadyMark();
			FillColor(53,239,65,251,BLUE);
		}
		*/
	}

	else if(Y > 131 && Y < 184)  //变量加
	{
		if(X > 169 && X < 229)  //电压
		{
			Val_Level ++;
		}
		else if(X > 274 && X < 334)  //脉冲数
		{
			if(Channel == 1)
				Num_Pulse[0] ++;
			else if(Channel == 2)
				Num_Pulse[1] ++;
			else if(Channel == 3)
				Num_Pulse[2] ++;
			else if(Channel == 4)
				Num_Pulse[3] ++;
			/*
			else if(Channel == 5)
				Num_Pulse[4] ++;
			else if(Channel == 6)
				Num_Pulse[5] ++;
			else if(Channel == 7)
				Num_Pulse[6] ++;
			else if(Channel == 8)
				Num_Pulse[7] ++;
			*/
		}
		else if(X > 379 && X < 440)   //频率
		{
			if(Channel == 1)
				Frequency[0] ++;
			else if(Channel == 2)
				Frequency[1] ++;
			else if(Channel == 3)
				Frequency[2] ++;
			else if(Channel == 4)
				Frequency[3] ++;
			/*
			else if(Channel == 5)
				Frequency[4] ++;
			else if(Channel == 6)
				Frequency[5] ++;
			else if(Channel == 7)
				Frequency[6] ++;
			else if(Channel == 8)
				Frequency[7] ++;
			*/
		}
	}
	else if(Y > 201 && Y < 255) //变量减
	{
		if(X > 169 && X < 229)  //电压
		{
			Val_Level --;
		}
		else if(X > 274 && X < 334)  //脉冲数
		{
			if(Channel == 1)
				Num_Pulse[0] --;
			else if(Channel == 2)
				Num_Pulse[1] --;
			else if(Channel == 3)
				Num_Pulse[2] --;
			else if(Channel == 4)
				Num_Pulse[3] --;
			/*
			else if(Channel == 5)
				Num_Pulse[4] --;
			else if(Channel == 6)
				Num_Pulse[5] --;
			else if(Channel == 7)
				Num_Pulse[6] --;
			else if(Channel == 8)
				Num_Pulse[7] --;
			*/
		}
		else if(X > 379 && X < 440) //频率
		{
			if(Channel == 1)
				Frequency[0] --;
			else if(Channel == 2)
				Frequency[1] --;
			else if(Channel == 3)
				Frequency[2] --;
			else if(Channel == 4)
				Frequency[3] --;
			/*
			else if(Channel == 5)
				Frequency[4] --;
			else if(Channel == 6)
				Frequency[5] --;
			else if(Channel == 7)
				Frequency[6] --;
			else if(Channel == 8)
				Frequency[7] --;
			*/
		}
	}
	
	//限制电压等级//电压突变可能会导致问题
	if(Val_Level > 9)
		Val_Level = 9;
	else if(Val_Level < 0)
		Val_Level = 0;
	
	
	for(i = 0;i < 8;i++)
	{
		//限制脉冲个数
		if(Num_Pulse[i] > 9)
			Num_Pulse[i] = 9;
		else if(Num_Pulse[i] < 0)
			Num_Pulse[i] = 0;
		//限制频率
		if(Frequency[i] > 9)
			Frequency[i] = 9;
		else if(Frequency[i] < 1)
			Frequency[i] = 1;
	}
	
	
	//判断当前应显示的值
	if(Channel == 1)
	{
		tempN = Num_Pulse[0];
		tempF = Frequency[0];
	}
	else if(Channel == 2)
	{
		tempN = Num_Pulse[1];
		tempF = Frequency[1];
	}
	else if(Channel == 3)
	{
    tempN = Num_Pulse[2];
		tempF = Frequency[2];
	}
	else if(Channel == 4)
	{
    tempN = Num_Pulse[3];
		tempF = Frequency[3];
	}
	/*
	else if(Channel == 5)
	{
    tempN = Num_Pulse[4];
		tempF = Frequency[4];
	}
	else if(Channel == 6)
	{
		tempN = Num_Pulse[5];
		tempF = Frequency[5];
	}
	else if(Channel == 7)
	{
		tempN = Num_Pulse[6];
		tempF = Frequency[6];
	}
	else if(Channel == 8)
	{
		tempN = Num_Pulse[7];
		tempF = Frequency[7];
	}
	*/
	//显示变量值
	switch(Val_Level)
	{
		case 0: TFT_ShowString(225,65,"1",BLACK);MCP_ResistanceSet(0x70);break;
		case 1: TFT_ShowString(225,65,"2",BLACK);MCP_ResistanceSet(0x4C);break;
		case 2: TFT_ShowString(225,65,"3",BLACK);MCP_ResistanceSet(0x36);break;
		case 3: TFT_ShowString(225,65,"4",BLACK);MCP_ResistanceSet(0x27);break;
		case 4: TFT_ShowString(225,65,"5",BLACK);MCP_ResistanceSet(0x1C);break;
		case 5: TFT_ShowString(225,65,"6",BLACK);MCP_ResistanceSet(0x13);break;
		case 6: TFT_ShowString(225,65,"7",BLACK);MCP_ResistanceSet(0x0D);break;
		case 7: TFT_ShowString(225,65,"8",BLACK);MCP_ResistanceSet(0x07);break;
		case 8: TFT_ShowString(225,65,"9",BLACK);MCP_ResistanceSet(0x03);break;
		case 9: TFT_ShowString(225,65,"X",BLACK);MCP_ResistanceSet(0x01);break;
		default: TFT_ShowString(225,65,"1",BLACK);MCP_ResistanceSet(0x70);break;
	}
	
	switch(tempN)
	{
		case 0: TFT_ShowString(327,65,"0",BLACK);break;
		case 1: TFT_ShowString(327,65,"1",BLACK);break;
		case 2: TFT_ShowString(327,65,"2",BLACK);break;
		case 3: TFT_ShowString(327,65,"3",BLACK);break;
		case 4: TFT_ShowString(327,65,"4",BLACK);break;
		case 5: TFT_ShowString(327,65,"5",BLACK);break;
		case 6: TFT_ShowString(327,65,"6",BLACK);break;
		case 7: TFT_ShowString(327,65,"7",BLACK);break;
		case 8: TFT_ShowString(327,65,"8",BLACK);break;
		case 9: TFT_ShowString(327,65,"9",BLACK);break;
		default: TFT_ShowString(327,65,"0",BLACK);break;
	}

	switch(tempF)
	{
		case 1: TFT_ShowString(430,65,"1",BLACK);break;
		case 2: TFT_ShowString(430,65,"2",BLACK);break;
		case 3: TFT_ShowString(430,65,"3",BLACK);break;
		case 4: TFT_ShowString(430,65,"4",BLACK);break;
		case 5: TFT_ShowString(430,65,"5",BLACK);break;
		case 6: TFT_ShowString(430,65,"6",BLACK);break;
		case 7: TFT_ShowString(430,65,"7",BLACK);break;
		case 8: TFT_ShowString(430,65,"8",BLACK);break;
		case 9: TFT_ShowString(430,65,"9",BLACK);break;
		default: TFT_ShowString(430,65,"1",BLACK);break;
	}
}
/****************************************************************************
* 名    称：void TouchHandle_PageTwo(unsigned short X,unsigned short Y)
* 功    能：读取触摸坐标值，判断触摸区域
* 入口参数：触摸点的坐标值
* 出口参数：无
* 说    明：
* 调用方法：
****************************************************************************/ 
void TouchHandle_PageTwo(unsigned short X,unsigned short Y)
{
	if(X > 345 && X < 435) //选择方案
	{
		if(Y > 74 && Y < 137)          //选择方案A
		{
			Clear();
			Val_Level = 0;
			Num_Pulse[0] = 1; Num_Pulse[1] = 1;
			Num_Pulse[2] = 1; Num_Pulse[3] = 1;
			Frequency[0] = 2; Frequency[1] = 2;
			Frequency[2] = 2; Frequency[3] = 2;
			FillColor(358,83,372,97,BLUE);
			FillColor(358,167,372,181,WHITE);
			ReadyMark(); 
		}
		else if(Y > 158 && Y < 221)   //选择方案B
		{
			Clear();
			Val_Level = 0;
			Num_Pulse[0] = 5; Num_Pulse[1] = 5;
			Num_Pulse[2] = 5; Num_Pulse[3] = 5;
			Frequency[0] = 5; Frequency[1] = 5;
			Frequency[2] = 5; Frequency[3] = 5;	
			FillColor(358,83,372,97,WHITE);
			FillColor(358,167,372,181,BLUE);			
			ReadyMark();
		}
	}
	else if(X > 214 && X < 297)  //电压
	{
		if(Y > 128 && Y < 180)  //变量加
		{
			Val_Level ++;
		}
		else if(Y > 198 && Y < 250) //变量减
		{
			Val_Level --;
		}
	}

	//限制电压等级//电压突变可能会导致问题
	if(Val_Level > 9)
		Val_Level = 9;
	else if(Val_Level < 0)
		Val_Level = 0;
	
	switch(Val_Level)
	{
		case 0: TFT_ShowString(280,61,"1",BLACK);MCP_ResistanceSet(0x70);break;
		case 1: TFT_ShowString(280,61,"2",BLACK);MCP_ResistanceSet(0x4C);break;
		case 2: TFT_ShowString(280,61,"3",BLACK);MCP_ResistanceSet(0x36);break;
		case 3: TFT_ShowString(280,61,"4",BLACK);MCP_ResistanceSet(0x27);break;
		case 4: TFT_ShowString(280,61,"5",BLACK);MCP_ResistanceSet(0x1C);break;
		case 5: TFT_ShowString(280,61,"6",BLACK);MCP_ResistanceSet(0x13);break;
		case 6: TFT_ShowString(280,61,"7",BLACK);MCP_ResistanceSet(0x0D);break;
		case 7: TFT_ShowString(280,61,"8",BLACK);MCP_ResistanceSet(0x07);break;
		case 8: TFT_ShowString(280,61,"9",BLACK);MCP_ResistanceSet(0x03);break;
		case 9: TFT_ShowString(280,61,"X",BLACK);MCP_ResistanceSet(0x01);break;
		default: TFT_ShowString(280,61,"1",BLACK);MCP_ResistanceSet(0x70);break;
	}
}
/****************************************************************************
* 名    称：void ShowInterfaceOne(void)
* 功    能：显示操作界面图形
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
void ShowInterfaceOne(void)
{		
	/*//通道标志
	FillColor(53,19,65,31,WHITE); 
	FillColor(53,50,65,62,WHITE);
	FillColor(53,82,65,94,WHITE);
	FillColor(53,113,65,125,WHITE);
	FillColor(53,145,65,157,WHITE);
	FillColor(53,176,65,188,WHITE);
	FillColor(53,208,65,220,WHITE);
	FillColor(53,239,65,251,WHITE);
	*/

	TouchHandle_PageOne(100,60);
	
	//变量数值
	FillColor(220,62,238,83,WHITE);
	FillColor(322,62,340,83,WHITE);
	FillColor(425,62,443,83,WHITE);
	TFT_ShowString(225,65,"1",BLACK);
	TFT_ShowString(327,65,"0",BLACK);
	TFT_ShowString(430,65,"1",BLACK);
}
/****************************************************************************
* 名    称：void ShowInterfaceTwo(void)
* 功    能：显示操作界面图形
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
void ShowInterfaceTwo(void)
{		
	FillColor(358,83,372,97,BLUE);
	FillColor(358,167,372,181,WHITE);
	FillColor(275,58,293,79,WHITE);
	TFT_ShowString(280,61,"1",BLACK);
	TouchHandle_PageTwo(400,100);
	//ReadyMark();
}
/****************************************************************************
* 名    称：void Clear(void)
* 功    能：
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void Clear()
{
	FillColor(52,56,65,70,WHITE); 
	FillColor(52,110,65,124,WHITE);
	FillColor(52,164,65,178,WHITE);
	FillColor(52,218,65,232,WHITE);
//	Num_Pulse[0] = 0; Num_Pulse[1] = 0;
//	Num_Pulse[2] = 0; Num_Pulse[3] = 0;
//	FillColor(53,145,65,157,WHITE);
//	FillColor(53,176,65,188,WHITE);
//	FillColor(53,208,65,220,WHITE);
//	FillColor(53,239,65,251,WHITE);
}
/****************************************************************************
* 名    称：void WorkMark(void)
* 功    能：
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void WorkMark()
{
	if(Num_Pulse[0] != 0)
	{	
		FillColor(52,56,65,70,GREEN); 
	}
	if(Num_Pulse[1] != 0)
	{	
		FillColor(52,110,65,124,GREEN);
	}
	if(Num_Pulse[2] != 0)
	{	
		FillColor(52,164,65,178,GREEN);
	}
	if(Num_Pulse[3] != 0)
	{	
		FillColor(52,218,65,232,GREEN);
	}
	/*
	if(Num_Pulse[4] != 0)
	{	
		FillColor(53,145,65,157,GREEN); 
	}
	if(Num_Pulse[5] != 0)
	{	
		FillColor(53,176,65,188,GREEN); 
	}
	if(Num_Pulse[6] != 0)
	{	
		FillColor(53,208,65,220,GREEN);
	}
	if(Num_Pulse[7] != 0)
	{	
		FillColor(53,239,65,251,GREEN);
	}
	*/
}
/****************************************************************************
* 名    称：void ReadyMark(void)
* 功    能：
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void ReadyMark()
{
	if(Num_Pulse[0] != 0)
	{	
		FillColor(52,56,65,70,GRED); 
	}
	if(Num_Pulse[1] != 0)
	{	
		FillColor(52,110,65,124,GRED);
	}
	if(Num_Pulse[2] != 0)
	{	
		FillColor(52,164,65,178,GRED);
	}
	if(Num_Pulse[3] != 0)
	{	
		FillColor(52,218,65,232,GRED);
	}
	/*
	if(Num_Pulse[4] != 0)
	{	
		FillColor(53,145,65,157,GRED); 
	}
	if(Num_Pulse[5] != 0)
	{	
		FillColor(53,176,65,188,GRED); 
	}
	if(Num_Pulse[6] != 0)
	{	
		FillColor(53,208,65,220,GRED);
	}
	if(Num_Pulse[7] != 0)
	{	
		FillColor(53,239,65,251,GRED);
	}
	*/
}
/****************************************************************************
* 名    称：unsigned char set_flag(void)
* 功    能：
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
unsigned char set_flag()
{
	unsigned char setFlag;
	if(Num_Pulse[0] | Num_Pulse[1] | Num_Pulse[2] | Num_Pulse[3])
		setFlag = 1;
	else
		setFlag = 0;
	return setFlag;
}

