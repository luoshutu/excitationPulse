

//IO��������	  
#define GT_RST_H    		GPIO_SetBits(GPIOB, GPIO_Pin_7);					//GT9147��λ�����ø�
#define GT_RST_L    		GPIO_ResetBits(GPIOB, GPIO_Pin_7);					//GT9147��λ�����õ�
#define GT_INT    			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6);      	    //GT9147�ж�����	
 
//I2C��д����	
#define GT_CMD_WR 		0X70     	//д����
#define GT_CMD_RD 		0X71		  //������
#define GT_TP_REG 		0X80      //���������ݵ�ַ
 
unsigned char GT9147_WR_Reg(unsigned short reg,unsigned char *buf,unsigned char len);
void GT9147_RD_Reg(unsigned short reg,unsigned char *buf,unsigned char len); 
unsigned char GT9147_Init(void);
unsigned char GT9147_Scan(void); 













