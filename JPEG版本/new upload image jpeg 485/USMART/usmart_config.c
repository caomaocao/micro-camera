#include "usmart.h"
#include "usmart_str.h"										    
////////////////////////////用户配置区///////////////////////////////////////////////
//这下面要包含所用到的函数所申明的头文件(用户自己添加) 
#include "delay.h"	
#include "usart.h"		
#include "sys.h"
#include "lcd.h"
#include "rtc.h"
#include "dac.h"
#include "24cxx.h"	    
#include "stmflash.h"	    
#include "rda5820.h"	    
#include "ov7670.h"	    
									 											
//函数名列表初始化(用户自己添加)
//用户直接在这里输入要执行的函数名及其查找串
struct _m_usmart_nametab usmart_nametab[]=
{
#if USMART_USE_WRFUNS==1 	//如果使能了读写操作
	(void*)read_addr,"u32 read_addr(u32 addr)",
	(void*)write_addr,"void write_addr(u32 addr,u32 val)",	 
#endif		   
	(void*)delay_ms,"void delay_ms(u16 nms)",
 	(void*)delay_us,"void delay_us(u32 nus)",

 	(void*)RTC_Set,"u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)",	
 	(void*)Dac1_Set_Vol,"void Dac1_Set_Vol(u16 vol)",	   
   	(void*)AT24CXX_WriteOneByte,"void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)",	 
 	(void*)AT24CXX_ReadOneByte,"u8 AT24CXX_ReadOneByte(u16 ReadAddr)",	

	(void*)Test_Write,"void Test_Write(u32 WriteAddr,u16 WriteData)",	 
	(void*)STMFLASH_ReadHalfWord,"u16 STMFLASH_ReadHalfWord(u32 faddr)",
																						 
 	(void*)RDA5820_Rssi_Set,"void RDA5820_Rssi_Set(u8 rssi)",	 
	(void*)RDA5820_Band_Set,"void RDA5820_Band_Set(u8 band)",	 
	(void*)RDA5820_Freq_Set,"void RDA5820_Freq_Set(u16 freq)",	 
	(void*)RDA5820_Vol_Set,"void RDA5820_Vol_Set(u8 vol)",	 
	(void*)RDA5820_TxPGA_Set,"void RDA5820_TxPGA_Set(u8 gain)",	 

	(void*)SCCB_WR_Reg,"u8 SCCB_WR_Reg(u8 reg,u8 data)", 
	(void*)SCCB_RD_Reg,"u8 SCCB_RD_Reg(u8 reg)", 
};						  
///////////////////////////////////END///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//函数控制管理器初始化
//得到各个受控函数的名字
//得到函数总数量
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//函数数量
	0,	  	//参数数量
	0,	 	//函数ID
	1,		//参数显示类型,0,10进制;1,16进制
	0,		//参数类型.bitx:,0,数字;1,字符串	    
	0,	  	//每个参数的长度暂存表,需要MAX_PARM个0初始化
	0,		//函数的参数,需要PARM_LEN个0初始化
};   



















