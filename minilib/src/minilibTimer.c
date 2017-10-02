/****************************************

File name     : minilibTimer.c
Author        : cratelle
Creation date : 9/28/2017 11:49:12 AM

****************************************/

//****************************************INCLUDE******************************************// 
#include "minilibTimer.h"

//****************************************DEFINES******************************************// 

//****************************************Global Constants*********************************// 

//****************************************Global Variables*********************************// 

//****************************************Function Prototypes******************************// 
void minilibTimer_set_clock(TIM_HandleTypeDef* htim);
//****************************************Local Function***********************************// 
void minilibTimer_set_clock(TIM_HandleTypeDef* htim)
{
    if (htim->Instance == TIM1)
    {
        __TIM1_CLK_ENABLE();
    }
    else if (htim->Instance == TIM2)
    {
        __TIM2_CLK_ENABLE();
    }
    else if (htim->Instance == TIM3)
    {
        __TIM3_CLK_ENABLE();
    }
    else if (htim->Instance == TIM4)
    {
        __TIM4_CLK_ENABLE();
    }
    else if (htim->Instance == TIM5)
    {
        __TIM5_CLK_ENABLE();
    }
    else if (htim->Instance == TIM6)
    {
        __TIM6_CLK_ENABLE();
    }
    else if (htim->Instance == TIM7)
    {
        __TIM7_CLK_ENABLE();
    }
    else if (htim->Instance == TIM8)
    {
        __TIM8_CLK_ENABLE();
    }
    else if (htim->Instance == TIM9)
    {
        __TIM9_CLK_ENABLE();
    }
    else if (htim->Instance == TIM10)
    {
        __TIM10_CLK_ENABLE();
    }
    else if (htim->Instance == TIM11)
    {
        __TIM11_CLK_ENABLE();
    }
    else if (htim->Instance == TIM12)
    {
        __TIM12_CLK_ENABLE();
    }
    else if (htim->Instance == TIM13)
    {
        __TIM13_CLK_ENABLE();
    }
    else if (htim->Instance == TIM14)
    {
        __TIM14_CLK_ENABLE();
    }
}

int minilibTimer_calc_prescaler(minilibTimer_props_s *props, double frequency)
{
    if (frequency > props->TimerClockFreq)
    {
        props->Prescaler = 0;
        props->Period = 0;
        props->Frequency = 0;
        
        return 2;
    }
    else if (frequency == 0)
    {
        props->Prescaler = 0;
        props->Period = 0;
        props->Frequency = 0;
        return 1;
    }

    	/* Fix for 16/32bit timers */
    if (props->MaxPeriod <= 0xFFFF) {
        props->MaxPeriod++;
    }

    props->Prescaler = 0;
    do
    {
        props->Period = (props->TimerClockFreq / (props->Prescaler + 1));
        props->Period = (props->Period / frequency);
        props->Prescaler++;
    } while (props->Period > (props->MaxPeriod) && props->Prescaler <= props->MaxPrescaler + 1);

    if (props->Prescaler > (props->MaxPrescaler + 1))
    {
        props->Prescaler = 0;
        props->Period = 0;
        props->Frequency = 0;

        return 1;
    }

    props->Frequency = frequency;
    return 0;
}

int minilibTimer_getTimerInfo(TIM_HandleTypeDef* htim, minilibTimer_props_s* props)
{
    props->MaxPrescaler = 0xFFFF;

    if (htim->Instance == TIM2 || htim->Instance == TIM5)
    {
        props->TimerClockFreq = HAL_RCC_GetPCLK1Freq();
        props->MaxPeriod = 0xFFFFFFFF;
        return 0;
    }
    else if (htim->Instance == TIM1  || 
             htim->Instance == TIM8  ||
             htim->Instance == TIM9  ||
             htim->Instance == TIM10 ||
             htim->Instance == TIM11)
    {
        props->TimerClockFreq = HAL_RCC_GetHCLKFreq();
        props->MaxPeriod = 0xFFFF;
        return 0;
    }
    else if (htim->Instance == TIM3  ||
             htim->Instance == TIM4  ||
             htim->Instance == TIM6  ||
             htim->Instance == TIM7  ||
             htim->Instance == TIM12 ||
             htim->Instance == TIM13 ||
             htim->Instance == TIM14)
    {
        props->TimerClockFreq = HAL_RCC_GetPCLK2Freq();
        props->MaxPeriod = 0xFFFF;
        return 0;
    }

    return 1;
}


//****************************************Global Function**********************************// 
void minilibTimer_init(TIM_HandleTypeDef* htim, minilibTimer_props_s* props)
{
    
    minilibTimer_set_clock(htim);
    minilibTimer_getTimerInfo(htim, props);
    minilibTimer_calc_prescaler(props,10000);

    htim->Init.Prescaler = props->Prescaler-1;
    htim->Init.CounterMode = TIM_COUNTERMODE_UP;
    htim->Init.Period = props->Period - 1;
    htim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim->Init.RepetitionCounter = 0;

    HAL_TIM_Base_Init(htim);
    HAL_TIM_Base_Start(htim);
}

void minilibPWMTimer_init(TIM_HandleTypeDef* htim)
{
    TIM_OC_InitTypeDef pwmChannelInit;
    minilibTimer_props_s props;

    minilibTimer_init(htim,&props);

    pwmChannelInit.OCMode = TIM_OCMODE_PWM1;
    pwmChannelInit.Pulse = 100;
    pwmChannelInit.OCPolarity = TIM_OCPOLARITY_HIGH;
    pwmChannelInit.OCFastMode = TIM_OCFAST_DISABLE;
    
    HAL_TIM_PWM_ConfigChannel(htim, &pwmChannelInit, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(htim, TIM_CHANNEL_1);

}
//****************************************Interrupts Handler*******************************// 

