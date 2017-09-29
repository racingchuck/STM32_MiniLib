/****************************************

File name     : can_hal.c
Author        : Charles Ratelle
Creation date : 07/08/2017

****************************************/

//****************************************INCLUDE******************************************// 
#include "can_hal.h"
#include "circbuffer.h"
#include "nOS.h"

#ifdef __cplusplus
extern "C" {
#endif

//****************************************DEFINES******************************************// 
//****************************************Global Constants*********************************// 
    const can_periph CAN_PERIPHS[NB_OF_CAN_PERIPH_ENTRY] =
    {
#define X_XTABLE(NAME, TX_PORT, TX_PIN, RX_PORT, RX_PIN, AF, INSTANCE) {NAME, TX_PORT, TX_PIN, RX_PORT, RX_PIN, AF, INSTANCE},
        X_CAN_CROSSTABLE
#undef X_XTABLE
    };
//****************************************Global Variables*********************************// 
    static uint32_t HAL_RCC_CAN1_CLK_ENABLED = 0;

#ifdef HCAN1
    extern CAN_HandleTypeDef hcan1;
    CanTxMsgTypeDef            TxM_1;
    CanRxMsgTypeDef            RxM_1;
    CAN_FilterConfTypeDef      sFilterConfig_1;
#endif // HCAN1

#ifdef HCAN2
    extern CAN_HandleTypeDef hcan2;
    CanTxMsgTypeDef            TxM_2;
    CanRxMsgTypeDef            RxM_2;
    CAN_FilterConfTypeDef      sFilterConfig_2;
#endif // HCAN2

    static volatile uint8_t CAN1_Com_cplt = 1;
    static volatile uint8_t CAN2_Com_cplt = 1;

//****************************************Function Prototypes******************************// 
    void can_hal_init_pin(GPIO_TypeDef* GPIOx, uint16_t gpio_pin, uint8_t gpio_af);
    void can_hal_init_periph(CAN_HandleTypeDef* can_handle);
    void can_hal_set_interrupt(CAN_HandleTypeDef* can_handle);

    __weak void CAN1_RX_Handler(CAN_HandleTypeDef* can_handle);
    __weak void CAN2_RX_Handler(CAN_HandleTypeDef* can_handle);

//****************************************Local Function***********************************// 
    void can_hal_init_pin(GPIO_TypeDef* GPIOx, uint16_t gpio_pin, uint8_t gpio_af)
    {
        GPIO_InitTypeDef gpioInit_s;

        // Enable GPIO Clock
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

        gpioInit_s.Pin = gpio_pin;
        gpioInit_s.Mode = GPIO_MODE_AF_PP;
        gpioInit_s.Pull = GPIO_NOPULL;
        gpioInit_s.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        gpioInit_s.Alternate = gpio_af; 

        HAL_GPIO_Init(GPIOx, &gpioInit_s);
        
    }

    void can_hal_init_periph(CAN_HandleTypeDef* can_handle)
    {
        if (can_handle->Instance == CAN1)
        {
            if (HAL_RCC_CAN1_CLK_ENABLED == 0)
            {
                __HAL_RCC_CAN1_CLK_ENABLE();
                HAL_RCC_CAN1_CLK_ENABLED++;
            }
            ;
        }
        else if (can_handle->Instance == CAN2)
        {
            __HAL_RCC_CAN2_CLK_ENABLE();

            // If CAN1 clock not enabled enable it
            if (HAL_RCC_CAN1_CLK_ENABLED == 0)
            {
                __HAL_RCC_CAN1_CLK_ENABLE();
                HAL_RCC_CAN1_CLK_ENABLED++;
            }
        }
    }

    void can_hal_set_interrupt(CAN_HandleTypeDef* can_handle)
    {
        if (can_handle->Instance == CAN1)
        {
            HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
        }
        else if (can_handle->Instance == CAN2)
        {
            HAL_NVIC_SetPriority(CAN2_RX0_IRQn, 1, 0);
            HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
        }
    }

//****************************************Global Function**********************************// 
    void can_hal_init(can_periph can, CAN_HandleTypeDef* can_handle)
    {

        can_handle->Instance = can.Instance;
        can_hal_init_periph(can_handle);

        can_hal_init_pin(can.TX_Port, can.TX_Pin, can.Alt_Func);
        can_hal_init_pin(can.RX_Port, can.RX_Pin, can.Alt_Func);

        can_hal_set_interrupt(can_handle);

        can_hal_set_baudrate(can_handle, CAN_1000KBPS);

        if (HAL_CAN_Init(can_handle) != HAL_OK)
        {
            while (1)
                ;
        }
    }

    void can_hal_set_filter_id(CAN_HandleTypeDef* can_handle, uint16_t id)
    {
    

        if (can_handle->Instance == CAN1)
        {
#ifdef HCAN1
            


            can_handle->pTxMsg = &TxM_1;
            can_handle->pRxMsg = &RxM_1;

            sFilterConfig_1.FilterNumber            = 0;
            sFilterConfig_1.FilterMode              = CAN_FILTERMODE_IDLIST;
            sFilterConfig_1.FilterScale             = CAN_FILTERSCALE_16BIT;
            sFilterConfig_1.FilterIdHigh            = id << 5;
            sFilterConfig_1.FilterIdLow             = 0x0000;
            sFilterConfig_1.FilterMaskIdHigh        = 0x0000;
            sFilterConfig_1.FilterMaskIdLow         = 0x0000;
            sFilterConfig_1.FilterFIFOAssignment    = 0;
            sFilterConfig_1.FilterActivation        = ENABLE;
            sFilterConfig_1.BankNumber              = 0x00;

            HAL_CAN_ConfigFilter(can_handle, &sFilterConfig_1);
#endif // HCAN1
        }
        else if (can_handle->Instance == CAN2)
        {
#ifdef HCAN2
            can_handle->pTxMsg = &TxM_2;
            can_handle->pRxMsg = &RxM_2;

            sFilterConfig_2.FilterNumber            = 14;
            sFilterConfig_2.FilterMode              = CAN_FILTERMODE_IDLIST;
            sFilterConfig_2.FilterScale             = CAN_FILTERSCALE_16BIT;
            sFilterConfig_2.FilterIdHigh            = id << 5;
            sFilterConfig_2.FilterIdLow             = 0x0000;
            sFilterConfig_2.FilterMaskIdHigh        = 0x0000;
            sFilterConfig_2.FilterMaskIdLow         = 0x0000;
            sFilterConfig_2.FilterFIFOAssignment    = 0;
            sFilterConfig_2.FilterActivation        = ENABLE;
            sFilterConfig_2.BankNumber              = 14;

            HAL_CAN_ConfigFilter(can_handle, &sFilterConfig_2);
                        
#endif // HCAN2

        }

    }

    void can_hal_set_filter_mask(CAN_HandleTypeDef* can_handle, uint32_t filter_mask, uint32_t filter_id)
    {
    

        if (can_handle->Instance == CAN1)
        {
#ifdef HCAN1
            can_handle->pTxMsg = &TxM_1;
            can_handle->pRxMsg = &RxM_1;

            sFilterConfig_1.FilterNumber            = 0;
            sFilterConfig_1.FilterMode              = CAN_FILTERMODE_IDMASK;
            sFilterConfig_1.FilterScale             = CAN_FILTERSCALE_32BIT;
            sFilterConfig_1.FilterIdHigh            = ((filter_id << 5) | (filter_id >> (32 - 5))) & 0xFFFF; // STID[10:0] & EXTID[17:13]
            sFilterConfig_1.FilterIdLow             = (filter_id >> (11 - 3)) & 0xFFF8; // EXTID[12:5] & 3 Reserved bits
            sFilterConfig_1.FilterMaskIdHigh        = ((filter_mask << 5) | (filter_mask >> (32 - 5))) & 0xFFFF;
            sFilterConfig_1.FilterMaskIdLow         = (filter_mask >> (11 - 3)) & 0xFFF8;
            sFilterConfig_1.FilterFIFOAssignment    = 0;
            sFilterConfig_1.FilterActivation        = ENABLE;
            sFilterConfig_1.BankNumber              = 0x00;

            HAL_CAN_ConfigFilter(can_handle, &sFilterConfig_1);                
#endif // HCAN1

        }
        else if (can_handle->Instance == CAN2)
        {

#ifdef HCAN2
            can_handle->pTxMsg = &TxM_2;
            can_handle->pRxMsg = &RxM_2;

            sFilterConfig_2.FilterNumber            = 14;
            sFilterConfig_2.FilterMode              = CAN_FILTERMODE_IDMASK;
            sFilterConfig_2.FilterScale             = CAN_FILTERSCALE_32BIT;
            sFilterConfig_2.FilterIdHigh            = ((filter_id << 5) | (filter_id >> (32 - 5))) & 0xFFFF; // STID[10:0] & EXTID[17:13]
            sFilterConfig_2.FilterIdLow             = (filter_id >> (11 - 3)) & 0xFFF8; // EXTID[12:5] & 3 Reserved bits
            sFilterConfig_2.FilterMaskIdHigh        = ((filter_mask << 5) | (filter_mask >> (32 - 5))) & 0xFFFF;
            sFilterConfig_2.FilterMaskIdLow         = (filter_mask >> (11 - 3)) & 0xFFF8;
            sFilterConfig_2.FilterFIFOAssignment    = 0;
            sFilterConfig_2.FilterActivation        = ENABLE;
            sFilterConfig_2.BankNumber              = 14;

            HAL_CAN_ConfigFilter(can_handle, &sFilterConfig_2);
                        
#endif // HCAN2
        }

    }

    void can_hal_tx_msg(CAN_HandleTypeDef* can_handle, sCircularBuffer *buffer)
    {
        can_message_s tmp_msg;

        if ((can_handle->State == HAL_CAN_STATE_BUSY_RX0) || (can_handle->State ==  HAL_CAN_STATE_BUSY_RX1) || (can_handle->State == HAL_CAN_STATE_BUSY_RX0_RX1))
        {
            CB_CAN_Read(buffer, &tmp_msg);

            can_handle->pTxMsg->StdId = tmp_msg.ID;
            can_handle->pTxMsg->RTR = CAN_RTR_DATA;
            can_handle->pTxMsg->IDE = CAN_ID_STD;
            can_handle->pTxMsg->DLC = tmp_msg.DLC;

            for (int i = 0; i < tmp_msg.DLC; i++)
            {
                can_handle->pTxMsg->Data[i] = tmp_msg.Data[i];
            }
            if (can_handle->Instance == CAN1)
            {
                CAN1_Com_cplt = 0;
            }
            else if (can_handle->Instance == CAN2)
            {
                CAN2_Com_cplt = 0;
            }

            while (HAL_CAN_Transmit_IT(can_handle) != HAL_OK)
            {
            
            }
        }
    }

    void can_hal_set_baudrate(CAN_HandleTypeDef* can_handle, uint32_t baudrate)
    {
        can_handle->Init.Mode = CAN_MODE_NORMAL;
        can_handle->Init.Prescaler = baudrate;
        can_handle->Init.BS1 = CAN_BS1_16TQ;
        can_handle->Init.BS2 = CAN_BS2_4TQ;
        can_handle->Init.SJW = CAN_SJW_1TQ;

        can_handle->Init.TTCM = DISABLE;
        can_handle->Init.ABOM = ENABLE;
        can_handle->Init.AWUM = DISABLE;
        can_handle->Init.NART = DISABLE;
        can_handle->Init.RFLM = DISABLE;
        can_handle->Init.TXFP = DISABLE;

        while (HAL_CAN_Init(can_handle) != HAL_OK)
        {
        }
    }
//****************************************Interrupts Handler*******************************// 
#ifdef HCAN1
    NOS_ISR(CAN1_RX0_IRQHandler)
    {
        HAL_CAN_IRQHandler(&hcan1);
        CAN1_RX_Handler(&hcan1);
        HAL_CAN_Receive_IT(&hcan1, CAN_FIFO0);
    }     
#endif // HCAN1

#ifdef HCAN2
    NOS_ISR(CAN2_RX0_IRQHandler)
    {
        HAL_CAN_IRQHandler(&hcan2);
        CAN2_RX_Handler(&hcan2);
        HAL_CAN_Receive_IT(&hcan2, CAN_FIFO0);
    }     
#endif // HCAN2

    // Weak reference to CAN1 Receive Interrupt handler
    __weak void CAN1_RX_Handler(CAN_HandleTypeDef* can_handle) {}

        // Weak reference to CAN1 Receive Interrupt handler
    __weak void CAN2_RX_Handler(CAN_HandleTypeDef* can_handle) {}


#ifdef __cplusplus
}
#endif
