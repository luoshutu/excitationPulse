#include "fsmc_sram.h"
#include "font.h"  
#include "demo.h"  
#define Bank1_LCD_D    ((uint32_t)0x60020000)    //disp Data ADDR
#define Bank1_LCD_C    ((uint32_t)0x60000000)	 //disp Reg ADDR

unsigned int  HDP=479;

unsigned int  HT=531;
unsigned int  HPS=43;
unsigned int  LPS=8;
unsigned char HPW=1;
unsigned int  VDP=271;
unsigned int  VT=288;
unsigned int  VPS=12;
unsigned int  FPS=4;
unsigned char VPW=10;

void LCD_Init_SD(void);
void LCD_WR_REG(unsigned int index);
void LCD_WR_CMD(unsigned int index,unsigned int val);

void LCD_WR_Data(unsigned int val);

void LCD_test(void);
void LCD_clear(unsigned int p);

void Delay(__IO uint32_t nCount);

unsigned short LCD_RD_data(void);
extern void lcd_rst(void);
extern void Delay(__IO uint32_t nCount);
void TFT_CLEAR(u16 x,u16 y,u32 len,u32 wid); 
void TFT_DrawPoint(u16 x,u16 y);
void TFT_ShowChar(u16 x,u16 y,u8 num,u16 COLOR);
void TFT_ShowNum(u16 x,u16 y,u32 num,u16 COLOR);
void TFT_ShowString(u16 x,u16 y,const u8 *p,u16 COLOR);
void FillColor(u16 x,u16 y, u16 x1, u16 y1, u16 z);

u16 POINT_COLOR=RED;     				//默认红色  

