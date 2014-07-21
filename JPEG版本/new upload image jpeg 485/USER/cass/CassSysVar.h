////////////////////////////////////////////////////////////////////////////////////////////
// 文   件   名  : CassSysVar.h
// 版        本  :                            
// 目的及主要功能: 系统全局变量和宏操作定义,其中IO端口，串口，IIC口定义需根据不同CPU特性修改
// 创 建 日  期  : 2008.3.5
// 修 改 日  期  : 2008.3.5                    
// 作        者  : hfl
// 修   改   者  :                      
////////////////////////////////////////////////////////////////////////////////////////////

#ifndef  CASSSYSVAR_H
#define  CASSSYSVAR_H

#include "CassSysCfg.h"
#include "CassType.h"
//#include "CassAutoVar.h"

//全局变量宏
#ifdef  _GLOBAL_VAR_DEFINE_
#define  EXTERN_VAR  
#else
#define  EXTERN_VAR    extern
#endif
//定时器
#define M_TIMER_INTERVAL 1000  //定时器最小的时间当量。单位 毫秒
//环境宏变量定义结束

//端口定义，需根据不同CPU进行定义，依次为输入口重新映射PLCIN000－PLCIN***；输出口重新映射PLCOUT000－PLCOUT***；输入口配置重新映射PLCINCFG000－PLCINCFG***；输出口配置重新映射PLCOUTCFG000－PLCOUTCFG***
#define PLCIN000 IO0PIN
#define PLCIN001 IO1PIN
#define PLCIN002 IO2PIN
#define PLCIN003 IO3PIN

#define PLCOUT000S0 IO0SET
#define PLCOUT000S1 IO0CLR
#define PLCOUT001S0 IO1SET
#define PLCOUT001S1 IO1CLR
#define PLCOUT002S0 IO2SET
#define PLCOUT002S1 IO2CLR
#define PLCOUT003S0 IO3SET
#define PLCOUT003S1 IO3CLR

#define PLCINCFG000 IO0DIR
#define PLCINCFG001 IO1DIR
#define PLCINCFG002 IO2DIR
#define PLCINCFG003 IO3DIR

#define PLCOUTCFG000 IO0DIR
#define PLCOUTCFG001 IO1DIR
#define PLCOUTCFG002 IO2DIR
#define PLCOUTCFG003 IO3DIR

#if defined MIDDLEMODE       //如果是中型系统，需添加中型CASS端口定义
#define PLCMIDIN000 7
#define PLCMIDIN001 6
#define PLCMIDIN002 5
#define PLCMIDIN003 4
#define PLCMIDIN004 3
#define PLCMIDIN005 2
#define PLCMIDIN006 1
#define PLCMIDIN007 0

#define PLCMIDOUT000 7
#define PLCMIDOUT001 6
#define PLCMIDOUT002 5
#define PLCMIDOUT003 4
#define PLCMIDOUT004 3
#define PLCMIDOUT005 2
#define PLCMIDOUT006 1
#define PLCMIDOUT007 0

#elif defined BIGMODE        //如果是大型系统，需添加中型CASS端口定义
#define PLCBIGIN000 0x0
#define PLCBIGIN001 0x0
#define PLCBIGIN002 0x0
#define PLCBIGIN003 0x0
#define PLCBIGIN004 0x0
#define PLCBIGIN005 0x0
#define PLCBIGIN006 0x0

#define PLCBIGOUT000 0x0
#define PLCBIGOUT001 0x0
#define PLCBIGOUT002 0x0
#define PLCBIGOUT003 0x0
#define PLCBIGOUT004 0x0
#define PLCBIGOUT005 0x0
#define PLCBIGOUT006 0x0


#endif

//反映状态的变量
EXTERN_VAR uint8 PLCRun;	//系统运行开关状态1 = stop,0 = run
EXTERN_VAR uint8 ErrorFlag ; //系统运行出错标志

#define COM_READ	3		//读功能号
#define COM_WRITE	6		//写功能号
EXTERN_VAR uint8  *pComInBuf ;				//接受缓冲区物理地址指针
EXTERN_VAR uint8  *pComOutBuf ;			//发送缓冲区逻辑地址指针
EXTERN_VAR uint8  *pComInBuf ;				//接收缓冲区逻辑地址指针
EXTERN_VAR uint8  *pComDataLen ;			//发送数据长度
EXTERN_VAR uint8  *pComDataCur ;			//接受数据位置指针
EXTERN_VAR uint8  *pComInTimeover ;			//接收到数据后等待下一字符的超时时间

