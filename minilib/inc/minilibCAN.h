/****************************************

File name     : minilibCAN.h
Author        : Charles Ratelle
Creation date : 07/08/2017

****************************************/
#ifndef __MINILIBCAN_H
#define __MINILIBCAN_H

#ifdef __cplusplus
extern "C" {
#endif
//****************************************INCLUDE******************************************// 
#include <stm32f4xx_hal.h>
//****************************************DEFINES******************************************//

struct sCircularBufferCAN;
// CAN RX Buffer size
#define CAN_RX_BUFF_SIZE 32
#define CAN_TX_BUFF_SIZE 16

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
		X_XTABLE(   CAN1_1,       GPIOB,    GPIO_PIN_9,     GPIOB,    GPIO_PIN_8,     GPIO_AF9_CAN1, CAN1) \
        X_XTABLE(   CAN1_2,       GPIOA,    GPIO_PIN_12,    GPIOA,    GPIO_PIN_11,    GPIO_AF9_CAN1, CAN1) \
        X_XTABLE(   CAN1_3,       GPIOD,    GPIO_PIN_1,     GPIOD,    GPIO_PIN_0,     GPIO_AF9_CAN1, CAN1) \
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

    typedef struct can_message_s
    {
        uint32_t ID;
        uint32_t EXTID;
        uint8_t DLC;
        uint8_t Data[8];
        uint8_t IDType;
    } can_message_s;

//****************************************Global Constants*********************************// 
    extern const can_periph CAN_PERIPHS[NB_OF_CAN_PERIPH_ENTRY];
//****************************************Global Variables*********************************// 
    CAN_HandleTypeDef hcan1;
    CAN_HandleTypeDef hcan2;
//****************************************Global Function Prototype************************// 
    void minilib_CAN_Init(can_periph can);
    void minilib_SetFilterId(CAN_HandleTypeDef* can_handle, uint16_t id);
    void minilib_CAN_SetFilterMask(CAN_HandleTypeDef* can_handle, uint32_t filter_mask, uint32_t filter_id);
    void minilib_CAN_TxMsg(CAN_HandleTypeDef* can_handle, struct sCircularBufferCAN *buffer);
    void minilib_CAN_SetBaudrate(CAN_HandleTypeDef* can_handle, uint32_t baudrate);

#ifdef __cplusplus
}
#endif
#endif // __MINILIBCAN_H
