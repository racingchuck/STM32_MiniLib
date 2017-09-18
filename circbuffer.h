/****************************************

File name     : circbuffer.h
Author        : Charles Ratelle
Creation date : 07/04/2017

****************************************/

#ifndef __CIRCBUFFER_H
#define __CIRCBUFFER_H

//****************************************INCLUDE******************************************// 
#include <stm32f407xx.h>
#include "can_hal.h"

#ifdef __cplusplus
extern "C" {
#endif
//****************************************DEFINES******************************************// 
typedef struct
{
    can_message_s * const buffer;
	volatile int head;
	volatile int tail;
	const int maxLen;
} circBuf_t;

#define CIRCBUF_DEF(x,y)			\
    can_message_s x##_dataSpace[y];		\
    circBuf_t x = {					\
        x##_dataSpace,				\
        0,							\
        0,							\
        y							\
    }

typedef enum
{
	BUFFER_OK		= 0u,
	BUFFER_EMPTY	= 0x10u,
	BUFFER_FULL		= 0x11u,
} eBufferResult_t;
//****************************************Global Constants*********************************// 
//****************************************Global Variables*********************************// 
//****************************************Global Function Prototype************************// 
    eBufferResult_t circBufPush(circBuf_t *c, can_message_s data);
    eBufferResult_t circBufPop(circBuf_t *c, can_message_s *data);
    eBufferResult_t circBufEmpty(circBuf_t *c);

#ifdef __cplusplus
}
#endif
#endif // __CIRCBUFFER_H
