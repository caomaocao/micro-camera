#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h" 
#include "beep.h"	 	 
#include "key.h"	 	 
#include "exti.h"	 	 
#include "wdg.h" 	 
#include "timer.h"		 	 
#include "tpad.h"
#include "oled.h"		 	 
#include "lcd.h"
#include "usmart.h"	
#include "rtc.h"	 	 
#include "wkup.h"	
#include "adc.h" 	 
#include "dac.h" 	 
#include "dma.h" 	 
#include "24cxx.h" 	 
#include "flash.h" 	 
#include "rs485.h" 	 
#include "can.h" 	 
#include "touch.h" 	 
#include "remote.h" 	 
#include "joypad.h"
#include "adxl345.h"
#include "ds18b20.h"
#include "dht11.h"
#include "24l01.h"
#include "mouse.h"
#include "stmflash.h"
#include "rda5820.h"
#include "audiosel.h"
#include "ov7670.h"
#include "malloc.h"
#include "sram.h"


const u8*LMODE_TBL[5]={"Auto","Sunny","Cloudy","Office","Home"};							//5种光照模式	    
const u8*EFFECTS_TBL[7]={"Normal","Negative","B&W","Redish","Greenish","Bluish","Antique"};	//7种特效 
u8 lightmode=0,saturation=0,brightness=0,contrast=0,effect=0;

extern u8 ov_sta;	//在exit.c里面定义
extern u8 ov_frame;	//在timer.c里面定义		 

u8 CassMem[4096];
extern u8 raw_data[];
extern u8 out_data[];
extern u8 ComInDataOk;

u8 sensor_id = 1;
u8 update_outbuf_flag = 1;
	
static void sys_image_var_init()
{

}



void Update_Outbuf()
{
	 u16 i;
	 u8 *ptrData;
   u16 tempData;
	 u16 blk_num = BMP_IMAGE_SIZE/UART_SND_BLK_DATA_SIZE; //RGB565

	 for(i=0; i<blk_num; i++)
	 {
		   ptrData = (u8 *)(out_data+i*UART_SND_BLK_SIZE);
		   memcpy(ptrData, &sensor_id, 1);
			 memcpy(ptrData+2, (u8 *)&i, 2);
			 tempData = UART_SND_BLK_DATA_SIZE;
			 memcpy(ptrData+4, (u8 *)&tempData, 2);
			 memcpy(ptrData+6, (u8 *)(raw_data+i*UART_SND_BLK_DATA_SIZE), UART_SND_BLK_DATA_SIZE);		
			tempData = 0x55AA;
			memcpy(ptrData+UART_SND_BLK_DATA_SIZE+6, (u8 *)&tempData, 2);
	 }
	 
	 //最后一个块
	 ptrData = (u8 *)(out_data+i*UART_SND_BLK_SIZE);
	 memcpy(ptrData, &sensor_id, 2);
	 memcpy(ptrData+2, &i, 2);
	 
	 //长度
	 tempData = BMP_IMAGE_SIZE - blk_num*UART_SND_BLK_DATA_SIZE;
	 memcpy(ptrData+4, &tempData, 2);
	 memcpy(ptrData+6, raw_data+i*UART_SND_BLK_DATA_SIZE, tempData);
	 
	 tempData = 0x55AA;
	 memcpy(ptrData+600+6, &tempData, 2);
	 
}

void Camera_refresh()
{
	u32 y,j;
	u16 color,temp;
	u16 *ptrData;
	
	if(ov_sta==2)
	{			
		OV7670_RRST=0;				//开始复位读指针 
		OV7670_RCK=0;
		OV7670_RCK=1;
		OV7670_RCK=0;
		OV7670_RRST=1;				//复位读指针结束 
		OV7670_RCK=1;  	
		
 		for(y=0;y<76800;y++)//宽
		{
				OV7670_RCK=0;
				color=GPIOC->IDR&0XFF;	//读数据
				OV7670_RCK=1; 
				color<<=8;  
				OV7670_RCK=0;
				color|=GPIOC->IDR&0XFF;	//读数据
				OV7670_RCK=1; 
				raw_data[2*y+1]= (color >> 8) & 0xFF;
			  raw_data[2*y]= ((u8)color)& 0xFF;
				
				//raw_data[*320+j]
			
		}		
 		EXTI->PR=1<<8;     			//清除LINE8上的中断标志位
		ov_sta=0;					//开始下一次采集
 		ov_frame++;

		//LCD_Scan_Dir(U2D_L2R);		//设定LCD刷新方向
		//LCD_SetCursor(0x00,0x0000);	//设置光标位置 
		//LCD_WriteRAM_Prepare();     //开始写入GRAM	
		
// 		ptrData = (u16 *)raw_data;
// 		for(y=0;y<76800;y++)
// 		{
// 			  LCD->LCD_RAM=*ptrData;
// 				ptrData++;
// 		}
	} 
}	

int main(void)
{
	u32 i,j;
	u8 tempsaturation=0,tempcontrast=0,tempbrightness;
	u16 temp;
	
	Stm32_Clock_Init(9);	//系统时钟设置
	sys_image_var_init();
	uart_init(36000000,115200); 	//串口1初始化  
	delay_init(72);			//延时初始化	
	FSMC_SRAM_Init();
	OV7670_Init();	
	
	/*调整OV7670效果*/
	OV7670_Light_Mode(lightmode);
	OV7670_Color_Saturation(saturation);
	OV7670_Brightness(brightness);
	OV7670_Contrast(contrast);
 	OV7670_Special_Effects(effect);	 	
	
	TIM3_Int_Init(9,7199);//10Khz的计数频率，计数到10为1ms  
	TIM6_Int_Init(10000,7199);			//10Khz计数频率,1秒钟中断									  
	EXTI8_Init();						//使能定时器捕获
	OV7670_Window_Set(10,174,240,320);	//设置窗口	  
  OV7670_CS=0;				    		    
	ov_sta=2;//使能OV7670
	ComInDataOk = 0;
	
	while(1)//识别每一帧	
	{	
		
		tempbrightness=brightness;
		tempcontrast=contrast;
		tempsaturation=saturation;
		
		if(ComInDataOk)
		{
			ComInDataOk = 0; 
			UartRecvHandle(0);
		}
		
			/*调整镜头参数*/
			brightness=CassMem[167];
			contrast=CassMem[168];
			saturation=CassMem[169];
			if((brightness!=tempbrightness)||(contrast!=tempcontrast)||(saturation!=tempsaturation))
			{	
				OV7670_Brightness(brightness);	
				OV7670_Contrast(contrast);
				OV7670_Color_Saturation(saturation);
				tempbrightness=brightness;
				tempcontrast=contrast;
				tempsaturation=saturation;
			}
			
		Camera_refresh();//OV7670刷新  
 		
		if(update_outbuf_flag)
 			 Update_Outbuf();
					
	}
}

















