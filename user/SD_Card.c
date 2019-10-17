#include "stdio.h"
#include "stm32f10x.h"
#include "stm32f10x_sdio.h"
#include <string.h>
#include "tjpgd.h"
#include "integer.h"
#include "ff.h"
#include "ffconf.h"
#include "diskio.h"

#include "core_cm3.h"
#include "sdio_sdcard.h"

#define _DF1S	0x81
#define Bank1_LCD_D    ((uint32_t)0x60020000)    //��ʾ�����ݵ�ַ	  
#define Bank1_LCD_C    ((uint32_t)0x60000000)	 //��ʾ��ָ���ַ

SD_CardInfo SDCardInfo;
uint32_t Buffer_Block_Tx[512], Buffer_Block_Rx[512]; 
SD_Error Status = SD_OK;

void Disp_Mask (
	int left,		/* Left end of active window (0 to DISP_XS-1) */
	int right,		/* Right end of active window (0 to DISP_XS-1, >=left) */
	int top,		/* Top end of active window (0 to DISP_YS-1) */
	int bottom		/* Bottom end of active window (0 to DISP_YS-1, >=top) */
);
static int MaskT, MaskL, MaskR, MaskB;	/* Active drawing area */

extern void LCD_WR_CMD(unsigned int index,unsigned int val);
extern void LCD_WR_Data(unsigned int val);
extern void Delay(__IO uint32_t nCount);
extern void LCD_WR_REG(unsigned int index);
extern void TFT_ShowString(u16 x,u16 y,const u8 *p,u16 Color);	
	
FATFS fs;                      // �߼��������ı�־
FIL fsrc, fdst;                // �ļ���־ 
JDEC jdec;       			  /* ��ѹĿ�� */
//unsigned char buffer[8];     // �ļ����ݻ�����
BYTE buffer[8192] __attribute__ ((aligned(4)));
FRESULT res;                   // FatFs ���ܺ������ؽ������
unsigned int br, bw;           // �ļ���/д������
    
/* User defined device identifier */
typedef struct {
    FILE *fp;      /* File pointer for input function */
    BYTE *fbuf;    /* Pointer to the frame buffer for output function */
    UINT wfbuf;    /* Width of the frame buffer [pix] */
} IODEV;
	
/****************************************************************************
* ��    �ƣ�in_func
* ��    �ܣ�jpg����ص�����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
UINT in_func( 
   JDEC* jd,          /* Decoder object */ 
   BYTE* buff,        /* Pointer to the read buffer (NULL:skip) */ 
    UINT nd           /* Number of bytes to read/skip from input stream */ 
) 
{ 
    UINT rb; 
    FIL *fil = (FIL*)jd->device;    /* Input stream of this session */   
    if (buff)     					/* Read nd bytes from the input strem */ 
    { 
        f_read(fil, buff, nd, &rb);		
        return rb;       		    /* Returns number of bytes could be read */ 
    }  
    else  
    { 	
        return (f_lseek(fil, f_tell(fil) + nd) == FR_OK) ? nd : 0;/* Skip nd bytes on the input stream */ 
    } 
} 


/****************************************************************************
* ��    �ƣ�void STM32_disp_blt
* ��    �ܣ�jpg��ʾ����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/

// ����Ļ����ʾͼƬ 
void STM32_disp_blt (
     int left,        /* Left end (-32768 to 32767) */ 
     int right,       /* Right end (-32768 to 32767, >=left) */ 
     int top,         /* Top end (-32768 to 32767) */ 
     int bottom,      /* Bottom end (-32768 to 32767, >=right) */ 
     const uint16_t *pat    /* Pattern data */ 
) 
{ 
	int yc, xc, xl, xs;
	if (left > right || top > bottom) return; 	// Check varidity 
	if (left > MaskR || right < MaskL  || top > MaskB || bottom < MaskT) return;	// Check if in active area 

	yc = bottom - top + 1;			// Vertical size 
	xc = right - left + 1; xs = 0;	//Horizontal size and skip 

	if (top < MaskT) {		// Clip top of source image if it is out of active area 
		pat += xc * (MaskT - top);
		yc -= MaskT - top;
		top = MaskT;
	}
	if (bottom > MaskB) {	// Clip bottom of source image if it is out of active area 
		yc -= bottom - MaskB;
		bottom = MaskB;
	}
	if (left < MaskL) {		// Clip left of source image if it is out of active area 
		pat += MaskL - left;
		xc -= MaskL - left;
		xs += MaskL - left;
		left = MaskL;
	}
	if (right > MaskR) {	 // Clip right of source image it is out of active area 
		xc -= right - MaskR;
		xs += right - MaskR;
		right = MaskR;
	}

	/*  ???????? */
	*(__IO uint16_t *) (Bank1_LCD_C)= 0x002a;	
	*(__IO uint16_t *) (Bank1_LCD_D)= left>>8;
	*(__IO uint16_t *) (Bank1_LCD_D)= left&0xff;
	*(__IO uint16_t *) (Bank1_LCD_D)= right>>8;	
	*(__IO uint16_t *) (Bank1_LCD_D)= right&0xff;


	*(__IO uint16_t *) (Bank1_LCD_C)= 0x002b;	
	*(__IO uint16_t *) (Bank1_LCD_D)= top>>8;
	*(__IO uint16_t *) (Bank1_LCD_D)= top&0xff;
	*(__IO uint16_t *) (Bank1_LCD_D)= bottom>>8;
	*(__IO uint16_t *) (Bank1_LCD_D)= bottom&0xff;

	*(__IO uint16_t *) (Bank1_LCD_C)= 0x002C;

	
	do {	/* Send image data */
		xl = xc;
		do {
			*(__IO uint16_t *) (Bank1_LCD_D)=*pat;			//????
			pat++;
		} while (--xl);	
		//pat += xs;
	} while (--yc);
}
/****************************************************************************
* ��    �ƣ�UINT out_func (JDEC* jd, void* bitmap, JRECT* rect)
* ��    �ܣ�jpg����ص�����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
UINT out_func (JDEC* jd, void* bitmap, JRECT* rect)
{
  jd=jd;
	STM32_disp_blt(rect->left, rect->right, rect->top, rect->bottom, (uint16_t*)bitmap); 
	return 1;    /* Continue to decompress */
}

