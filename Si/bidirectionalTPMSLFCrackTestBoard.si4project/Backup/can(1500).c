/**
  ******************************************************************************
  * File Name          : CAN.c
  * Description        : This file provides code for the configuration
  *                      of the CAN instances.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "can.h"

#include "gpio.h"

/* USER CODE BEGIN 0 */
static CanTxMsgTypeDef TxMessage;
static CanRxMsgTypeDef RxMessage;
/* USER CODE END 0 */

CAN_HandleTypeDef hcan;

/* CAN init function */
void MX_CAN_Init(void)
{

  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 12;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SJW = CAN_SJW_2TQ;
  hcan.Init.BS1 = CAN_BS1_2TQ;
  hcan.Init.BS2 = CAN_BS2_3TQ;
  hcan.Init.TTCM = DISABLE;
  hcan.Init.ABOM = DISABLE;
  hcan.Init.AWUM = DISABLE;
  hcan.Init.NART = DISABLE;
  hcan.Init.RFLM = DISABLE;
  hcan.Init.TXFP = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();
  
    /**CAN GPIO Configuration    
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX 
    */
    GPIO_InitStruct.Pin = CANBus_Rx_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(CANBus_Rx_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = CANBus_Tx_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(CANBus_Tx_GPIO_Port, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(USB_HP_CAN1_TX_IRQn, 4, 0);
    HAL_NVIC_EnableIRQ(USB_HP_CAN1_TX_IRQn);
    HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();
  
    /**CAN GPIO Configuration    
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX 
    */
    HAL_GPIO_DeInit(GPIOA, CANBus_Rx_Pin|CANBus_Tx_Pin);

    /* CAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USB_HP_CAN1_TX_IRQn);
    HAL_NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

/**
  * @brief  The CAN handle structer member(mail box) initialization.
  * @param  pCanHandle, CAN module handle pointer.
  * @retval None.
  */
static void HAL_CANMailBox_Init(CAN_HandleTypeDef* pCanHandle)
{
	pCanHandle->pTxMsg = &TxMessage;
	pCanHandle->pRxMsg = &RxMessage;

	if (HAL_CAN_Init(pCanHandle) != HAL_OK) 

	{
		_Error_Handler(__FILE__, __LINE__);
	}
}

/**
  * @brief  Initialize CAN module's filter.
  * @param  pCanHandle, CAN module handle pointer.
  * @retval None.
  */
static void HAL_CANFilter_Init(CAN_HandleTypeDef* pCanHandle)
{
	CAN_FilterConfTypeDef can_filter;

/*
	can_filter.FilterIdHigh 		= ((((uint32_t)0x37 << 21) | CAN_ID_STD | CAN_RTR_DATA) & 0xFFFF0000) >> 16;
	can_filter.FilterIdLow  		= ((((uint32_t)0x37 << 21) | CAN_ID_STD | CAN_RTR_DATA) & 0x0000FFFF);
	can_filter.FilterMaskIdHigh 	= ((((uint32_t)0xE8 << 21) | CAN_ID_EXT | CAN_RTR_REMOTE) & 0xFFFF0000) >> 16;
	can_filter.FilterMaskIdLow  	= ((((uint32_t)0xE8 << 21) | CAN_ID_EXT | CAN_RTR_REMOTE) & 0x0000FFFF);
*/
	can_filter.FilterIdHigh 		= 0x00000000;
	can_filter.FilterIdLow  		= 0x00000000;
	can_filter.FilterMaskIdHigh 	= 0xffffffff;
	can_filter.FilterMaskIdLow  	= 0xffffffff;
	can_filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	can_filter.FilterNumber			= 0;
	can_filter.FilterMode			= CAN_FILTERMODE_IDMASK;
	can_filter.FilterScale 			= CAN_FILTERSCALE_32BIT;
	can_filter.FilterActivation		= ENABLE;
	can_filter.BankNumber			= 14;

	if (HAL_CAN_ConfigFilter(pCanHandle, &can_filter) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
}

/**
  * @brief  Initialize CAN module's filter.
  * @param  pCanHandle, CAN module handle pointer.
  * @retval None.
  */
void HAL_CANInterrupts_Config(CAN_HandleTypeDef* pCanHandle, uint32_t itSrc, FunctionalState status)
{
	if (status == ENABLE)
	{
		__HAL_CAN_ENABLE_IT(pCanHandle, itSrc);
	}
	else if (status == DISABLE)
	{
		__HAL_CAN_DISABLE_IT(pCanHandle, itSrc);
	}
}

/**
  * @brief  Send a CAN message by specified CAN module.
  * @param  pCanHandle, CAN module handle pointer.
  * @param  pTxMsg, CAN message stored buffer which will be sent.
  * @retval 0, Send successfully; -1, Send unsuccessfully;
  */
int HAL_CANSendMessage(CAN_HandleTypeDef* pCanHandle, CAN_MessageTypeDef* pTxMsg)
{
	CanTxMsgTypeDef TxMsgHead;

	if ((pCanHandle == NULL) || (pTxMsg == NULL))return (-1);

	if ((pTxMsg->frameType == DataFrameWithStandardId) || \
		(pTxMsg->frameType == RemoteFrameWithStandardId))
	{
		if (pTxMsg->frameId > 0x7FF)return (-1);

		TxMsgHead.IDE = CAN_ID_STD;
		TxMsgHead.StdId = pTxMsg->frameId;

		if (pTxMsg->frameType == DataFrameWithStandardId)
		{
			TxMsgHead.RTR = CAN_RTR_DATA;

			if (pTxMsg->dLc > 8)return (-1);
			TxMsgHead.DLC = pTxMsg->dLc;

			for (uint8_t i = 0; i < TxMsgHead.DLC; i++)
			{
				TxMsgHead.Data[i] = pTxMsg->data[i];
			}
		}
		else
		{
			TxMsgHead.RTR = CAN_RTR_REMOTE;
			TxMsgHead.DLC = 0;
		}
	}
	else if ((pTxMsg->frameType == DataFrameWithExtendedId) || \
			 (pTxMsg->frameType == RemoteFrameWithExtendedId))
	{
		if (pTxMsg->frameId > 0x1FFFFFFF)return (-1);

		TxMsgHead.IDE = CAN_ID_EXT;
		TxMsgHead.ExtId = pTxMsg->frameId;

		if (pTxMsg->frameType == DataFrameWithExtendedId)
		{
			TxMsgHead.RTR = CAN_RTR_DATA;

			if (pTxMsg->dLc > 8)return (-1);
			TxMsgHead.DLC = pTxMsg->dLc;

			for (uint8_t i = 0; i < TxMsgHead.DLC; i++)
			{
				TxMsgHead.Data[i] = pTxMsg->data[i];
			}			
		}
		else
		{
			TxMsgHead.RTR = CAN_RTR_REMOTE;
			TxMsgHead.DLC = 0;
		}
	}
	else
	{
		return (-1);
	}

	pCanHandle->pTxMsg = &TxMsgHead;

#if (configCAN_TRANSMIT_INTERRUPT_ENABLE == 0)
	if (HAL_CAN_Transmit(pCanHandle, 100) == HAL_OK)
	{
		return (0);
	}
	else
	{
		return (-1);
	}
#else
	if (HAL_CAN_Transmit_IT(pCanHandle) == HAL_OK)
	{
		HAL_CANInterrupts_Config(pCanHandle, CAN_IT_EWG | \
											 CAN_IT_EPV | \
											 CAN_IT_BOF | \
											 CAN_IT_LEC | \
											 CAN_IT_ERR, 
								DISABLE);
		
		return (0);
	}
	else
	{
		return (-1);
	}
#endif
}

/**
  * @brief  Receive a CAN message by specified CAN module.
  * @param  pCanHandle, CAN module handle pointer.
  * @param  pRxMsg, CAN message stored buffer which will be received.
  * @retval 0, Send successfully; -1, Send unsuccessfully;
  */
int HAL_CANReceiveMessage(CAN_HandleTypeDef* pCanHandle, CAN_MessageTypeDef* pRxMsg)
{
	if ((pCanHandle == NULL) || (pRxMsg == NULL))return (-1);

#if (configUSE_CANRX_FIFO == 0)
	if (pCanHandle->pRxMsg->IDE == CAN_ID_STD)
	{
		pRxMsg->frameId = pCanHandle->pRxMsg->StdId;

		if (pCanHandle->pRxMsg->RTR == CAN_RTR_DATA)
		{
			pRxMsg->frameType = DataFrameWithStandardId;
			pRxMsg->dLc       = pCanHandle->pRxMsg->DLC;
		}
		else if (pCanHandle->pRxMsg->RTR == CAN_RTR_REMOTE)
		{
			pRxMsg->frameType = RemoteFrameWithStandardId;
			pRxMsg->dLc       = 0;
		}
		else 
		{
			return (-1);
		}
	}
	else if (pCanHandle->pRxMsg->IDE == CAN_ID_EXT)
	{
		pRxMsg->frameId = pCanHandle->pRxMsg->ExtId;

		if (pCanHandle->pRxMsg->RTR == CAN_RTR_DATA)
		{
			pRxMsg->frameType = DataFrameWithExtendedId;
			pRxMsg->dLc       = pCanHandle->pRxMsg->DLC;
		}
		else if (pCanHandle->pRxMsg->RTR == CAN_RTR_REMOTE)
		{
			pRxMsg->frameType = RemoteFrameWithExtendedId;
			pRxMsg->dLc       = 0;
		}
		else
		{
			return (-1);
		}
	}
	else
	{
		return (-1);
	}

	if (pRxMsg->dLc != 0)
	{
		for (uint8_t i = 0; i < pRxMsg->dLc; i++)
		{
			pRxMsg->data[i] = pCanHandle->pRxMsg->Data[i];
		}
	}
#else
	if (pCanHandle->pRx1Msg->IDE == CAN_ID_STD)
	{
		pRxMsg->frameId = pCanHandle->pRx1Msg->StdId;

		if (pCanHandle->pRx1Msg->RTR == CAN_RTR_DATA)
		{
			pRxMsg->frameType = DataFrameWithStandardId;
			pRxMsg->dLc 	  = pCanHandle->pRx1Msg->DLC;
		}
		else if (pCanHandle->pRx1Msg->RTR == CAN_RTR_REMOTE)
		{
			pRxMsg->frameType = RemoteFrameWithStandardId;
			pRxMsg->dLc 	  = 0;
		}
		else 
		{
			return (-1);
		}
	}
	else if (pCanHandle->pRx1Msg->IDE == CAN_ID_EXT)
	{
		pRxMsg->frameId = pCanHandle->pRx1Msg->ExtId;

		if (pCanHandle->pRx1Msg->RTR == CAN_RTR_DATA)
		{
			pRxMsg->frameType = DataFrameWithExtendedId;
			pRxMsg->dLc 	  = pCanHandle->pRx1Msg->DLC;
		}
		else if (pCanHandle->pRxMsg->RTR == CAN_RTR_REMOTE)
		{
			pRxMsg->frameType = RemoteFrameWithExtendedId;
			pRxMsg->dLc 	  = 0;
		}
		else
		{
			return (-1);
		}
	}
	else
	{
		return (-1);
	}

	if (pRxMsg->dLc != 0)
	{
		for (uint8_t i = 0; i < pRxMsg->dLc; i++)
		{
			pRxMsg->data[i] = pCanHandle->pRx1Msg->Data[i];
		}
	}
#endif

	return (0);
}

/**
  * @brief  CAN module's attribute intialization including CAN mail box, filter and interrupt status.
  * @brief  This function must be called after calling void MX_CAN_Init(void), and before using CAN module.
  * @param  None.
  * @retval None.
  */
void CanAttributeInit(void)
{
	HAL_CANMailBox_Init(&hcan);
	HAL_CANFilter_Init(&hcan);
	
#if (configCAN_FIFO0_RECEIVE_INTERRUPT_ENABLE == 1)
	HAL_CANInterrupts_Config(&hcan, CAN_IT_FMP0, ENABLE);
#endif

#if (configCAN_FIFO1_RECEIVE_INTERRUPT_ENABLE == 1)
	HAL_CANInterrupts_Config(&hcan, CAN_IT_FMP1, ENABLE);
#endif

#if 0
#if (configCAN_TRANSMIT_INTERRUPT_ENABLE == 1)
	HAL_CANInterrupts_Config(&hcan, CAN_IT_TME, ENABLE);
#endif
#endif
}


/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