//串口通讯相关定义
//由于要遵循以前的风格，串口定义从1开始
#ifdef M_UART0
//串口1内存定义
	EXTERN_VAR uint8 COMInBuf0[IN_BUFLEN_MAX_UART0] ;		//输入缓冲区
	EXTERN_VAR uint8 COMOutBuf0[IN_BUFLEN_MAX_UART0] ;	//输出缓冲区
	EXTERN_VAR uint8 COMDataLen0 ;						//输出数据长度
	EXTERN_VAR uint8 COMDataCur0 ;						//数据传输指针
	EXTERN_VAR uint8 COMInTimeover0 ;					//数据接收超时时间
	EXTERN_VAR uint8 COMInFrmFlag0 ;					//数据接收到一帧的标志
#endif
#ifdef M_UART1
//串口2内存定义
	EXTERN_VAR uint8 COMInBuf1[IN_BUFLEN_MAX_UART1] ;		//输入缓冲区
	EXTERN_VAR uint8 COMOutBuf1[IN_BUFLEN_MAX_UART1] ;	//输出缓冲区
	EXTERN_VAR uint8 COMDataLen1 ;						//输出数据长度
	EXTERN_VAR uint8 COMDataCur1 ;						//数据传输指针
	EXTERN_VAR uint8 COMInTimeover1 ;					//数据接收超时时间
	EXTERN_VAR uint8 COMInFrmFlag1 ;					//数据接收到一帧的标志
#endif

#ifdef M_UART2
//串口3内存定义
	EXTERN_VAR uint8 COMInBuf2[IN_BUFLEN_MAX_UART2] ;		//输入缓冲区
	EXTERN_VAR uint8 COMOutBuf2[IN_BUFLEN_MAX_UART2] ;	//输出缓冲区
	EXTERN_VAR uint8 COMDataLen2 ;						//输出数据长度
	EXTERN_VAR uint8 COMDataCur2 ;						//数据传输指针
	EXTERN_VAR uint8 COMInTimeover2 ;					//数据接收超时时间
	EXTERN_VAR uint8 COMInFrmFlag2 ;					//数据接收到一帧的标志
#endif
//…列出所有串口内存定义

EXTERN_VAR uint8 LastPLCRun;  //运行开关上一次状态

EXTERN_VAR uint16 nEngineFirstCurTime; //快速引擎逻辑的当前时间
EXTERN_VAR uint16 nEngineSecondCurTime;//慢速引擎的当前时间

EXTERN_VAR uint8 Time1msCur ; //用于累积10ms定时器
EXTERN_VAR uint8 Time2msCur ; //用于累积2ms定时器
EXTERN_VAR uint8 Time4msCur ; //用于累积4ms定时器
EXTERN_VAR uint8 Time5msCur ; //用于累积5ms定时器
EXTERN_VAR uint8 Time10msCur ;//用于累积100ms定时器
EXTERN_VAR uint8 Time100msCur ;//用于累积1000ms定时器

EXTERN_VAR uint8 T10msFlag ;//10ms事件到标记
EXTERN_VAR uint8 T100msFlag ;//100ms事件到标记
EXTERN_VAR uint8 T1000msFlag ;//100ms事件到标记

extern uint8 MCStack ;	//PLC翻译代码中用到的母线堆栈
extern uint8 CurStatus ;	//PLC翻译代码中的当前状态
extern uint8 WorkStack ;//PLC翻译代码中用到的工作区堆栈
extern uint8 MPStack ;//PLC翻译代码中用到的逻辑堆栈
extern uint8 TempValue ;//PLC翻译代码中用到的临时变量

EXTERN_VAR uint8 RunLedStatus; //运行指示灯状态，1=on，0=off

//关于端口操作代码，尚需考虑，请注意直接端口与PLC端口的区别
//将整个直接端口PLCPORT配置成输入

#define CfgInputPort(PLCPORT) GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;\
                              GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;\
                              GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;\
                              GPIO_Init(PLCPORT, &GPIO_InitStructure)
//将整个直接端口PLCPORT配置成输出     
#define CfgOutputPort(PLCPORT) GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;\
                              GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;\
                              GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;\
                              GPIO_Init(PLCPORT, &GPIO_InitStructure)


//将直接端口PLCPORT第PROTNUM个管脚配置成输入
#define CfgInputPortBit(PLCPORT,PORTNUM)    GPIO_InitStructure.GPIO_Pin = PORTNUM;\
                                          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;\
                                          GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;\
                                            GPIO_Init(PLCPORT, &GPIO_InitStructure) 
