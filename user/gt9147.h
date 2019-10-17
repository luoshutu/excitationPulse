

//IO操作函数	  
#define GT_RST_H    		GPIO_SetBits(GPIOB, GPIO_Pin_7);					//GT9147复位引脚置高
#define GT_RST_L    		GPIO_ResetBits(GPIOB, GPIO_Pin_7);					//GT9147复位引脚置低
#define GT_INT    			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6);      	    //GT9147中断引脚	
 
//I2C读写命令	
#define GT_CMD_WR 		0X70     	//写命令
#define GT_CMD_RD 		0X71		  //读命令
#define GT_TP_REG 		0X80      //触摸点数据地址
 
unsigned char GT9147_WR_Reg(unsigned short reg,unsigned char *buf,unsigned char len);
void GT9147_RD_Reg(unsigned short reg,unsigned char *buf,unsigned char len); 
unsigned char GT9147_Init(void);
unsigned char GT9147_Scan(void); 













