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
#define Bank1_LCD_D    ((uint32_t)0x60020000)    //显示区数据地址	  
#define Bank1_LCD_C    ((uint32_t)0x60000000)	 //显示区指令地址

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
	
FATFS fs;                      // 逻辑驱动器的标志
FIL fsrc, fdst;                // 文件标志 
JDEC jdec;       			  /* 解压目标 */
//unsigned char buffer[8];     // 文件内容缓冲区
BYTE buffer[8192] __attribute__ ((aligned(4)));
FRESULT res;                   // FatFs 功能函数返回结果变量
unsigned int br, bw;           // 文件读/写计数器
    
/* User defined device identifier */
typedef struct {
    FILE *fp;      /* File pointer for input function */
    BYTE *fbuf;    /* Pointer to the frame buffer for output function */
    UINT wfbuf;    /* Width of the frame buffer [pix] */
} IODEV;
	
/****************************************************************************
* 名    称：in_func
* 功    能：jpg解码回调函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
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
* 名    称：void STM32_disp_blt
* 功    能：jpg显示函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/

// 在屏幕上显示图片 
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
* 名    称：UINT out_func (JDEC* jd, void* bitmap, JRECT* rect)
* 功    能：jpg解码回调函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
UINT out_func (JDEC* jd, void* bitmap, JRECT* rect)
{
  jd=jd;
	STM32_disp_blt(rect->left, rect->right, rect->top, rect->bottom, (uint16_t*)bitmap); 
	return 1;    /* Continue to decompress */
}

/****************************************************************************
* 名    称：void OutPutFile(void)
* 功    能：TXT文件输出函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void OutPutFile(char ImageName)
{ 
  u8 scale;
  FILINFO finfo;
  DIR dirs;
  JRESULT res1;      /* Result code of TJpgDec API */
  
  char path[50]={""}; 	  						 //目录名为空，表示是根目录
  char *result1, *result2; 
	
  /*开启长文件名功能时， 要预先初始化文件名缓冲区的长度 */
  #if _USE_LFN
    static char lfn[_MAX_LFN * (_DF1S ? 2 : 1) + 1];
    finfo.lfname = lfn;
    finfo.lfsize = sizeof(lfn);
  #endif
  disk_initialize(0);						     //fatfs可以管理多个介质的分区， 所以把物理存储介质SD卡标示为0区，相当于磁盘编号
  f_mount(0, &fs);							     //将文件系统设置到0区 
  
	if (f_opendir(&dirs, path) == FR_OK)            //读取该磁盘的根目录        
  {
    while (f_readdir(&dirs, &finfo) == FR_OK)  	 //循环依次读取文件名
    {	 
      if (finfo.fattrib & AM_ARC) 			     		//判断文件属性是否为存档型	 TXT文件一般都为存档型
      {
				if(!finfo.fname[0])	 break;    		     		//如果是文件名为空表示到目录的末尾。退出
				
				if(finfo.fname[0] == ImageName)
				{  
					if(ImageName == '1')
						Disp_Mask(0, 480, 0, 272);		 //设置绘图区域
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
					
					result1=strstr(finfo.fname,".JPG");								 //判断是否是bin（小写）后缀的文件名
					result2=strstr(finfo.fname,".jpg");	     						 //比较后缀是否bin（大写）后缀的文件名				
				
					if(result1!=NULL||result2!=NULL)									//是的话就输出文件的内容	
					{	 								  								  		 			  
						res = f_open(&fsrc, finfo.fname, FA_OPEN_EXISTING | FA_READ); //以读的方式打开文件
					
						/* 准备解压 */
						res1 = jd_prepare(&jdec, in_func, buffer, 8192, &fsrc);

						if(res1 == JDR_OK)
						{
							/* 确定缩放因子 */
							for (scale = 0; scale < 3; scale++) 
							{
								if (((jdec.width >> scale) <= 480) && ((jdec.height >> scale) <= 272))break;
							}
							res1 = jd_decomp(&jdec, out_func,scale);   /* 根据缩放因子解压JPEG文件并显示 */
						}
					}						
				}
				f_close(&fsrc);				 //关闭源文件
				Delay(0xff);													   
			}											   
   	}
  } 
}
/****************************************************************************
* 名    称：void SD_TEST(void)
* 功    能：SD卡测试函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
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
			// 从地址0开始读取512字节  
		Status = SD_ReadBlock(Buffer_Block_Rx, 0x00,  512); 
	}
	if(Status == SD_OK)
	{
		// 返回成功的话，显示SD卡测试成功信息 
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