//将直接端口PLCPORT第PROTNUM个管脚配置成输出
#define CfgOutputPortBit(PLCPORT,PORTNUM) GPIO_InitStructure.GPIO_Pin = PORTNUM;\
                              GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;\
                              GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;\
                              GPIO_Init(PLCPORT, &GPIO_InitStructure)

//将直接CPU端口PLCPORT值读入到内存单元MemData中
#define ReadSingleDirectPort(MemData,PLCPORT)  MemData = GPIO_ReadInputData(PLCPORT)   
//将值MemData写入到直接端口PLCPORT中
#define WriteSingleDirectPort(MemData,PLCPORT) GPIO_SetBits(PLCPORT,MemData);\
                                         GPIO_ResetBits(PLCPORT,~MemData)

//将直接端口PLCPORT第PORTNUM管脚的值读入到MemData中
#define ReadSingleDirectPortBit(MemData,PLCPORT,PORTNUM) MemData = GPIO_ReadInputDataBit(PLCPORT,PORTNUM) 
//将值MemData写入到直接端口PLCPORT第PORTNUM管脚中
#define WriteSingleDirectPortBit(MemData,PLCPORT,PORTNUM) if(MemData != 0) \
							GPIO_SetBits(PLCPORT,PORTNUM);\
							else \
							GPIO_ResetBits(PLCPORT,PORTNUM);\

#if defined SMALLMODE            //此处填写小型引擎CASS端口操作代码 
//将CASS端口PLCPORT的值读入到MemData中  
#define ReadSinglePort(MemData,PLCPORT)  MemData = GPIO_ReadInputData(PLCPORT)   
//将值MemData写入到CASS端口PLCPORT中
#define WriteSinglePort(MemData,PLCPORT) GPIO_SetBits(PLCPORT,MemData);\
                                         GPIO_ResetBits(PLCPORT,~MemData)
              
#elif defined MIDDLEMODE         //此处填写小型引擎CASS端口操作代码 
#define ReadSinglePort(MemData,PLCPORT)    if(PLCPORT & 1) \
												PLCCODELINEA##0 |= (1 << PLCNUMCODELINEA); \
										   else \
												PLCCODELINEA##1 |= (1 << PLCNUMCODELINEA); \
										   if(PLCPORT & (1 << 1)) \
												PLCCODELINEB##0 |= (1 << PLCNUMCODELINEB); \
										   else \
												PLCCODELINEB##1 |= (1 << PLCNUMCODELINEB); \
											if(PLCPORT & (1 << 2)) \
												PLCCODELINEC##0 |= (1 << PLCNUMCODELINEC); \
										   else \
												PLCCODELINEC##1 |= (1 << PLCNUMCODELINEC); \
										   PLCPULSE1 |= (1 << PLCNUMPULSE); \
										   MemData = ~PLCINPUTPORT;	
										          
//将值MemData写入到PLC端口PLCPORT中
#define WriteSinglePort(MemData,PLCPORT)   	PLCOUTPUTPORT##0 = ~MemData; \
											PLCOUTPUTPORT##1 = MemData; \
										    if(PLCPORT & 1) \
												PLCCODELINEA##0 |= (1 << PLCNUMCODELINEA); \
										    else \
												PLCCODELINEA##1 |= (1 << PLCNUMCODELINEA); \
										    if(PLCPORT & (1 << 1)) \
												PLCCODELINEB##0 |= (1 << PLCNUMCODELINEB); \
										    else \
												PLCCODELINEB##1 |= (1 << PLCNUMCODELINEB); \
											if(PLCPORT & (1 << 2)) \
												PLCCODELINEC##0 |= (1 << PLCNUMCODELINEC); \
											else \
												PLCCODELINEC##1 |= (1 << PLCNUMCODELINEC); \
											PLCPULSE1 |= (1 << PLCNUMPULSE); \
											PLCPULSE1 |= (1 << PLCNUMPULSE); \
											PLCPULSE0 |= (1 << PLCNUMPULSE); 
											


#elif defined BIGMODE            //此处填写小型引擎CASS端口操作代码 
//将CASS端口PLCPORT的值读入到MemData中  
#define ReadSinglePort(MemData,PLCPORT)     
//将值MemData写入到CASS端口PLCPORT中
#define WriteSinglePort(MemData,PLCPORT)  
#endif

