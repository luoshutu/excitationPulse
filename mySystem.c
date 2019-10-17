#include "stm32f10x.h"
#include "demo.h"
#include "mySystem.h"
#include "MCP.h"

extern void TFT_ShowString(u16 x,u16 y,const u8 *Channel,u16 Color);
extern void FillColor(u16 x,u16 y, u16 x1, u16 y1, u16 z);
extern void OutPutFile(char ImageName);

int Val_Level = 0; //��ѹ��ֵ�ȼ�,
int Num_Pulse[8] = {0};	 //�������
int Frequency[8] = {0};	 //����Ƶ��
int Channel = 1;         //ͨ������
int Function = 1;       //����ѡ��
/****************************************************************************
* ��    �ƣ�void SysInit()
* ��    �ܣ���ʼ��ϵͳ����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/ 
void SysInit()
{
	ChannelOneReset;        //ͨ��1�Ŀ����ź�������ţ���λ
	ChannelTwoReset;				//ͨ��2�Ŀ����ź�������ţ���λ
	ChannelThreeReset;      //ͨ��3�Ŀ����ź�������ţ���λ
	ChannelFourReset;       //ͨ��4�Ŀ����ź�������ţ���λ
	ChannelFiveReset;       //ͨ��5�Ŀ����ź�������ţ���λ
	ChannelSixReset;			 	//ͨ��6�Ŀ����ź�������ţ���λ
	ChannelSevenReset;      //ͨ��7�Ŀ����ź�������ţ���λ
	ChannelEightReset;      //ͨ��8�Ŀ����ź�������ţ���λ
	
	Val_Level = 0;
	MCP_ResistanceSet(0x70);                  //��ʼ����ѹΪ8V
	PowerNEnable;	      //��Դʹ��
	GPIO_SetBits(GPIOE, GPIO_Pin_5);          //start����
	GPIO_SetBits(GPIOE, GPIO_Pin_6);          //stop����
	
	Num_Pulse[0] = 0;Num_Pulse[1] = 0;Num_Pulse[2] = 0;Num_Pulse[3] = 0;  //��ʼ�����������Ƶ��
	Num_Pulse[4] = 0;Num_Pulse[5] = 0;Num_Pulse[6] = 0;Num_Pulse[7] = 0;
	Frequency[0] = 1;Frequency[1] = 1;Frequency[2] = 1;Frequency[3] = 1;
	Frequency[4] = 1;Frequency[5] = 1;Frequency[6] = 1;Frequency[7] = 1;
}
/****************************************************************************
* ��    �ƣ�void TouchHandle_PageOne(unsigned short X,unsigned short Y)
* ��    �ܣ���ȡ��������ֵ���жϴ�������
* ��ڲ����������������ֵ
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/ 
void TouchHandle_PageOne(unsigned short X,unsigned short Y)
{	
	int i;
	int tempN,tempF;
	
	//����ͨ����Ϊ8��
	if(!(0 < Channel < 9)) Channel = 1;						 
				
	//�жϴ���������
	if(X > 70 && X < 130) //ѡ��ͨ��
	{
		if(Y > 43 && Y < 83)          //ѡ��ͨ��1
		{
			Channel = 1;
			Clear();
			ReadyMark();
			FillColor(52,56,65,70,BLUE); 
		}
		else if(Y > 97 && Y < 137)   //ѡ��ͨ��2
		{
			Channel = 2;
			Clear();
			ReadyMark();
			FillColor(52,110,65,124,BLUE);
		}
		else if(Y > 151 && Y < 191)   //ѡ��ͨ��3
		{
			Channel = 3;
			Clear();
			ReadyMark();
			FillColor(52,164,65,178,BLUE);
		}
		else if(Y > 204 && Y < 245)  //ѡ��ͨ��4
		{
			Channel = 4;
			Clear();
			ReadyMark();
			FillColor(52,218,65,232,BLUE);
		}
		/*
		else if(Y > 140 && Y < 160)  //ѡ��ͨ��5
		{
			Channel = 5;
			Clear();
			ReadyMark();
			FillColor(53,145,65,157,BLUE);
		}
			else if(Y > 171 && Y < 191)  //ѡ��ͨ��6
		{
			Channel = 6;
			Clear();
			ReadyMark();
			FillColor(53,176,65,188,BLUE);
		}
		else if(Y > 203 && Y < 223)   //ѡ��ͨ��7
		{
			Channel = 7;
			Clear();
			ReadyMark();
			FillColor(53,208,65,220,BLUE);
		}
		else if(Y > 234 && Y < 254)    //ѡ��ͨ��8
		{
			Channel = 8;
			Clear();
			ReadyMark();
			FillColor(53,239,65,251,BLUE);
		}
		*/
	}

	else if(Y > 131 && Y < 184)  //������
	{
		if(X > 169 && X < 229)  //��ѹ
		{
			Val_Level ++;
		}
		else if(X > 274 && X < 334)  //������
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
		else if(X > 379 && X < 440)   //Ƶ��
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
	else if(Y > 201 && Y < 255) //������
	{
		if(X > 169 && X < 229)  //��ѹ
		{
			Val_Level --;
		}
		else if(X > 274 && X < 334)  //������
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
		else if(X > 379 && X < 440) //Ƶ��
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
	
	//���Ƶ�ѹ�ȼ�//��ѹͻ����ܻᵼ������
	if(Val_Level > 9)
		Val_Level = 9;
	else if(Val_Level < 0)
		Val_Level = 0;
	
	
	for(i = 0;i < 8;i++)
	{
		//�����������
		if(Num_Pulse[i] > 9)
			Num_Pulse[i] = 9;
		else if(Num_Pulse[i] < 0)
			Num_Pulse[i] = 0;
		//����Ƶ��
		if(Frequency[i] > 9)
			Frequency[i] = 9;
		else if(Frequency[i] < 1)
			Frequency[i] = 1;
	}
	
	
	//�жϵ�ǰӦ��ʾ��ֵ
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
	//��ʾ����ֵ
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
* ��    �ƣ�void TouchHandle_PageTwo(unsigned short X,unsigned short Y)
* ��    �ܣ���ȡ��������ֵ���жϴ�������
* ��ڲ����������������ֵ
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/ 
void TouchHandle_PageTwo(unsigned short X,unsigned short Y)
{
	if(X > 345 && X < 435) //ѡ�񷽰�
	{
		if(Y > 74 && Y < 137)          //ѡ�񷽰�A
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
		else if(Y > 158 && Y < 221)   //ѡ�񷽰�B
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
	else if(X > 214 && X < 297)  //��ѹ
	{
		if(Y > 128 && Y < 180)  //������
		{
			Val_Level ++;
		}
		else if(Y > 198 && Y < 250) //������
		{
			Val_Level --;
		}
	}

	//���Ƶ�ѹ�ȼ�//��ѹͻ����ܻᵼ������
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
* ��    �ƣ�void ShowInterfaceOne(void)
* ��    �ܣ���ʾ��������ͼ��
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
void ShowInterfaceOne(void)
{		
	/*//ͨ����־
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
	
	//������ֵ
	FillColor(220,62,238,83,WHITE);
	FillColor(322,62,340,83,WHITE);
	FillColor(425,62,443,83,WHITE);
	TFT_ShowString(225,65,"1",BLACK);
	TFT_ShowString(327,65,"0",BLACK);
	TFT_ShowString(430,65,"1",BLACK);
}
/****************************************************************************
* ��    �ƣ�void ShowInterfaceTwo(void)
* ��    �ܣ���ʾ��������ͼ��
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
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
* ��    �ƣ�void Clear(void)
* ��    �ܣ�
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
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
* ��    �ƣ�void WorkMark(void)
* ��    �ܣ�
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
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
* ��    �ƣ�void ReadyMark(void)
* ��    �ܣ�
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
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
* ��    �ƣ�unsigned char set_flag(void)
* ��    �ܣ�
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
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

