/****************************************

File name     : circbuffer.c
Author        : Charles Ratelle
Creation date : 07/04/2017

****************************************/

//****************************************INCLUDE******************************************// 
#include "minilibCircBuffer.h"

#ifdef __cplusplus
extern "C" {
#endif

//****************************************DEFINES******************************************//

//****************************************Global Constants*********************************// 

//****************************************Global Variables*********************************// 

//****************************************Function Prototypes******************************// 

//****************************************Local Function***********************************// 

//****************************************Global Function**********************************// 
    void CB_CAN_Init(struct sCircularBufferCAN *cb, uint16_t size, can_message_s buffer[]) {
        cb->size = size;
        cb->write = 0; 
        cb->read = 0; 
        cb->buf = buffer;
    }

    void CB_Byte_Init(struct sCircularBufferUINT8 *cb, uint16_t size, uint8_t buffer[]) {
        cb->size = size;
        cb->write = 0; 
        cb->read = 0; 
        cb->buf = buffer;
    }

    uint16_t CBLengthDataCAN(struct sCircularBufferCAN *cb)
    {
        return ((cb->write - cb->read) & (cb->size - 1));
    }

    uint16_t CBLengthDataUINT8(struct sCircularBufferUINT8 *cb)
    {
        return ((cb->write - cb->read) & (cb->size - 1));
    }

    eBufferResult_t CB_CAN_Write(struct sCircularBufferCAN *cb, can_message_s data)
    {
        if (CBLengthDataCAN(cb) == (cb->size - 1)) { return BUFFER_FULL; }
        cb->buf[cb->write] = data;
        cb->write = (cb->write + 1) & (cb->size - 1);
        return BUFFER_OK;
    }

    eBufferResult_t CB_UINT8_Write(sCircularBufferUINT8 *cb, uint8_t data)
    {
        if (CBLengthDataUINT8(cb) == (cb->size - 1)) { return BUFFER_FULL; }
        cb->buf[cb->write] = data;
        cb->write = (cb->write + 1) & (cb->size - 1);
        return BUFFER_OK;
    }

    eBufferResult_t CB_CAN_Read(struct sCircularBufferCAN *cb, can_message_s *data)
    {
        if (CBLengthDataCAN(cb) == 0) { return BUFFER_EMPTY; }
        *data = cb->buf[cb->read];
        cb->read = (cb->read + 1)& (cb->size - 1);
        return BUFFER_OK;
    }

    eBufferResult_t CB_UINT8_Read(struct sCircularBufferUINT8 *cb, uint8_t *data)
    {
        if (CBLengthDataUINT8(cb) == 0) { return BUFFER_EMPTY; }
        *data = cb->buf[cb->read];
        cb->read = (cb->read + 1)& (cb->size - 1);
        return BUFFER_OK;
    }
//****************************************Interrupts Handler*******************************// 

#ifdef __cplusplus
}
#endif