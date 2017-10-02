/****************************************

File name     : minilibUART.h
Author        : Charles Ratelle
Creation date : 05/04/2017

****************************************/

#ifndef __MINILIBUART_H
#define __MINILIBUART_H
//****************************************INCLUDE******************************************// 
#include <stm32f4xx_hal.h>
#include <stdbool.h>
#include "minilibHelpers.h"

#ifdef __cplusplus
extern "C" {
#endif
//****************************************DEFINES******************************************//
#define HUART2
#define RX_BUFF_SIZE 32
// UART Configuration table
//				    Name            Tx Port         Tx Pin          Rx Port         Rx Pin          Alt func            Instance
#define X_UART_CROSSTABLE 	\
		X_XTABLE(   USART1_1,       GPIOA,          GPIO_PIN_9,     GPIOA,          GPIO_PIN_10,    GPIO_AF7_USART1,    USART1) \
        X_XTABLE(   USART1_2,       GPIOB,          GPIO_PIN_6,     GPIOA,          GPIO_PIN_7,     GPIO_AF7_USART1,    USART1) \
        X_XTABLE(   USART2_1,       GPIOA,          GPIO_PIN_2,     GPIOA,          GPIO_PIN_3,     GPIO_AF7_USART2,    USART2) \
        X_XTABLE(   USART2_2,       GPIOA,          GPIO_PIN_2,     GPIOD,          GPIO_PIN_6,     GPIO_AF7_USART2,    USART2) \
        X_XTABLE(   USART3_1,       GPIOB,          GPIO_PIN_10,    GPIOA,          GPIO_PIN_11,    GPIO_AF7_USART3,    USART3) \
        X_XTABLE(   USART3_2,       GPIOD,          GPIO_PIN_8,     GPIOD,          GPIO_PIN_9,     GPIO_AF7_USART3,    USART3) \
        X_XTABLE(   USART3_3,       GPIOC,          GPIO_PIN_10,    GPIOC,          GPIO_PIN_11,    GPIO_AF7_USART3,    USART3) \
        X_XTABLE(   USART6_1,       GPIOC,          GPIO_PIN_6,     GPIOC,          GPIO_PIN_7,	    GPIO_AF8_USART6,    USART6) \

    typedef enum uart_periph_names_e
    {
#define X_XTABLE(NAME, TX_PORT, TX_PIN, RX_PORT, RX_PIN, AF, INSTANCE) NAME,
        X_UART_CROSSTABLE							
#undef X_XTABLE
                NB_OF_UART_PERIPH_ENTRY,
    } uart_periph_names_e;

    typedef struct
    {
        uart_periph_names_e Name;
        GPIO_TypeDef* TX_Port;
        uint16_t TX_Pin;
        GPIO_TypeDef* RX_Port;
        uint16_t RX_Pin;
        uint8_t Alt_Func;
        USART_TypeDef* Instance;
    } uart_periph;

    //****************************************Global Constants*********************************// 
    extern const uart_periph UART_PERIPHS[NB_OF_UART_PERIPH_ENTRY];
    //****************************************Global Variables*********************************// 

//****************************************Global Function Prototype************************// 
    void uart_init(uart_periph uart, UART_HandleTypeDef* uart_handle);
    void uart_set_receive_interrupt(uart_periph uart, UART_HandleTypeDef* uart_handle, uint8_t* receive_buffer);
    void uart_send(UART_HandleTypeDef* uart_handle, uint8_t data[], uint8_t size);


#ifdef __cplusplus
}
#endif
#endif // __MINILIBUART_H
