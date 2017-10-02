/****************************************

File name     : uart.c
Author        : Charles Ratelle
Creation date : 05/04/2017

****************************************/

//****************************************INCLUDE******************************************// 
#include "minilibUART.h"
#include "nOS.h"

#ifdef __cplusplus
extern "C" {
#endif
//****************************************DEFINES******************************************// 

//****************************************Global Constants*********************************// 
    const uart_periph UART_PERIPHS[NB_OF_UART_PERIPH_ENTRY] =
    {
#define X_XTABLE(NAME, TX_PORT, TX_PIN, RX_PORT, RX_PIN, AF, INSTANCE) {NAME, TX_PORT, TX_PIN, RX_PORT, RX_PIN, AF, INSTANCE},
        X_UART_CROSSTABLE
#undef X_XTABLE
    };
    //****************************************Global Variables*********************************//
#ifdef HUART1
    extern UART_HandleTypeDef huart1;
    extern uint8_t receiveBuffer1[RX_BUFF_SIZE];
#endif // HUART1

#ifdef HUART2
    extern UART_HandleTypeDef huart2;
    extern uint8_t receiveBuffer2[RX_BUFF_SIZE];
#endif // HUART2

#ifdef HUART3
    extern UART_HandleTypeDef huart3;
    extern uint8_t receiveBuffer3[RX_BUFF_SIZE];
#endif // HUART3

#ifdef HUART6
    extern UART_HandleTypeDef huart6;
    extern uint8_t receiveBuffer6[RX_BUFF_SIZE];
#endif // HUART6

//****************************************Function Prototypes******************************// 
    void uart_init_pin(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
    void uart_periph_init();
    void uart_init_periph_clock(uart_periph* uart);
    void uart_set_interrupt(uart_periph* uart);

    __weak void USART1_RX_Handler(void);
    __weak void USART2_RX_Handler(void);
    __weak void USART3_RX_Handler(void);
    __weak void USART6_RX_Handler(void);
//****************************************Local Function***********************************// 
    void uart_init_pin(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
    {
        GPIO_InitTypeDef gpioInit;

                // Enable GPIO clock
        if (GPIOx == GPIOA)
        {
            __GPIOA_CLK_ENABLE();
        }
        else if (GPIOx == GPIOB)
        {
            __GPIOB_CLK_ENABLE();
        }
        else if (GPIOx == GPIOC)
        {
            __GPIOC_CLK_ENABLE();
        }
        else if (GPIOx == GPIOD)
        {
            __GPIOD_CLK_ENABLE();
        }
        else if (GPIOx == GPIOE)
        {
            __GPIOE_CLK_ENABLE();
        }

            // Set GPIO configuration parameters
        gpioInit.Pin        = gpio_pin;
        gpioInit.Mode       = GPIO_MODE_AF_PP;
        gpioInit.Pull       = GPIO_PULLUP;
        gpioInit.Speed      = GPIO_SPEED_FREQ_VERY_HIGH;
        gpioInit.Alternate  = GPIO_AF7_USART2;       /// To be changed

            // Initialize GPIO
        HAL_GPIO_Init(GPIOx, &gpioInit);

    }

    void uart_periph_init(uart_periph* uart, UART_HandleTypeDef* huart)
    {
    
        huart->Instance             = uart->Instance;
        huart->Init.BaudRate        = 115200;
        huart->Init.WordLength      = UART_WORDLENGTH_8B;
        huart->Init.StopBits        = UART_STOPBITS_1;
        huart->Init.Parity          = UART_PARITY_NONE;
        huart->Init.Mode            = UART_MODE_TX_RX;
        huart->Init.HwFlowCtl       = UART_HWCONTROL_NONE;
        huart->Init.OverSampling    = UART_OVERSAMPLING_16;

        if (HAL_UART_Init(huart) != HAL_OK)
        {
            asm("bkpt 255");
        }
    }

    void uart_init_periph_clock(uart_periph* uart)
    {
        if (uart->Instance == USART1)
        {
            __HAL_RCC_USART1_CLK_ENABLE();
        }
        else if (uart->Instance == USART2)
        {
            __HAL_RCC_USART2_CLK_ENABLE();
        }
        else if (uart->Instance == USART3)
        {
            __HAL_RCC_USART3_CLK_ENABLE();
        }
        else if (uart->Instance == USART6)
        {
            __HAL_RCC_USART6_CLK_ENABLE();
        }
    }

    void uart_set_interrupt(uart_periph* uart)
    {
        if (uart->Instance == USART1)
        {
            HAL_NVIC_SetPriority(USART1_IRQn, 3, 0);
            HAL_NVIC_EnableIRQ(USART1_IRQn);
        }
        else if (uart->Instance == USART2)
        {
            HAL_NVIC_SetPriority(USART2_IRQn, 3, 0);
            HAL_NVIC_EnableIRQ(USART2_IRQn);
        }
        else if (uart->Instance == USART3)
        {
            HAL_NVIC_SetPriority(USART3_IRQn, 3, 0);
            HAL_NVIC_EnableIRQ(USART3_IRQn);
        }
        else if (uart->Instance == USART6)
        {
            HAL_NVIC_SetPriority(USART6_IRQn, 3, 0);
            HAL_NVIC_EnableIRQ(USART6_IRQn);
        }
    }

//****************************************Global Function**********************************// 
    void uart_init(uart_periph uart, UART_HandleTypeDef* uart_handle)
    {
        // Initialize UART pins

        uart_init_periph_clock(&uart);

        uart_init_pin(uart.TX_Port, uart.TX_Pin);
        uart_init_pin(uart.RX_Port, uart.RX_Pin);

        uart_set_interrupt(&uart);

            // Initialize UART
        uart_periph_init(&uart, uart_handle);	
    }

    void uart_set_receive_interrupt(uart_periph uart, UART_HandleTypeDef* uart_handle, uint8_t* receive_buffer)
    {
        HAL_UART_Receive_IT(uart_handle, receive_buffer, RX_BUFF_SIZE);
    }

    void uart_send(UART_HandleTypeDef* uart_handle, uint8_t data[], uint8_t size)
    {
        while (HAL_UART_Transmit_IT(uart_handle, data, size) != HAL_OK)
        {
            
        }
    }
//****************************************Interrupts Handler*******************************// 

#ifdef HUART1
    void USART1_IRQHandler(void)
    {
        HAL_UART_IRQHandler(&huart1);
        HAL_UART_Receive_IT(&huart1, (uint8_t *)receiveBuffer1, RX_BUFF_SIZE);
    }			  
#endif // HUART1

#ifdef HUART2
    NOS_ISR(USART2_IRQHandler)
    {
        HAL_UART_IRQHandler(&huart2);
        HAL_UART_Receive_IT(&huart2, (uint8_t *)receiveBuffer2, RX_BUFF_SIZE);
    }			  
#endif // HUART2

#ifdef HUART3
    NOS_ISR(USART3_IRQHandler)
    {
        HAL_UART_IRQHandler(&huart3);
        HAL_UART_Receive_IT(&huart3, (uint8_t *)receiveBuffer3, RX_BUFF_SIZE);
    }			  
#endif // HUART3



    void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) 
    {
        if (huart->Instance == USART1)
        {
            USART1_RX_Handler();
        }
        else if (huart->Instance == USART2)
        {
            USART2_RX_Handler();
        }
        else if (huart->Instance == USART3)
        {
            USART3_RX_Handler();
        }
        else if (huart->Instance == USART6)
        {
            USART6_RX_Handler();
        }            
    }

#ifdef HUART1
    __weak void USART1_RX_Handler(void) 
    {
        HAL_UART_Receive_IT(&huart1, (uint8_t *)receiveBuffer1, RX_BUFF_SIZE);
    }   
#endif // HUART1

#ifdef HUART2
    __weak void USART2_RX_Handler(void) 
    {
        HAL_UART_Receive_IT(&huart2, (uint8_t *)receiveBuffer2, RX_BUFF_SIZE);
    }   
#endif // HUART2

#ifdef HUART3
    __weak void USART3_RX_Handler(void) 
    {
        HAL_UART_Receive_IT(&huart3, (uint8_t *)receiveBuffer3, RX_BUFF_SIZE);
    }   
#endif // HUART3

#ifdef HUART6
    __weak void USART6_RX_Handler(void) 
    {
        HAL_UART_Receive_IT(&huart3, (uint8_t *)receiveBuffer6, RX_BUFF_SIZE);
    }   
#endif // HUART6

#ifdef __cplusplus
}
#endif