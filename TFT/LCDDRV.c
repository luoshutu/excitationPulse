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

u16 POINT_COLOR=RED;     				//Ĭ�Ϻ�ɫ  

/****************************************************************************
* ��    �ƣ�LCD_WR_REG(unsigned int index)
* ��    �ܣ�FSMCд��ʾ���Ĵ�����ַ����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void LCD_WR_REG(unsigned int index)
{
	*(__IO uint16_t *) (Bank1_LCD_C)= index;

}			 

/****************************************************************************
* ��    �ƣ�void LCD_WR_CMD(unsigned int index,unsigned int val)
* ��    �ܣ�FSMCд��ʾ���Ĵ������ݺ���
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void LCD_WR_CMD(unsigned int index,unsigned int val)
{	
	*(__IO uint16_t *) (Bank1_LCD_C)= index;	
	*(__IO uint16_t *) (Bank1_LCD_D)= val;
}


/****************************************************************************
* ��    �ƣ�unsigned short LCD_RD_data(void)
* ��    �ܣ�FSMC����ʾ��16λ���ݺ���
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
unsigned short LCD_RD_data(void){
	unsigned int a=0;
	a=*(__IO uint16_t *) (Bank1_LCD_D);   //�ղ���
	a=*(__IO uint16_t *) (Bank1_LCD_D);   //������ʵ��16λ��������	  
	return(a);	
}



/****************************************************************************
* ��    �ƣ�LCD_WR_Data(unsigned int val)
* ��    �ܣ�FSMCд16λ���ݺ���
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
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
* ��    �ƣ�void Delay(__IO uint32_t nCount)
* ��    �ܣ���ʱ����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
/****************************************************************************
* ��    �ƣ�void LCD_Init(void)
* ��    �ܣ�2.4 ��TFT ��ʼ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void LCD_Init_SD(void)
{	
	GPIO_ResetBits(GPIOE, GPIO_Pin_1);
	Delay(0xAFFFF);					   
	GPIO_SetBits(GPIOE, GPIO_Pin_1 );		 	 
	Delay(0xAFFFF);

	LCD_WR_REG(0x00E2);	        //PLL ��Ƶ��120M   PLL=10*36/3=120M
	LCD_WR_Data(0x0023);	    //N=0x36 for 6.5M, 0x23 for 10M crystal
	LCD_WR_Data(0x0002);
	LCD_WR_Data(0x0004);

	LCD_WR_REG(0x00E0);         
	LCD_WR_Data(0x0001);
	Delay(0xAFFF);
	LCD_WR_REG(0x00E0);
	LCD_WR_Data(0x0003);		// PLL ʹ��  PLL��Ϊʱ��
	Delay(0xAFFF);
	LCD_WR_REG(0x0001);         // �����λ
	Delay(0xAFFF);
	LCD_WR_REG(0x00E6);	        //PLL setting for PCLK, depends on resolution
	LCD_WR_Data(0x0001);
	LCD_WR_Data(0x0033);
	LCD_WR_Data(0x0032);

	LCD_WR_REG(0x00B0);	        //����LCDģʽ
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


	LCD_WR_REG(0x00F0);             //16λ  565ģʽ
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

	LCD_WR_REG(0x00d0);//���ö�̬����������� 
	LCD_WR_Data(0x000d);
}

/****************************************************************************
* ��    �ƣ�void TFT_CLEAR(u8 x,u16 y,u8 len,u16 wid)
* ��    �ܣ�2.4 ��TFT ��������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
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
* ��    �ƣ�void TFT_DrawPoint(u8 x,u16 y)
* ��    �ܣ�2.4 ��TFT ���㺯��
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void TFT_DrawPoint(u16 x,u16 y)
{								       
	LCD_WR_REG(0x002A);				    
	LCD_WR_Data(x>>8);	    			//��ʼ��ַX
	LCD_WR_Data(x);	    			
	LCD_WR_Data(x>>8);	    			//������ַX
	LCD_WR_Data(x);	    			    
  LCD_WR_REG(0x002b);	
	LCD_WR_Data(y>>8);	    			//��ʼ��ַY
	LCD_WR_Data(y);	    			    
	LCD_WR_Data(y>>8);	    			//������ַY
	LCD_WR_Data(y);	    			
	LCD_WR_REG(0x2c);
	LCD_WR_Data(POINT_COLOR); 	   	
} 
  
/****************************************************************************
* ��    �ƣ�void TFT_ShowChar(u8 x,u16 y,u8 num)
* ��    �ܣ���ָ��λ����ʾһ���ַ�
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void TFT_ShowChar(u16 x,u16 y,u8 num,u16 COLOR)
{       
	#define MAX_CHAR_POSX 464
	#define MAX_CHAR_POSY 262
	u8 temp;
	u8 pos,t;      
	if(x>MAX_CHAR_POSX||y>MAX_CHAR_POSY)return;	

	num=num-' ';						//�õ�ƫ�ƺ��ֵ
	for(pos=0;pos<16;pos++)	
	{
	  temp=asc2_1608[num][pos];		//�����ģ�����ֵ		   				
		for(t=0;t<8;t++)
		{                 
			LCD_WR_REG(0x002A);	    
			LCD_WR_Data((x+t)>>8);	    			//��ʼ��ַX
			LCD_WR_Data((x+t)&0x00ff);	    			
			LCD_WR_Data((x+t)>>8);	    			//������ַX
			LCD_WR_Data((x+t)&0x00ff);	    			    
			
			LCD_WR_REG(0x002B);
			LCD_WR_Data((y+pos)>>8);	    			//��ʼ��ַY
			LCD_WR_Data((y+pos)&0x00ff);	    			    
			LCD_WR_Data((y+pos)>>8);	    			//������ַY
			LCD_WR_Data((y+pos)&0x00ff);	
			
			LCD_WR_REG(0x2c);
			
			if(temp&0x01)LCD_WR_Data(COLOR);	  //λΪ1 ��ָ����ɫд�뵽����
			else LCD_WR_Data(WHITE);                 //λΪ0 �ñ���ɫд�뵽����    
			temp>>=1; 
		}
	}
 	
/*	//����������ɫ
	LCD_WR_REG(0x002A);				    
	LCD_WR_Data(x>>8);	    			//��ʼ��ַX
	LCD_WR_Data(x&0x00ff);
	LCD_WR_Data((x+7)>>8);	    		//������ַX
	LCD_WR_Data((x+7)&0x00ff);
  
	LCD_WR_REG(0x002B);	
	LCD_WR_Data(y>>8);	    			//��ʼ��ַY
	LCD_WR_Data(y&0x00ff);
	LCD_WR_Data((y+15)>>8);	    	 	//������ַY
	LCD_WR_Data((y+15)&0x00ff);
	
	LCD_WR_REG(0x2c);
	 
	num=num-' ';						//�õ�ƫ�ƺ��ֵ
	for(pos=0;pos<16;pos++)	{
	    temp=asc2_1608[num][pos];		//�����ģ�����ֵ
	    for(t=0;t<8;t++)
	    {                 
	      if(temp&0x01)LCD_WR_Data(POINT_COLOR);	  //λΪ1 ��ָ����ɫд�뵽����
	      else LCD_WR_Data(COLOR);                 //λΪ0 �ð�ɫд�뵽����    
	      temp>>=1; 
	    }
	}
*/
}  	 
/****************************************************************************
* ��    �ƣ�u32 mn(u8 m,u8 n)
* ��    �ܣ�
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
u32 mn(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}
/****************************************************************************
* ��    �ƣ�void TFT_ShowNum(u8 x,u16 y,u32 num)
* ��    �ܣ���ָ��λ����ʾһ������0-65536
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void TFT_ShowNum(u16 x,u16 y,u32 num,u16 COLOR)
{      
	u32 res;   	   
	u8 t=0,t1=0;   
	res=num;
	if(!num)TFT_ShowChar(x,y,'0',COLOR);		//��������Ϊ0 ��ָ��λ����ʾ0
	while(res)  						//�õ����ֳ���
	{
		res/=10;
		t++;
	}
	t1=t;
	while(t)							//��ʾ����
	{
		res=mn(10,t-1); 	 
	    TFT_ShowChar(x+(t1-t)*8,y,(num/res)%10+'0',COLOR);	 //��ָ��λ����ʾ����
		t--;
	}
	if(t1==3) TFT_ShowChar(x+18,y,' ',COLOR);	                 //����ֵ��3λ�Ļ������һ����ʾ�����				     
} 
/****************************************************************************
* ��    �ƣ�void TFT_ShowString(u8 x,u16 y,const u8 *p)
* ��    �ܣ���ָ��λ����ʾ�ַ���
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
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
* ��    �ƣ�void FillColor(u8 x,u16 y, u8 x1, u16 y1, u16 z)
* ��    �ܣ�����Ļ�ϻ�����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����
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
