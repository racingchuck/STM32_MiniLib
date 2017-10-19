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
__IO uint32_t tim1_pwm_ic_value[4];
__IO uint32_t tim2_pwm_ic_value[4];
__IO uint32_t tim3_pwm_ic_value[4];
__IO uint32_t tim4_pwm_ic_value[4];
__IO uint32_t tim5_pwm_ic_value[4];
__IO uint32_t tim8_pwm_ic_value[4];
__IO uint32_t tim9_pwm_ic_value[2];
__IO uint32_t tim10_pwm_ic_value;
__IO uint32_t tim11_pwm_ic_value;
__IO uint32_t tim12_pwm_ic_value[2];
__IO uint32_t tim13_pwm_ic_value;
__IO uint32_t tim14_pwm_ic_value;

//****************************************Function Prototypes******************************// 
void minilibTimer_set_clock(TIM_HandleTypeDef* htim);
tim_result_e minilibPWM_Pin_Init(TIM_TypeDef* instance, GPIO_TypeDef* port, uint16_t pin);
tim_result_e minilibIC_Pin_Init(TIM_TypeDef* instance, GPIO_TypeDef* port, uint16_t pin);
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

tim_result_e minilibTimer_calc_prescaler(minilibTimer_props_s *props)
{
    double frequency = props->Frequency;
    if (frequency > props->TimerClockFreq)
    {
        props->Prescaler = 0;
        props->Period = 0;
        props->Frequency = 0;
        
        return TIM_FREQ_TOO_HIGH;
    }
    else if (frequency == 0)
    {
        props->Prescaler = 0;
        props->Period = 0;
        props->Frequency = 0;
        return TIM_FREQ_TOO_LOW;
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

        return TIM_FREQ_TOO_LOW;
    }

    props->Frequency = frequency;
    props->Micros = 1000000 / frequency;
    props->MaxMicros = 0xFFFFFFFF / props->MaxPeriod;
    return TIM_OK;
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
void minilibTimer_init(minilibTimer_props_s* props)
{
    TIM_HandleTypeDef* htim = &props->htim;
    // Enable Timer Clock
    minilibTimer_set_clock(htim);

    minilibTimer_getTimerInfo(htim, props);

    if (minilibTimer_calc_prescaler(props) != TIM_OK)
    {
        while (1)
            ;
    }
    ;

    htim->Init.Prescaler = props->Prescaler - 1;
    htim->Init.CounterMode = TIM_COUNTERMODE_UP;
    htim->Init.Period = props->Period - 1;
    htim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim->Init.RepetitionCounter = 0;

    // Initialize timer
    HAL_TIM_Base_Init(htim);

    // Start timer
    HAL_TIM_Base_Start(htim);
}

void minilibTimer_IC_init(minilibTimer_props_s* props)
{
    TIM_HandleTypeDef* htim = &props->htim;
    // Enable Timer Clock
    minilibTimer_set_clock(htim);

    minilibTimer_getTimerInfo(htim, props);

    if (minilibTimer_calc_prescaler(props) != TIM_OK)
    {
        while (1)
            ;
    }
    ;

    htim->Init.Prescaler = props->Prescaler - 1;
    htim->Init.CounterMode = TIM_COUNTERMODE_UP;
    htim->Init.Period = props->Period - 1;
    htim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim->Init.RepetitionCounter = 0;

    // Initialize timer
    if (HAL_TIM_IC_Init(htim) != HAL_OK)
    {
      /* Initialization Error */
        while (1)
            ;
    }
}

void minilibPWMTimer_init(minilibTimer_props_s* props, TIM_TypeDef* instance, double frequency)
{
    props->htim.Instance = instance;
    props->Frequency = frequency;

    minilibTimer_init(props);
}

void minilibPWM_IC_init(minilibTimer_props_s* props, TIM_TypeDef* instance, double frequency)
{
    props->htim.Instance = instance;
    props->Frequency = frequency;

    minilibTimer_IC_init(props);
}

void minilibPWM_IC_ConfigChannel(minilibTimer_props_s* props, mlTIM_Channel_e channel, GPIO_TypeDef* port, uint16_t pin)
{
    TIM_IC_InitTypeDef sConfig;
    TIM_SlaveConfigTypeDef sSlaveConfig;

    minilibIC_Pin_Init(props->htim.Instance, port, pin);

    HAL_NVIC_SetPriority(TIM3_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);

    sConfig.ICPrescaler     = TIM_ICPSC_DIV1;
    sConfig.ICFilter        = 0x0;  
    sConfig.ICPolarity      = TIM_ICPOLARITY_FALLING;
    sConfig.ICSelection     = TIM_ICSELECTION_INDIRECTTI;        

    if (HAL_TIM_IC_ConfigChannel(&props->htim, &sConfig, TIM_CHANNEL_1) != HAL_OK)
    {
      /* Configuration Error */
        while (1)
            ;
    }

    sConfig.ICPolarity      = TIM_ICPOLARITY_RISING;
    sConfig.ICSelection     = TIM_ICSELECTION_DIRECTTI;       

    if (HAL_TIM_IC_ConfigChannel(&props->htim, &sConfig, TIM_CHANNEL_2) != HAL_OK)
    {
      /* Configuration Error */
        while (1)
            ;
    }

    sSlaveConfig.SlaveMode     = TIM_SLAVEMODE_RESET;
    sSlaveConfig.InputTrigger  = TIM_TS_TI2FP2;

    if (HAL_TIM_SlaveConfigSynchronization(&props->htim, &sSlaveConfig) != HAL_OK)
    {
      /* Configuration Error */
        while (1)
            ;
    }

    if (HAL_TIM_IC_Start_IT(&props->htim, TIM_CHANNEL_1) != HAL_OK)
    {
      /* Starting Error */
        while (1)
            ;
    }

}

tim_result_e minilibPWM_set_pulse(minilibTimer_props_s* props, mlTIM_Channel_e channel, uint32_t pulse)
{

    TIM_OC_InitTypeDef pwmChannelInit;

    if (pulse > props->Period)
    {
        return TIM_ERROR;
    }

    pwmChannelInit.Pulse = pulse;
    pwmChannelInit.OCMode = TIM_OCMODE_PWM2;
    pwmChannelInit.OCPolarity = TIM_OCPOLARITY_LOW;

    // Save value
    

    __HAL_TIM_SET_COMPARE(&props->htim, channel, pulse);
   
    return TIM_OK;
}

void minilibPWMConfig_Channel(minilibTimer_props_s* props, mlTIM_Channel_e channel, GPIO_TypeDef* port, uint16_t pin)
{
    TIM_OC_InitTypeDef pwmChannelInit;
    // PWM Settings
    pwmChannelInit.OCMode = TIM_OCMODE_PWM1;
    pwmChannelInit.Pulse = 100;
    pwmChannelInit.OCPolarity = TIM_OCPOLARITY_HIGH;
    pwmChannelInit.OCFastMode = TIM_OCFAST_DISABLE;

    minilibPWM_Pin_Init(props->htim.Instance, port, pin);
 
    HAL_TIM_PWM_ConfigChannel(&props->htim, &pwmChannelInit, channel);
    HAL_TIM_PWM_Start(&props->htim, channel);
}

tim_result_e minilibPWM_Pin_Init(TIM_TypeDef* instance, GPIO_TypeDef* port, uint16_t pin)
{
    GPIO_InitTypeDef GPIO_InitStruct;


    if (port == GPIOA)
    {
        __GPIOA_CLK_ENABLE();
    }
    else if (port == GPIOB)
    {
        __GPIOB_CLK_ENABLE();
    }
    else if (port == GPIOC)
    {
        __GPIOC_CLK_ENABLE();
    }
    else if (port == GPIOD)
    {
        __GPIOD_CLK_ENABLE();
    }
    else if (port == GPIOE)
    {
        __GPIOE_CLK_ENABLE();
    }
    else if (port == GPIOF)
    {
        __GPIOF_CLK_ENABLE();
    }
    else if (port == GPIOG)
    {
        __GPIOG_CLK_ENABLE();
    }
    else if (port == GPIOH)
    {
        __GPIOH_CLK_ENABLE();
    }
    

    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    if (instance == 0)
    {
        return TIM_ERROR;
    }
#ifdef TIM1
    else if (instance == TIM1)
    {
        GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
    }
#endif // TIM1
#ifdef TIM2
    else if (instance == TIM2)
    {
        GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    }	  
#endif // TIM2
#ifdef TIM3
    else if (instance == TIM3)
    {
        GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
    }	  
#endif // TIM3
#ifdef TIM4
    else if (instance == TIM4)
    {
        GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
    }	  
#endif // TIM4
#ifdef TIM5
    else if (instance == TIM5)
    {
        GPIO_InitStruct.Alternate = GPIO_AF2_TIM5;
    }	  
#endif // TIM5
#ifdef TIM8
    else if (instance == TIM8)
    {
        GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;
    }	  
#endif // TIM8
#ifdef TIM9
    else if (instance == TIM9)
    {
        GPIO_InitStruct.Alternate = GPIO_AF3_TIM9;
    }	  
#endif // TIM9
#ifdef TIM10
    else if (instance == TIM10)
    {
        GPIO_InitStruct.Alternate = GPIO_AF3_TIM10;
    }	  
#endif // TIM10
#ifdef TIM11
    else if (instance == TIM11)
    {
        GPIO_InitStruct.Alternate = GPIO_AF3_TIM11;
    }	  
#endif // TIM11
#ifdef TIM12
    else if (instance == TIM12)
    {
        GPIO_InitStruct.Alternate = GPIO_AF9_TIM12;
    }	  
#endif // TIM12
#ifdef TIM13
    else if (instance == TIM13)
    {
        GPIO_InitStruct.Alternate = GPIO_AF9_TIM13;
    }	  
#endif // TIM13
#ifdef TIM14
    else if (instance == TIM14)
    {
        GPIO_InitStruct.Alternate = GPIO_AF9_TIM14;
    }	  
#endif // TIM14
    else
    {
        return TIM_ERROR;
    }
  
    HAL_GPIO_Init(port, &GPIO_InitStruct);

    return TIM_OK;

}

tim_result_e minilibIC_Pin_Init(TIM_TypeDef* instance, GPIO_TypeDef* port, uint16_t pin)
{
    GPIO_InitTypeDef GPIO_InitStruct;


    if (port == GPIOA)
    {
        __GPIOA_CLK_ENABLE();
    }
    else if (port == GPIOB)
    {
        __GPIOB_CLK_ENABLE();
    }
    else if (port == GPIOC)
    {
        __GPIOC_CLK_ENABLE();
    }
    else if (port == GPIOD)
    {
        __GPIOD_CLK_ENABLE();
    }
    else if (port == GPIOE)
    {
        __GPIOE_CLK_ENABLE();
    }
    else if (port == GPIOF)
    {
        __GPIOF_CLK_ENABLE();
    }
    else if (port == GPIOG)
    {
        __GPIOG_CLK_ENABLE();
    }
    else if (port == GPIOH)
    {
        __GPIOH_CLK_ENABLE();
    }
    

    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    if (instance == 0)
    {
        return TIM_ERROR;
    }
#ifdef TIM1
    else if (instance == TIM1)
    {
        GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
    }
#endif // TIM1
#ifdef TIM2
    else if (instance == TIM2)
    {
        GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    }	  
#endif // TIM2
#ifdef TIM3
    else if (instance == TIM3)
    {
        GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
    }	  
#endif // TIM3
#ifdef TIM4
    else if (instance == TIM4)
    {
        GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
    }	  
#endif // TIM4
#ifdef TIM5
    else if (instance == TIM5)
    {
        GPIO_InitStruct.Alternate = GPIO_AF2_TIM5;
    }	  
#endif // TIM5
#ifdef TIM8
    else if (instance == TIM8)
    {
        GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;
    }	  
#endif // TIM8
#ifdef TIM9
    else if (instance == TIM9)
    {
        GPIO_InitStruct.Alternate = GPIO_AF3_TIM9;
    }	  
#endif // TIM9
#ifdef TIM10
    else if (instance == TIM10)
    {
        GPIO_InitStruct.Alternate = GPIO_AF3_TIM10;
    }	  
#endif // TIM10
#ifdef TIM11
    else if (instance == TIM11)
    {
        GPIO_InitStruct.Alternate = GPIO_AF3_TIM11;
    }	  
#endif // TIM11
#ifdef TIM12
    else if (instance == TIM12)
    {
        GPIO_InitStruct.Alternate = GPIO_AF9_TIM12;
    }	  
#endif // TIM12
#ifdef TIM13
    else if (instance == TIM13)
    {
        GPIO_InitStruct.Alternate = GPIO_AF9_TIM13;
    }	  
#endif // TIM13
#ifdef TIM14
    else if (instance == TIM14)
    {
        GPIO_InitStruct.Alternate = GPIO_AF9_TIM14;
    }	  
#endif // TIM14
    else
    {
        return TIM_ERROR;
    }
  
    HAL_GPIO_Init(port, &GPIO_InitStruct);

    return TIM_OK;

}

tim_result_e minilibPWM_setMicros(minilibTimer_props_s* props, uint8_t channel, uint32_t micros)
{
    if (micros > props->MaxMicros)
    {
        return TIM_ERROR;
    }
    tim_result_e result = minilibPWM_set_pulse(props, channel, (uint32_t)((props->Period - 1) * micros) / props->Micros);
}

tim_result_e minilibPWM_setPercent(minilibTimer_props_s* props, uint8_t channel, uint32_t percent)
{
    if (percent < 0)
    {
        percent = 0;
    }
    else if (percent > 100)
    {
        percent = 100;
    }
    tim_result_e result = minilibPWM_set_pulse(props, channel, (uint32_t)((props->Period - 1) * percent) / 100);
}
//****************************************Interrupts Handler*******************************// 

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    uint32_t val;
    int i = 0;
    int test = 0;

    int channel = htim->Channel;

    if (htim->Instance == TIM1)
    {
        // Get channel index
        while (channel >>= 1) { ++i; }

        tim1_pwm_ic_value[i] = HAL_TIM_ReadCapturedValue(htim, htim->Channel);
    }
    else if (htim->Instance == TIM2)
    {
        // Get channel index
        while (channel >>= 1) { ++i; }

        tim2_pwm_ic_value[i] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
    }
    else if (htim->Instance == TIM3)
    {
        // Get channel index
        while (channel >>= 1) { ++i; }
        tim3_pwm_ic_value[i] = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
    }
    else if (htim->Instance == TIM4)
    {
        // Get channel index
        while (channel >>= 1) { ++i; }

        tim4_pwm_ic_value[i] = HAL_TIM_ReadCapturedValue(htim, htim->Channel);
    }
    else if (htim->Instance == TIM5)
    {
        // Get channel index
        while (channel >>= 1) { ++i; }

        tim5_pwm_ic_value[i] = HAL_TIM_ReadCapturedValue(htim, htim->Channel);
    }
    else if (htim->Instance == TIM8)
    {
        // Get channel index
        while (channel >>= 1) { ++i; }

        tim8_pwm_ic_value[i] = HAL_TIM_ReadCapturedValue(htim, htim->Channel);
    }
    else if (htim->Instance == TIM9)
    {
        // Get channel index
        while (channel >>= 1) { ++i; }

        tim9_pwm_ic_value[i] = HAL_TIM_ReadCapturedValue(htim, htim->Channel);
    }
    else if (htim->Instance == TIM10)
    {
        tim10_pwm_ic_value = HAL_TIM_ReadCapturedValue(htim, htim->Channel);
    }
    else if (htim->Instance == TIM11)
    {
        tim11_pwm_ic_value = HAL_TIM_ReadCapturedValue(htim, htim->Channel);
    }
    else if (htim->Instance == TIM12)
    {
        // Get channel index
        while (channel >>= 1) { ++i; }

        tim12_pwm_ic_value[i] = HAL_TIM_ReadCapturedValue(htim, htim->Channel);
    }
    else if (htim->Instance == TIM13)
    {
        tim13_pwm_ic_value = HAL_TIM_ReadCapturedValue(htim, htim->Channel);
    }
    else if (htim->Instance == TIM14)
    {
        tim14_pwm_ic_value = HAL_TIM_ReadCapturedValue(htim, htim->Channel);
    }

}