/****************************************************************************
* ��    �ƣ�void OutPutFile(void)
* ��    �ܣ�TXT�ļ��������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void OutPutFile(char ImageName)
{ 
  u8 scale;
  FILINFO finfo;
  DIR dirs;
  JRESULT res1;      /* Result code of TJpgDec API */
  
  char path[50]={""}; 	  						 //Ŀ¼��Ϊ�գ���ʾ�Ǹ�Ŀ¼
  char *result1, *result2; 
	
  /*�������ļ�������ʱ�� ҪԤ�ȳ�ʼ���ļ����������ĳ��� */
  #if _USE_LFN
    static char lfn[_MAX_LFN * (_DF1S ? 2 : 1) + 1];
    finfo.lfname = lfn;
    finfo.lfsize = sizeof(lfn);
  #endif
  disk_initialize(0);						     //fatfs���Թ��������ʵķ����� ���԰�����洢����SD����ʾΪ0�����൱�ڴ��̱��
  f_mount(0, &fs);							     //���ļ�ϵͳ���õ�0�� 
  
	if (f_opendir(&dirs, path) == FR_OK)            //��ȡ�ô��̵ĸ�Ŀ¼        
  {
    while (f_readdir(&dirs, &finfo) == FR_OK)  	 //ѭ�����ζ�ȡ�ļ���
    {	 
      if (finfo.fattrib & AM_ARC) 			     		//�ж��ļ������Ƿ�Ϊ�浵��	 TXT�ļ�һ�㶼Ϊ�浵��
      {
				if(!finfo.fname[0])	 break;    		     		//������ļ���Ϊ�ձ�ʾ��Ŀ¼��ĩβ���˳�
				
				if(finfo.fname[0] == ImageName)
				{  
					if(ImageName == '1')
						Disp_Mask(0, 480, 0, 272);		 //���û�ͼ����
					else if(ImageName == '2')
						Disp_Mask(0, 480, 0, 272);
//					else if(ImageName == '2')
//						Disp_Mask(219, 249, 44, 80);
//					else if(ImageName == '3')
//						Disp_Mask(321, 351, 43, 79);
//					else if(ImageName == '4')
//						Disp_Mask(424, 454, 44, 80);
//					else if(ImageName == '5')
//						Disp_Mask(0, 480, 0, 272);
					
					result1=strstr(finfo.fname,".JPG");								 //�ж��Ƿ���bin��Сд����׺���ļ���
					result2=strstr(finfo.fname,".jpg");	     						 //�ȽϺ�׺�Ƿ�bin����д����׺���ļ���				
				
					if(result1!=NULL||result2!=NULL)									//�ǵĻ�������ļ�������	
					{	 								  								  		 			  
						res = f_open(&fsrc, finfo.fname, FA_OPEN_EXISTING | FA_READ); //�Զ��ķ�ʽ���ļ�
					
						/* ׼����ѹ */
						res1 = jd_prepare(&jdec, in_func, buffer, 8192, &fsrc);

						if(res1 == JDR_OK)
						{
							/* ȷ���������� */
							for (scale = 0; scale < 3; scale++) 
							{
								if (((jdec.width >> scale) <= 480) && ((jdec.height >> scale) <= 272))break;
							}
							res1 = jd_decomp(&jdec, out_func,scale);   /* �����������ӽ�ѹJPEG�ļ�����ʾ */
						}
					}						
				}
				f_close(&fsrc);				 //�ر�Դ�ļ�
				Delay(0xff);													   
			}											   
   	}
  } 
}
/****************************************************************************
* ��    �ƣ�void SD_TEST(void)
* ��    �ܣ�SD�����Ժ���
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void SD_TEST(void)
{
	Status = SD_Init();
	Status = SD_GetCardInfo(&SDCardInfo);
	Status = SD_SelectDeselect((uint32_t) (SDCardInfo.RCA << 16));
	Status = SD_EnableWideBusOperation(SDIO_BusWide_4b);
	Status = SD_SetDeviceMode(SD_DMA_MODE);  
	if (Status == SD_OK)
	{
			// �ӵ�ַ0��ʼ��ȡ512�ֽ�  
		Status = SD_ReadBlock(Buffer_Block_Rx, 0x00,  512); 
	}
	if(Status == SD_OK)
	{
		// ���سɹ��Ļ�����ʾSD�����Գɹ���Ϣ 
		TFT_ShowString(200,100,"SD_Card_Successfully",WHITE); 
	}
	else
	{	
		TFT_ShowString(200,100,"SD_Card_Fail",WHITE);
	} 
}

void Disp_Mask (
	int left,		/* Left end of active window (0 to DISP_XS-1) */
	int right,		/* Right end of active window (0 to DISP_XS-1, >=left) */
	int top,		/* Top end of active window (0 to DISP_YS-1) */
	int bottom		/* Bottom end of active window (0 to DISP_YS-1, >=top) */
)
{
	{
		MaskL = left;
		MaskR = right;
		MaskT = top;
		MaskB = bottom;
	}
}
