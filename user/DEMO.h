#ifdef GLOBALS 
#define EXT
#else
#define EXT extern 
#endif

#define Led_ON()   GPIO_SetBits(GPIOB, GPIO_Pin_5);  	      //LED1��
#define Led_OFF()  GPIO_ResetBits(GPIOB, GPIO_Pin_5); 	      //LED2��

#define TP_CS()  GPIO_ResetBits(GPIOB,GPIO_Pin_7)	  		  //����SPI Ƭѡ����
#define TP_DCS() GPIO_SetBits(GPIOB,GPIO_Pin_7)				  //����SPI Ƭѡ��ֹ
#define PEN  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)          //�������  

#define RED	  0XF800
#define GREEN 0X07E0
#define BLUE  0X001F  
#define BRED  0XF81F
#define GRED  0XFFE0
#define GBLUE 0X07FF
#define BLACK 0
#define WHITE 0xffff




