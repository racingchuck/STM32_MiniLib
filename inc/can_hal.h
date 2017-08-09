/****************************************

File name     : can_hal.h
Author        : Charles Ratelle
Creation date : 07/08/2017

****************************************/

#ifndef __CAN_HAL_H
#define __CAN_HAL_H

#ifdef __cplusplus
extern "C" {
#endif
//****************************************INCLUDE******************************************// 
#include <stm32f4xx_hal.h>
//****************************************DEFINES******************************************//
// CAN Baudrate Table
// Configured for a APB1 Clock of 42 Mhz
#define CAN_1000KBPS 2
#define CAN_500KBPS 4
#define CAN_250KBPS 8
#define CAN_125KBPS 16
#define CAN_62KBPS 32

// CAN Configuration table
//				    Name          Tx Port   Tx Pin          Rx Port   Rx Pin          Alt func       Instance
#define X_CAN_CROSSTABLE 	\
        X_XTABLE(   CAN1_2,       GPIOA,    GPIO_PIN_12,    GPIOA,    GPIO_PIN_11,    GPIO_AF9_CAN1, CAN1) \
        X_XTABLE(   CAN1_3,       GPIOD,    GPIO_PIN_1,     GPIOD,    GPIO_PIN_0,     GPIO_AF9_CAN1, CAN1) \
		X_XTABLE(   CAN1_1,       GPIOB,    GPIO_PIN_9,     GPIOB,    GPIO_PIN_8,     GPIO_AF9_CAN1, CAN1) \
        X_XTABLE(   CAN2_1,       GPIOB,    GPIO_PIN_5,     GPIOB,    GPIO_PIN_6,     GPIO_AF9_CAN2, CAN2) \
        X_XTABLE(   CAN2_2,       GPIOB,    GPIO_PIN_13,    GPIOB,    GPIO_PIN_12,    GPIO_AF9_CAN2, CAN2) \

    typedef enum can_periph_names_e
    {
#define X_XTABLE(NAME, TX_PORT, TX_PIN, RX_PORT, RX_PIN, AF, INSTANCE) NAME,
        X_CAN_CROSSTABLE							
#undef X_XTABLE
                NB_OF_CAN_PERIPH_ENTRY,
    } can_periph_names_e;

    typedef struct
    {
        can_periph_names_e Name;
        GPIO_TypeDef* TX_Port;
        uint16_t TX_Pin;
        GPIO_TypeDef* RX_Port;
        uint16_t RX_Pin;
        uint8_t Alt_Func;
        CAN_TypeDef* Instance;
    } can_periph;
//****************************************Global Constants*********************************// 
    extern const can_periph CAN_PERIPHS[NB_OF_CAN_PERIPH_ENTRY];
//****************************************Global Variables*********************************// 

//****************************************Global Function Prototype************************// 
    void can_hal_init(can_periph can, CAN_HandleTypeDef* can_handle);
    void can_hal_set_filter_id(CAN_HandleTypeDef* can_handle, uint16_t id);
    void can_hal_set_filter_mask(CAN_HandleTypeDef* can_handle, uint32_t filter_mask, uint32_t filter_id);
    void can_hal_tx_msg(CAN_HandleTypeDef* can_handle, uint32_t id, uint8_t data[], uint8_t dlc);
    void can_hal_set_baudrate(CAN_HandleTypeDef* can_handle, uint32_t baudrate);

#ifdef __cplusplus
}
#endif

#endif // __CAN_HAL_H
