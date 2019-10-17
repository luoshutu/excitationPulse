#ifndef _mySystem_
#define _mySystem_	4004	

#define KeyStart GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5)==0
#define PageTurning  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6)==0
#define PowerEnable GPIO_SetBits(GPIOE, GPIO_Pin_0)
#define PowerNEnable  GPIO_ResetBits(GPIOE, GPIO_Pin_0)

#define ChannelOneReset GPIO_ResetBits(GPIOC, GPIO_Pin_7)
#define	ChannelTwoReset GPIO_ResetBits(GPIOC, GPIO_Pin_6)			
#define	ChannelThreeReset GPIO_ResetBits(GPIOC, GPIO_Pin_5)        
#define	ChannelFourReset GPIO_ResetBits(GPIOC, GPIO_Pin_4)     
#define	ChannelFiveReset GPIO_ResetBits(GPIOC, GPIO_Pin_0)        
#define	ChannelSixReset GPIO_ResetBits(GPIOC, GPIO_Pin_1)			
#define	ChannelSevenReset GPIO_ResetBits(GPIOC, GPIO_Pin_2)       
#define	ChannelEightReset GPIO_ResetBits(GPIOC, GPIO_Pin_3) 

#define ChannelOneSet GPIO_SetBits(GPIOC, GPIO_Pin_7)
#define	ChannelTwoSet GPIO_SetBits(GPIOC, GPIO_Pin_6)			
#define	ChannelThreeSet GPIO_SetBits(GPIOC, GPIO_Pin_5)        
#define	ChannelFourSet GPIO_SetBits(GPIOC, GPIO_Pin_4)     
#define	ChannelFiveSet GPIO_SetBits(GPIOC, GPIO_Pin_0)        
#define	ChannelSixSet GPIO_SetBits(GPIOC, GPIO_Pin_1)			
#define	ChannelSevenSet GPIO_SetBits(GPIOC, GPIO_Pin_2)       
#define	ChannelEightSet GPIO_SetBits(GPIOC, GPIO_Pin_3) 

#define PLLReset GPIO_ResetBits(GPIOB, GPIO_Pin_9)
#define PLLSet   GPIO_SetBits(GPIOB, GPIO_Pin_9)

void SysInit(void);
void TouchHandle_PageOne(unsigned short X,unsigned short Y);
void TouchHandle_PageTwo(unsigned short X,unsigned short Y);
void ShowInterfaceOne(void);
void ShowInterfaceTwo(void);
void Clear(void);
void WorkMark(void);
void ReadyMark(void);
unsigned char set_flag();
#endif
