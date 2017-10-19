/****************************************

File name     : minilibTimer.h
Author        : cratelle
Creation date : 9/28/2017 11:45:02 AM

****************************************/

#ifndef __minilibTimer_H
#define __minilibTimer_H


//****************************************INCLUDE******************************************// 
#include "stm32f4xx_hal.h"
//****************************************DEFINES******************************************// 
// Timer Configuration table
//				    Name            Tx Port         Tx Pin          Rx Port         Rx Pin          Alt func            Instance
#define X_TIM_CROSSTABLE 	\
        X_XTABLE( TIM1_1,    TIM1,   GPIOA,  GPIO_PIN_8  ) \
        X_XTABLE( TIM1_2,    TIM1,   GPIOE,  GPIO_PIN_9 ) \
        X_XTABLE( TIM1_3,    TIM1,   GPIOA,  GPIO_PIN_9  ) \
        X_XTABLE( TIM1_4,    TIM1,   GPIOE,  GPIO_PIN_10 ) \
        X_XTABLE( TIM1_5,    TIM1,   GPIOA,  GPIO_PIN_10 ) \
        X_XTABLE( TIM1_6,    TIM1,   GPIOE,  GPIO_PIN_13 ) \
        X_XTABLE( TIM1_7,    TIM1,   GPIOA,  GPIO_PIN_11 ) \
        X_XTABLE( TIM1_8,    TIM1,   GPIOE,  GPIO_PIN_14 ) \
        X_XTABLE( TIM2_1,    TIM2,   GPIOA,  GPIO_PIN_0  ) \
        X_XTABLE( TIM2_2,    TIM2,   GPIOA,  GPIO_PIN_5  ) \
        X_XTABLE( TIM2_3,    TIM2,   GPIOA,  GPIO_PIN_15 ) \
        X_XTABLE( TIM2_4,    TIM2,   GPIOA,  GPIO_PIN_1 ) \
        X_XTABLE( TIM2_5,    TIM2,   GPIOB,  GPIO_PIN_3  ) \
        X_XTABLE( TIM2_6,    TIM2,   GPIOA,  GPIO_PIN_2  ) \
        X_XTABLE( TIM2_7,    TIM2,   GPIOB,  GPIO_PIN_10 ) \
        X_XTABLE( TIM2_8,    TIM2,   GPIOA,  GPIO_PIN_3 ) \
        X_XTABLE( TIM2_9,    TIM2,   GPIOB,  GPIO_PIN_11 ) \
        X_XTABLE( TIM3_1,    TIM3,   GPIOA,  GPIO_PIN_6  ) \
        X_XTABLE( TIM3_2,    TIM3,   GPIOB,  GPIO_PIN_4  ) \
        X_XTABLE( TIM3_3,    TIM3,   GPIOC,  GPIO_PIN_6  ) \
        X_XTABLE( TIM3_4,    TIM3,   GPIOA,  GPIO_PIN_7  ) \
        X_XTABLE( TIM3_5,    TIM3,   GPIOB,  GPIO_PIN_5  ) \
        X_XTABLE( TIM3_6,    TIM3,   GPIOC,  GPIO_PIN_7  ) \
        X_XTABLE( TIM3_7,    TIM3,   GPIOB,  GPIO_PIN_0  ) \
        X_XTABLE( TIM3_8,    TIM3,   GPIOC,  GPIO_PIN_8  ) \
        X_XTABLE( TIM3_9,    TIM3,   GPIOB,  GPIO_PIN_1  ) \
        X_XTABLE( TIM3_10,   TIM3,   GPIOC,  GPIO_PIN_9  ) \
        X_XTABLE( TIM4_1,    TIM4,   GPIOB,  GPIO_PIN_6  ) \
        X_XTABLE( TIM4_2,    TIM4,   GPIOD,  GPIO_PIN_12 ) \
        X_XTABLE( TIM4_3,    TIM4,   GPIOB,  GPIO_PIN_7  ) \
        X_XTABLE( TIM4_4,    TIM4,   GPIOD,  GPIO_PIN_13 ) \
        X_XTABLE( TIM4_5,    TIM4,   GPIOB,  GPIO_PIN_8  ) \
        X_XTABLE( TIM4_6,    TIM4,   GPIOD,  GPIO_PIN_14 ) \
        X_XTABLE( TIM4_7,    TIM4,   GPIOB,  GPIO_PIN_9  ) \
        X_XTABLE( TIM4_8,    TIM4,   GPIOD,  GPIO_PIN_15 ) \
        X_XTABLE( TIM5_1,    TIM5,   GPIOA,  GPIO_PIN_0 ) \
        X_XTABLE( TIM5_2,    TIM5,   GPIOH,  GPIO_PIN_10 ) \
        X_XTABLE( TIM5_3,    TIM5,   GPIOA,  GPIO_PIN_1 ) \
        X_XTABLE( TIM5_4,    TIM5,   GPIOH,  GPIO_PIN_11 ) \
        X_XTABLE( TIM5_5,    TIM5,   GPIOA,  GPIO_PIN_2 ) \
        X_XTABLE( TIM5_6,    TIM5,   GPIOH,  GPIO_PIN_12 ) \
        X_XTABLE( TIM5_7,    TIM5,   GPIOA,  GPIO_PIN_3  ) \
        X_XTABLE( TIM5_8,    TIM5,   GPIOI,  GPIO_PIN_0  ) \
        X_XTABLE( TIM8_1,    TIM8,   GPIOC,  GPIO_PIN_6  ) \
        X_XTABLE( TIM8_2,    TIM8,   GPIOI,  GPIO_PIN_5  ) \
        X_XTABLE( TIM8_3,    TIM8,   GPIOC,  GPIO_PIN_7  ) \
        X_XTABLE( TIM8_4,    TIM8,   GPIOI,  GPIO_PIN_6  ) \
        X_XTABLE( TIM8_5,    TIM8,   GPIOC,  GPIO_PIN_8  ) \
        X_XTABLE( TIM8_6,    TIM8,   GPIOI,  GPIO_PIN_7  ) \
        X_XTABLE( TIM8_7,    TIM8,   GPIOC,  GPIO_PIN_9  ) \
        X_XTABLE( TIM8_8,    TIM8,   GPIOI,  GPIO_PIN_2  ) \
        X_XTABLE( TIM9_1,    TIM9,   GPIOA,  GPIO_PIN_2  ) \
        X_XTABLE( TIM9_2,    TIM9,   GPIOE,  GPIO_PIN_5  ) \
        X_XTABLE( TIM9_3,    TIM9,   GPIOA,  GPIO_PIN_3  ) \
        X_XTABLE( TIM9_4,    TIM9,   GPIOE,  GPIO_PIN_6  ) \
        X_XTABLE( TIM10_1,   TIM10,  GPIOB,  GPIO_PIN_8  ) \
        X_XTABLE( TIM10_2,   TIM10,  GPIOF,  GPIO_PIN_6  ) \
        X_XTABLE( TIM11_1,   TIM11,  GPIOB,  GPIO_PIN_9  ) \
        X_XTABLE( TIM11_2,   TIM11,  GPIOF,  GPIO_PIN_7  ) \
        X_XTABLE( TIM12_1,   TIM12,  GPIOB,  GPIO_PIN_14 ) \
        X_XTABLE( TIM12_2,   TIM12,  GPIOH,  GPIO_PIN_6  ) \
        X_XTABLE( TIM12_3,   TIM12,  GPIOB,  GPIO_PIN_15 ) \
        X_XTABLE( TIM12_4,   TIM12,  GPIOH,  GPIO_PIN_9 ) \
        X_XTABLE( TIM13_1,   TIM13,  GPIOA,  GPIO_PIN_6  ) \
        X_XTABLE( TIM13_2,   TIM13,  GPIOF,  GPIO_PIN_8  ) \
        X_XTABLE( TIM14_1,   TIM14,  GPIOA,  GPIO_PIN_7  ) \
        X_XTABLE( TIM14_2,   TIM14,  GPIOF,  GPIO_PIN_9  ) \

