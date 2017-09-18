/****************************************

File name     : circbuffer.c
Author        : Charles Ratelle
Creation date : 07/04/2017

****************************************/

//****************************************INCLUDE******************************************// 
#include "circbuffer.h"

#ifdef __cplusplus
extern "C" {
#endif

//****************************************DEFINES******************************************// 

//****************************************Global Constants*********************************// 

//****************************************Global Variables*********************************// 

//****************************************Function Prototypes******************************// 

//****************************************Local Function***********************************// 

//****************************************Global Function**********************************// 
    eBufferResult_t circBufPush(circBuf_t *c, can_message_s data)
{
	// Position head pointer
	int next = c->head + 1;
	if (next >= c->maxLen)
	{
		next = 0;
	}

	// Check if buffer is full
	if (next == c->tail)
	{
		return BUFFER_FULL;
	}

	c->buffer[c->head] = data;
	c->head = next;

	return BUFFER_OK;

}

    eBufferResult_t circBufPop(circBuf_t *c, can_message_s *data)
{
    	// Check if buffer is empty
    if (c->head == c->tail)
    {
        return BUFFER_EMPTY;
    }
	// Update next pointer
	int next = c->tail + 1;
	if (next >= c->maxLen)
		next = 0;

	*data = c->buffer[c->tail]; // Read data
	c->tail = next;				// Update tail
	return BUFFER_OK;
}

    eBufferResult_t circBufEmpty(circBuf_t *c)
    {
        	// Check if buffer is empty
        if (c->head == c->tail)
        {
            return BUFFER_EMPTY;
        }
    }
//****************************************Interrupts Handler*******************************// 

#ifdef __cplusplus
}
#endif