/****************************************

File name     : circbuffer.h
Author        : Charles Ratelle
Creation date : 14/07/2017

****************************************/

#ifndef __HELPERS_H
#define __HELPERS_H

//****************************************INCLUDE******************************************// 
#include <stm32f4xx_hal.h>

#ifdef __cplusplus
extern "C" {
#endif
    //****************************************DEFINES******************************************// 
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0])) // Array size define
    //****************************************Global Constants*********************************// 
    //****************************************Global Variables*********************************// 
    //****************************************Global Function Prototype************************//
    void serialize_int_32(uint8_t buf[4], uint32_t val);

#ifdef __cplusplus
}
#endif
#endif // __HELPERS_H