//IIC宏,需根据不同CPU进自定义行修改
#define IIC0Start()  I2CONCLR |= 0x1C;I2CONSET = 0x60//发出START信号
#define IIC0Stop()   I2CONCLR |= 0x1C;I2CONSET = 0x50//发出STOP信号
#define IIC0SndByt(x) I2DAT = x; I2CONCLR |= 0x20//发送一个字节的数据

//...依次列出每个IIC口宏定义和错误次数定义

//实时时钟芯片读写地址
#define PCF8563R 0xA3    //读8563的地址
#define PCF8563W 0xA2    //写8563的地址

//IIC状态码定义
#define TW_START		0x08
#define TW_REP_START		0x10
/* Master Transmitter */
#define TW_MT_SLA_ACK		0x18
#define TW_MT_SLA_NACK		0x20
#define TW_MT_DATA_ACK		0x28
#define TW_MT_DATA_NACK		0x30
#define TW_MT_ARB_LOST		0x38
/* Master Receiver */
#define TW_MR_ARB_LOST		0x38
#define TW_MR_SLA_ACK		0x40
#define TW_MR_SLA_NACK		0x48
#define TW_MR_DATA_ACK		0x50
#define TW_MR_DATA_NACK		0x58
/* Slave Transmitter */
#define TW_ST_SLA_ACK		0xA8
#define TW_ST_ARB_LOST_SLA_ACK	0xB0
#define TW_ST_DATA_ACK		0xB8
#define TW_ST_DATA_NACK		0xC0
#define TW_ST_LAST_DATA		0xC8
/* Slave Receiver */
#define TW_SR_SLA_ACK		0x60
#define TW_SR_ARB_LOST_SLA_ACK	0x68
#define TW_SR_GCALL_ACK		0x70
#define TW_SR_ARB_LOST_GCALL_ACK 0x78
#define TW_SR_DATA_ACK		0x80
#define TW_SR_DATA_NACK		0x88
#define TW_SR_GCALL_DATA_ACK	0x90
#define TW_SR_GCALL_DATA_NACK	0x98
#define TW_SR_STOP		0xA0
/* Misc */
#define TW_NO_INFO		0xF8
#define TW_BUS_ERROR		0x00

EXTERN_VAR uint8 PIDTempArray[8]; //PID指令翻译时需用到的临时存储区



EXTERN_VAR uint8 StepFlag;
EXTERN_VAR uint16 StepLen;    //步长
EXTERN_VAR uint8 FirstDebug;
EXTERN_VAR uint32 PointIndex[100]; //梯形图调试
EXTERN_VAR uint8 PointLen;
EXTERN_VAR uint32 AimIndex;
EXTERN_VAR uint32 CurIndex;
EXTERN_VAR uint8 StopDebug;
//EXTERN_VAR uint8 ComOutInfo[1000];
//EXTERN_VAR uint16 ComOutLen;
//EXTERN_VAR uint8 ComInInfo[100];
//EXTERN_VAR uint16 ComInLen;
EXTERN_VAR uint8 PlcFlag;//1快速，2慢速
EXTERN_VAR uint32 tempDebug;
EXTERN_VAR uint32 NumDebug;
EXTERN_VAR uint8 RSTFlag;//软复位标志

EXTERN_VAR pFunction FirstProgramSub ;
EXTERN_VAR pFunction SecondProgramSub ;
EXTERN_VAR pFunction Time1ms_Sub ;
EXTERN_VAR pFunction Time10ms_Sub ;
EXTERN_VAR pFunction Time100ms_Sub ;
EXTERN_VAR pFunction Time1000ms_Sub ;
EXTERN_VAR pFunction InitAddr ;
EXTERN_VAR pFunction FraimProcessEMS ;
extern uint32 FirstProgram;
extern uint32 SecondProgram;
extern uint32 Time1MS;
extern uint32 Time10MS;
extern uint32 Time100MS;
extern uint32 Time1000MS;
extern uint32 FraimProcessAddr;
extern uint32 SendInfoAddr;
extern uint32 WritePortAddr;
extern uint32 ComProAddr;
extern uint8 ComOutInfo[71];
extern uint8 ComInInfo[71];
extern uint8 ComInLen;
extern uint8 ComOutLen;
extern uint8 ModeFlag;

extern uint32 DCSProgram;
extern uint32 ADConvertAddr;
extern uint32 DAConvertAddr;
extern uint32 DCS_TimerCount;
extern uint32 ConfigurationInitAddr; 


EXTERN_VAR pFunction DCSProgramSub ;
EXTERN_VAR pFunction Configuration_init ;

#define M_T2_PWM
#define TIM3CLKHZ 72000000
#endif