/****************************************************************************
* 名    称：LCD_WR_REG(unsigned int index)
* 功    能：FSMC写显示器寄存器地址函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void LCD_WR_REG(unsigned int index)
{
	*(__IO uint16_t *) (Bank1_LCD_C)= index;

}			 

/****************************************************************************
* 名    称：void LCD_WR_CMD(unsigned int index,unsigned int val)
* 功    能：FSMC写显示器寄存器数据函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void LCD_WR_CMD(unsigned int index,unsigned int val)
{	
	*(__IO uint16_t *) (Bank1_LCD_C)= index;	
	*(__IO uint16_t *) (Bank1_LCD_D)= val;
}


/****************************************************************************
* 名    称：unsigned short LCD_RD_data(void)
* 功    能：FSMC读显示区16位数据函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
unsigned short LCD_RD_data(void){
	unsigned int a=0;
	a=*(__IO uint16_t *) (Bank1_LCD_D);   //空操作
	a=*(__IO uint16_t *) (Bank1_LCD_D);   //读出的实际16位像素数据	  
	return(a);	
}



/****************************************************************************
* 名    称：LCD_WR_Data(unsigned int val)
* 功    能：FSMC写16位数据函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void LCD_WR_Data(unsigned int val)
{   
	*(__IO uint16_t *) (Bank1_LCD_D)= val; 	
}

void LCD_WR_Data_8(unsigned int val)
{
	*(__IO uint16_t *) (Bank1_LCD_D)= val;
}
/****************************************************************************
* 名    称：void Delay(__IO uint32_t nCount)
* 功    能：延时函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
/****************************************************************************
* 名    称：void LCD_Init(void)
* 功    能：2.4 寸TFT 初始化函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void LCD_Init_SD(void)
{	
	GPIO_ResetBits(GPIOE, GPIO_Pin_1);
	Delay(0xAFFFF);					   
	GPIO_SetBits(GPIOE, GPIO_Pin_1 );		 	 
	Delay(0xAFFFF);

	LCD_WR_REG(0x00E2);	        //PLL 倍频到120M   PLL=10*36/3=120M
	LCD_WR_Data(0x0023);	    //N=0x36 for 6.5M, 0x23 for 10M crystal
	LCD_WR_Data(0x0002);
	LCD_WR_Data(0x0004);

	LCD_WR_REG(0x00E0);         
	LCD_WR_Data(0x0001);
	Delay(0xAFFF);
	LCD_WR_REG(0x00E0);
	LCD_WR_Data(0x0003);		// PLL 使能  PLL作为时钟
	Delay(0xAFFF);
	LCD_WR_REG(0x0001);         // 软件复位
	Delay(0xAFFF);
	LCD_WR_REG(0x00E6);	        //PLL setting for PCLK, depends on resolution
	LCD_WR_Data(0x0001);
	LCD_WR_Data(0x0033);
	LCD_WR_Data(0x0032);

	LCD_WR_REG(0x00B0);	        //设置LCD模式
	LCD_WR_Data(0x0000);
	LCD_WR_Data(0x0000);
	LCD_WR_Data((HDP>>8)&0X00FF);  //Set HDP
	LCD_WR_Data(HDP&0X00FF);
	LCD_WR_Data((VDP>>8)&0X00FF);  //Set VDP
	LCD_WR_Data(VDP&0X00FF);
	LCD_WR_Data(0x0000);

	LCD_WR_REG(0x00B4);	//HSYNC
	LCD_WR_Data((HT>>8)&0X00FF);  //Set HT
	LCD_WR_Data(HT&0X00FF);
	LCD_WR_Data((HPS>>8)&0X00FF);  //Set HPS
	LCD_WR_Data(HPS&0X00FF);
	LCD_WR_Data(HPW);			   //Set HPW
	LCD_WR_Data((LPS>>8)&0X00FF);  //Set HPS
	LCD_WR_Data(LPS&0X00FF);
	LCD_WR_Data(0x0000);

	LCD_WR_REG(0x00B6);	//VSYNC
	LCD_WR_Data((VT>>8)&0X00FF);   //Set VT
	LCD_WR_Data(VT&0X00FF);
	LCD_WR_Data((VPS>>8)&0X00FF);  //Set VPS
	LCD_WR_Data(VPS&0X00FF);
	LCD_WR_Data(VPW);			   //Set VPW
	LCD_WR_Data((FPS>>8)&0X00FF);  //Set FPS
	LCD_WR_Data(FPS&0X00FF);

	LCD_WR_REG(0x00BA);
	LCD_WR_Data(0x000F);    //GPIO[3:0] out 1

	LCD_WR_REG(0x00B8);
	LCD_WR_Data(0x0007);    //GPIO3=input, GPIO[2:0]=output
	LCD_WR_Data(0x0001);    //GPIO0 normal

	LCD_WR_REG(0x0036);            //rotation
	LCD_WR_Data(0x0000);


	LCD_WR_REG(0x00F0);             //16位  565模式
	LCD_WR_Data(0x0003);


	Delay(0xAFFF);

	TFT_CLEAR(0,0,480,272);
	LCD_WR_REG(0x0029); //display on

	LCD_WR_REG(0x00BE); //set PWM for B/L
	LCD_WR_Data(0x0006);
	LCD_WR_Data(0x0080);

	LCD_WR_Data(0x0001);
	LCD_WR_Data(0x00f0);
	LCD_WR_Data(0x0000);
	LCD_WR_Data(0x0000);

	LCD_WR_REG(0x00d0);//设置动态背光控制配置 
	LCD_WR_Data(0x000d);
}

/****************************************************************************
* 名    称：void TFT_CLEAR(u8 x,u16 y,u8 len,u16 wid)
* 功    能：2.4 寸TFT 清屏函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void TFT_CLEAR(u16 x,u16 y,u32 len,u32 wid)
{                    
  u32 w;

	LCD_WR_REG(0x002A);	
	LCD_WR_Data(x>>8);	    
	LCD_WR_Data(x);
	LCD_WR_Data((len-1)>>8);	    
	LCD_WR_Data((len-1));
  LCD_WR_REG(0x002b);	
	LCD_WR_Data(y>>8);	    
	LCD_WR_Data(y);
	LCD_WR_Data((wid-1)>>8);	    
	LCD_WR_Data((wid-1));
	LCD_WR_REG(0x002c);
	w=(len-x)*(wid-y);
	while(w--) LCD_WR_Data(0xffff);
}   

/****************************************************************************
* 名    称：void TFT_DrawPoint(u8 x,u16 y)
* 功    能：2.4 寸TFT 画点函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void TFT_DrawPoint(u16 x,u16 y)
{								       
	LCD_WR_REG(0x002A);				    
	LCD_WR_Data(x>>8);	    			//起始地址X
	LCD_WR_Data(x);	    			
	LCD_WR_Data(x>>8);	    			//结束地址X
	LCD_WR_Data(x);	    			    
  LCD_WR_REG(0x002b);	
	LCD_WR_Data(y>>8);	    			//起始地址Y
	LCD_WR_Data(y);	    			    
	LCD_WR_Data(y>>8);	    			//结束地址Y
	LCD_WR_Data(y);	    			
	LCD_WR_REG(0x2c);
	LCD_WR_Data(POINT_COLOR); 	   	
} 
  
/****************************************************************************
* 名    称：void TFT_ShowChar(u8 x,u16 y,u8 num)
* 功    能：在指定位置显示一个字符
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void TFT_ShowChar(u16 x,u16 y,u8 num,u16 COLOR)
{       
	#define MAX_CHAR_POSX 464
	#define MAX_CHAR_POSY 262
	u8 temp;
	u8 pos,t;      
	if(x>MAX_CHAR_POSX||y>MAX_CHAR_POSY)return;	

	num=num-' ';						//得到偏移后的值
	for(pos=0;pos<16;pos++)	
	{
	  temp=asc2_1608[num][pos];		//获得字模数组的值		   				
		for(t=0;t<8;t++)
		{                 
			LCD_WR_REG(0x002A);	    
			LCD_WR_Data((x+t)>>8);	    			//起始地址X
			LCD_WR_Data((x+t)&0x00ff);	    			
			LCD_WR_Data((x+t)>>8);	    			//结束地址X
			LCD_WR_Data((x+t)&0x00ff);	    			    
			
			LCD_WR_REG(0x002B);
			LCD_WR_Data((y+pos)>>8);	    			//起始地址Y
			LCD_WR_Data((y+pos)&0x00ff);	    			    
			LCD_WR_Data((y+pos)>>8);	    			//结束地址Y
			LCD_WR_Data((y+pos)&0x00ff);	
			
			LCD_WR_REG(0x2c);
			
			if(temp&0x01)LCD_WR_Data(COLOR);	  //位为1 用指定颜色写入到像素
			else LCD_WR_Data(WHITE);                 //位为0 用背景色写入到像素    
			temp>>=1; 
		}
	}
 	
/*	//不保留背景色
	LCD_WR_REG(0x002A);				    
	LCD_WR_Data(x>>8);	    			//起始地址X
	LCD_WR_Data(x&0x00ff);
	LCD_WR_Data((x+7)>>8);	    		//结束地址X
	LCD_WR_Data((x+7)&0x00ff);
  
	LCD_WR_REG(0x002B);	
	LCD_WR_Data(y>>8);	    			//起始地址Y
	LCD_WR_Data(y&0x00ff);
	LCD_WR_Data((y+15)>>8);	    	 	//结束地址Y
	LCD_WR_Data((y+15)&0x00ff);
	
	LCD_WR_REG(0x2c);
	 
	num=num-' ';						//得到偏移后的值
	for(pos=0;pos<16;pos++)	{
	    temp=asc2_1608[num][pos];		//获得字模数组的值
	    for(t=0;t<8;t++)
	    {                 
	      if(temp&0x01)LCD_WR_Data(POINT_COLOR);	  //位为1 用指定颜色写入到像素
	      else LCD_WR_Data(COLOR);                 //位为0 用白色写入到像素    
	      temp>>=1; 
	    }
	}
*/
}  	 
/****************************************************************************
* 名    称：u32 mn(u8 m,u8 n)
* 功    能：
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
u32 mn(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}
/****************************************************************************
* 名    称：void TFT_ShowNum(u8 x,u16 y,u32 num)
* 功    能：在指定位置显示一个数字0-65536
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void TFT_ShowNum(u16 x,u16 y,u32 num,u16 COLOR)
{      
	u32 res;   	   
	u8 t=0,t1=0;   
	res=num;
	if(!num)TFT_ShowChar(x,y,'0',COLOR);		//加入数字为0 在指定位置显示0
	while(res)  						//得到数字长度
	{
		res/=10;
		t++;
	}
	t1=t;
	while(t)							//显示数字
	{
		res=mn(10,t-1); 	 
	    TFT_ShowChar(x+(t1-t)*8,y,(num/res)%10+'0',COLOR);	 //在指定位置显示数字
		t--;
	}
	if(t1==3) TFT_ShowChar(x+18,y,' ',COLOR);	                 //触摸值是3位的话，最后一个显示区清除				     
} 
/****************************************************************************
* 名    称：void TFT_ShowString(u8 x,u16 y,const u8 *p)
* 功    能：在指定位置显示字符串
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void TFT_ShowString(u16 x,u16 y,const u8 *p,u16 COLOR)
{         
    while(*p!='\0')
    {       
        //if(x>MAX_CHAR_POSX){x=0;y+=12;}
        //if(y>MAX_CHAR_POSY){y=x=0;TFT_CLEAR(0,60,479,799); }
        TFT_ShowChar(x,y,*p,COLOR);
        x+=8;
        p++;
    }  
}
    
/****************************************************************************
* 名    称：void FillColor(u8 x,u16 y, u8 x1, u16 y1, u16 z)
* 功    能：在屏幕上画区域
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：
****************************************************************************/
void FillColor(u16 x,u16 y, u16 x1, u16 y1, u16 z)
{	
	u16 a,b;
	POINT_COLOR=z; 
	for(a=0; a<(y1-y); a++)
	{
	  for(b=0; b<(x1-x); b++)
		{
	   TFT_DrawPoint(x+b,y+a);
	  }
	}	  	
} 
