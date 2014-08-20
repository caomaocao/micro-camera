/******************** (C) COPYRIGHT 2009 HDU ********************
* File Name          : configuration_system.h
* Author             : HWD
* Version            : V1.0
* Date               : 10/08/2009
* Description        : 系统的头文件,引擎中抽象驱动接口的配置头文件
*                      
********************************************************************************/
#ifndef _CONFIGURATION_SYSTEM_H
#define _CONFIGURATION_SYSTEM_H	

#include "configuration_def.h"
#include "CassType.h"
//#include "CassAutoVar.h"
#include "math.h"

union FloatChange
{
	fp32 f;	   // 浮点数
	uint8 uc[4];   // 4个字节
	uint32 ul;	   // 长整型
};


extern uint32 Configuration_ReadParauint32(uint32 addr);
extern fp32 Configuration_ReadParafp32(uint32 addr);
extern uint8 Configuration_ReadParauint8(uint32 addr);
extern uint32* Configuration_ReadParauint32Array(uint32 addr,uint16 number);
extern fp32* Configuration_ReadParafp32Array(uint32 addr,uint16 number);
extern uint8* Configuration_ReadParauint8Array(uint32 addr,uint16 number);


extern void Configuration_ConfigSys(void);

extern fp32 ABS(fp32 data);
extern fp32 ACOS(fp32 data);
extern fp32 ASIN(fp32 data);
extern fp32 COS(fp32 data);
extern fp32 SIN(fp32 data);
extern fp32 LN(fp32 data);
extern fp32 SQRT(fp32 data);
extern fp32 TRUN(fp32 data);
extern fp32 EXP(fp32 data);
extern fp32 ATAN(fp32 data);
extern fp32 TAN(fp32 data);
extern fp32 LG(fp32 data);
extern fp32 MAX(fp32 data1,fp32 data2);
extern fp32 MIN(fp32 data1,fp32 data2);
extern fp32 POW(fp32 data1,fp32 data2);
#endif










