#include <stm32f4xx_hal.h>
#include "minilibTimer.h"
#include "arm_math.h"

#ifdef __cplusplus
extern "C"
#endif

TIM_HandleTypeDef htim1;
extern uint32_t tim3_pwm_ic_value[4];
static int timerValue;
static uint32_t timerICValue;

static uint32_t micros = 2000;
static float micros_2 = 1500;

minilibTimer_props_s timer4_props;
minilibTimer_props_s timer3_props;


void SystemClock_Config(void);

void SysTick_Handler(void)
{
    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
}

int main(void)
{
    
    HAL_Init();
    SystemClock_Config();
     
    minilibPWMTimer_init(&timer4_props, TIM4, 50);
    minilibPWMConfig_Channel(&timer4_props, mlTIM_Channel_1, GPIOD, GPIO_PIN_12);
   // minilibPWMConfig_Channel(&timer4_props, mlTIM_Channel_3, GPIOD, GPIO_PIN_14);
    minilibPWM_IC_init(&timer3_props, TIM3, 100);
    minilibPWM_IC_ConfigChannel(&timer3_props, mlTIM_Channel_2, GPIOB, GPIO_PIN_5);


    if (minilibPWM_setMicros(&timer4_props, mlTIM_Channel_1, micros) != TIM_OK)
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
     /* if (minilibPWM_setMicros(&timer4_props, mlTIM_Channel_3, 2000) != TIM_OK)
          HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);*/

    for (;;)
    {
        if (micros > 1999)
            micros = 1000;
       //if (micros_2 > 1999)
       //    micros_2 = 1000;

                	//minilibPWM_setMicros(&timer4_props, mlTIM_Channel_1, micros);
                	//minilibPWM_setMicros(&timer4_props, mlTIM_Channel_3, micros_2);
                	micros += 10;
                	//micros_2 += 20;*/
        HAL_Delay(5);

        timerICValue = tim3_pwm_ic_value[0];
        micros_2 = (float)(timerICValue / (((float)timer3_props.TimerClockFreq) / ((float)timer3_props.Prescaler*1000000.0))) ;
    }
}

void SystemClock_Config(void)
{

    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;

        /**Configure the main internal regulator output voltage 
        */
    __HAL_RCC_PWR_CLK_ENABLE();

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

        /**Initializes the CPU, AHB and APB busses clocks 
        */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 168;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        while (1)
            ;
    }

        /**Initializes the CPU, AHB and APB busses clocks 
        */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        while (1)
            ;
    }

        /**Configure the Systick interrupt time 
        */
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

        /**Configure the Systick 
        */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

      /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&timer3_props.htim);
}