typedef enum tim_periph_names_e
{
#define X_XTABLE(NAME, TIMER, PORT, PIN) NAME,
    X_TIM_CROSSTABLE							
#undef X_XTABLE
                    NB_OF_TIM_PERIPH_ENTRY,
} tim_periph_names_e;

typedef struct
{
    TIM_HandleTypeDef htim;
    uint32_t Frequency;
    uint32_t MaxPeriod;
    uint32_t MaxPrescaler;
    uint32_t Period;
    uint32_t Prescaler;
    uint32_t TimerClockFreq;
    uint32_t Micros;
    uint32_t MaxMicros;
} minilibTimer_props_s;

typedef enum
{
    TIM_OK = 0,
    TIM_FREQ_TOO_HIGH,
    TIM_FREQ_TOO_LOW,
    TIM_ERROR
} tim_result_e;

typedef enum
{
    mlTIM_Channel_1   = TIM_CHANNEL_1,
    mlTIM_Channel_2   = TIM_CHANNEL_2,  
    mlTIM_Channel_3   = TIM_CHANNEL_3,  
    mlTIM_Channel_4   = TIM_CHANNEL_4,  
    mlTIM_Channel_ALL = TIM_CHANNEL_ALL
}mlTIM_Channel_e;
//****************************************Global Constants*********************************// 

//****************************************Global Variables*********************************// 

//****************************************Global Function Prototype************************// 
void minilibTimer_init(minilibTimer_props_s* props);
void minilibPWMTimer_init(minilibTimer_props_s* props, TIM_TypeDef* instance, double frequency);
void minilibPWMConfig_Channel(minilibTimer_props_s* props, mlTIM_Channel_e channel, GPIO_TypeDef* port, uint16_t pin);
tim_result_e minilibPWM_set_pulse(minilibTimer_props_s* props, mlTIM_Channel_e channel, uint32_t pulse);
tim_result_e minilibPWM_setMicros(minilibTimer_props_s* props, uint8_t channel, uint32_t micros);
tim_result_e minilibPWM_setPercent(minilibTimer_props_s* props, uint8_t channel, uint32_t percent);
void minilibPWM_IC_init(minilibTimer_props_s* props, TIM_TypeDef* instance, double frequency);
void minilibPWM_IC_ConfigChannel(minilibTimer_props_s* props, mlTIM_Channel_e channel, GPIO_TypeDef* port, uint16_t pin);
#endif // __minilibTimer_H