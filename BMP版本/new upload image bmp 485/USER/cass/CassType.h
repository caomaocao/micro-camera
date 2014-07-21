////////////////////////////////////////////////////////////////////////////////////////////
// 文   件   名  : CassSysVar.h
// 版        本  :                            
// 目的及主要功能: 存放系统变量类型的声明，目的是为了引擎数据类型无关性
// 创 建 日  期  : 2008.3.5
// 修 改 日  期  : 2008.3.5                    
// 作        者  : hfl
// 修   改   者  :                      
////////////////////////////////////////////////////////////////////////////////////////////

#ifndef CASSTYPE_H
#define CASSTYPE_H

typedef unsigned char uint8;
typedef unsigned int uint16;
typedef unsigned long uint32;
typedef signed char int8;
typedef signed int int16;
typedef signed long int32;
typedef float fp32;
typedef double fp64;
typedef uint8 (*pFunction)(void) ;
typedef uint8 (*pFun)(uint32) ;
typedef uint8 (*ppFun)(uint8,uint32) ;
typedef uint8 (*pppFun)(uint32*) ;

#endif
