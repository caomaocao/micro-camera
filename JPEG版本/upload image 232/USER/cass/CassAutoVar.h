#ifndef CASSAUTOVAR_H
#define CASSAUTOVAR_H
#include "CassType.h"
#ifdef  _GLOBAL_VAR_DEFINE_
#define  EXTERN_VAR
#else
#define  EXTERN_VAR    extern
#endif
#define LocalAddr 1
#define F_CPU 7372800
#define M_UART2
#define UART2BAUT 115200
#define IN_BUFLEN_MAX_UART2 71
#define OUT_BUFLEN_MAX_UART2 71
#define EngineFirstTime 1
#define EngineSecondTime 2
#define EngineDcsTime 1000
#define CPUDATA32

#define M_T0_INT
#define M_T0_1MS
#define XStart 0 //输入区起始偏移
#define XEnd 3 //输入区结束偏移
EXTERN_VAR uint32 OldValue[1];//端口延时数据存放区
#define YStart 8 //输出区起始偏移
#define YEnd 11 //输出区结束偏移
#define MStart 24 //M区起始偏移
#define MEnd 73 //M区结束偏移
#define SStart 224 //S区起始偏移
#define SEnd 224 //S区结束偏移
#define TStart 226 //T区起始偏移
#define TEnd 233 //T区结束偏移
#define TCurStart 258 //T区当前值地址起始偏移
#define TCurEnd 385 //T区当前值地址结束偏移
#define TSetStart 386 //T区设定值地址起始偏移
#define TSetEnd 513 //T区设定值地址结束偏移
#define CStart 514 //C区起始偏移
#define CEnd 514 //C区结束偏移
#define CCurStart 520 //C区当前值地址起始偏移
#define CCurEnd 535 //C区当前值地址结束偏移
#define CSetStart 536 //C区设定值地址起始偏移
#define CSetEnd 551 //C区设定值地址结束偏移
#define CDStart 552 //CD区起始偏移
#define CDEnd 552 //CD区结束偏移
#define CDCurStart 560 //C区当前值地址起始偏移
#define CDCurEnd 591 //C区当前值地址结束偏移
#define CDSetStart 592 //C区设定值地址起始偏移
#define CDSetEnd 623 //C区设定值地址结束偏移
#define CQStart 624 //CQ区起始偏移
#define CQEnd 623 //CQ区结束偏移
#define CQCurStart 624 //CQ区当前值地址起始偏移
#define CQCurEnd 624 //CQ区当前值地址结束偏移
#define CQSetStart 624 //CQ区设定值地址起始偏移
#define CQSetEnd 623 //CQ区设定值地址结束偏移
#define TCQStart 624 //TCQ区起始偏移
#define TCQEnd 623 //TCQ区结束偏移
#define TCQCurStart 624 //TCQ区当前值地址起始偏移
#define TCQCurEnd 623 //TCQ区当前值地址结束偏移
#define TCQSetStart 624 //TCQ区设定值地址起始偏移
#define TCQSetEnd 623 //TCQ区设定值地址结束偏移
#define DStart 624 //D区起始偏移
#define DEnd 1823 //D区结束偏移
#define CASSVALUEF 1824
//extern uint8 CassMem[1828];

#define SMALLMODE
#define CPUDATALEN 32


//以下定义是为了高速计数器的使用
#define VALUEY0 8
#define VALUEY1 9
#define VALUEY2 10
#define VALUEY3 11

//Define C Function Flag 
#endif

