#ifndef _CONFIGURATION_STACK_H
#define _CONFIGURATION_STACK_H

#include "CassType.h"

#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10


// 栈的结构体
typedef struct{
	fp32* base;
	fp32* top;
	uint32 stackSize;
}stack;

int8 InitStack(stack* s);
int8 GetTop(stack s,fp32* e);
int8 Push(stack* s,fp32 e);
int8 Pop(stack* s,fp32 *e);
uint8 ClearStack(stack* s);
#endif
