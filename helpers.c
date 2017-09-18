/****************************************

File name     : can_hal.c
Author        : Charles Ratelle
Creation date : 14/08/2017

****************************************/

//****************************************INCLUDE******************************************// 
#include "helpers.h"

#ifdef __cplusplus
extern "C" {
#endif

//****************************************DEFINES******************************************// 
//****************************************Global Constants*********************************// 
//****************************************Global Variables*********************************// 
//****************************************Function Prototypes******************************// 
//****************************************Local Function***********************************// 
//****************************************Global Function**********************************//
    void serialize_int_32(uint8_t buf[4], uint32_t val)
    {
        uint32_t uval = val;

        buf[0] = uval;
        buf[1] = uval >> 8;
        buf[2] = uval >> 16;
        buf[3] = uval >> 24;
    }
//****************************************Interrupts Handler*******************************// 


#ifdef __cplusplus
}
#endif
