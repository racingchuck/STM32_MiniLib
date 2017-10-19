#include <stm32f4xx_hal.h>
#include "minilibUART.h"
#include "nOS.h"

#ifdef __cplusplus
extern "C"
#endif

#define UART_PERIPH         USART2_1
#define UART1_BUF_SIZE      128
#define THREAD_STACK_SIZE   128

UART_HandleTypeDef huart2;
nOS_Queue uart2Queue;
uint8_t uart2_char_data[UART1_BUF_SIZE];
uint8_t receiveBuffer2;
nOS_Sem uart2_rx_rdy_sem;

void uart_rx_task_init(void);
int uart_rx_task(void* arg);
static void SysTick_Init(void);

nOS_Thread UartThread;
nOS_Stack stackA[THREAD_STACK_SIZE];

NOS_ISR(SysTick_Handler)
{
    nOS_Tick(1);
    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
}

int main(void)
{

    // nOS Initialization
    __disable_irq();
    nOS_Init();
    __enable_irq();


    HAL_Init();

    __GPIOD_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.Pin = GPIO_PIN_12;

    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

    nOS_SemCreate(&uart2_rx_rdy_sem, 0, 10);

    uart_rx_task_init();

    nOS_ThreadCreate(&UartThread, uart_rx_task, 0, stackA, THREAD_STACK_SIZE, NOS_CONFIG_HIGHEST_THREAD_PRIO, NOS_THREAD_READY, "UARTTask");

    SysTick_Init();
    nOS_Start();

    while (1)
    {
        
    }
}


static void SysTick_Init(void)
{
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / NOS_CONFIG_TICKS_PER_SECOND);
}

void uart_rx_task_init(void)
{
    uart_init(UART_PERIPHS[UART_PERIPH], &huart2);
    nOS_QueueCreate(&uart2Queue, uart2_char_data, sizeof(uint8_t), UART1_BUF_SIZE);
    uart_set_receive_interrupt(UART_PERIPHS[UART_PERIPH], &huart2, &receiveBuffer2);
}

int uart_rx_task(void* arg)
{
    uint8_t uart_rx;
    NOS_UNUSED(arg);
    while (1)
    {
        if (nOS_SemTake(&uart2_rx_rdy_sem, NOS_NO_WAIT) == NOS_OK)
        {
            nOS_QueueRead(&uart2Queue, &uart_rx, 1);
            while (uart_rx != '\n')
            {
                nOS_QueueRead(&uart2Queue, &uart_rx, 1);
            }
            HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
        }
        nOS_Sleep(10);
    }


    return 0;
}

void USART2_RX_Handler(void)
{
    switch (nOS_QueueWrite(&uart2Queue, &receiveBuffer2, NOS_NO_WAIT))
    {
    case NOS_OK:
        break;
    case NOS_E_FULL:
        break;
    default:
        while (1)
        {}
    }

    if (receiveBuffer2 == '\n')
    {
        nOS_SemGive(&uart2_rx_rdy_sem);
    }

    HAL_UART_Receive_IT(&huart2, &receiveBuffer2, RX_BUFF_SIZE);
}
