/****************************************

File name     : circbuffer.h
Author        : Charles Ratelle
Creation date : 07/04/2017

****************************************/

#ifndef __CIRCBUFFER_H
#define __CIRCBUFFER_H

//****************************************INCLUDE******************************************// 
#include <stm32f4xx_hal.h>
#include "can_hal.h"
#ifdef __cplusplus
extern "C" {
#endif
//****************************************DEFINES******************************************// 
    typedef struct sCircularBufferCAN
    {
        struct can_message_s *buf;
        uint16_t size;
        uint16_t read;
        uint16_t write;
    } sCircularBuffer;

    typedef struct sCircularBufferUINT8
    {
        uint8_t *buf;
        uint16_t size;
        uint16_t read;
        uint16_t write;
    } sCircularBufferUINT8;

    typedef enum
    {
        BUFFER_OK    = 0u,
        BUFFER_EMPTY = 0x10u,
        BUFFER_FULL  = 0x11u,
    } eBufferResult_t;
    //****************************************Global Constants*********************************// 
    //****************************************Global Variables*********************************// 
    //****************************************Global Function Prototype************************// 
    void CB_CAN_Init(struct sCircularBufferCAN *cb, uint16_t size, struct can_message_s buffer[]);

    uint16_t CBLengthDataCAN(struct sCircularBufferCAN *cb);
    eBufferResult_t CB_CAN_Write(struct sCircularBufferCAN *cb, struct can_message_s data);
    eBufferResult_t CB_CAN_Read(struct sCircularBufferCAN *cb, struct can_message_s *data);

    uint16_t CBLengthDataUINT8(struct sCircularBufferUINT8 *cb);
    eBufferResult_t CB_UINT8_Write(struct sCircularBufferUINT8 *cb, uint8_t data);
    eBufferResult_t CB_UINT8_Read(struct sCircularBufferUINT8 *cb, uint8_t *data);

#ifdef __cplusplus
}
#endif
#endif // __CIRCBUFFER_H
