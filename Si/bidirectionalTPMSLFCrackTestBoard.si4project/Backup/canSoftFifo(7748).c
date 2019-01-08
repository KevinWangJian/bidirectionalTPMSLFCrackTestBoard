/*
 * canSoftFifo.c
 *
 *  Created on: Jan 7, 2019
 *      Author: wj1080
 */

#include <stdlib.h>
#include "canSoftFifo.h"

static CANMsgRxFifoTypeDef canRxMsgBuffer = 
{
	.ptRxWrite = 0,
	.ptRxRead  = 0,
	.rxBuff[0].frameType = (CAN_FrameTypeAndIDFormat_Def)0,
	.rxBuff[1].frameType = (CAN_FrameTypeAndIDFormat_Def)0,
	.rxBuff[2].frameType = (CAN_FrameTypeAndIDFormat_Def)0,
	.rxBuff[3].frameType = (CAN_FrameTypeAndIDFormat_Def)0,
	.rxBuff[4].frameType = (CAN_FrameTypeAndIDFormat_Def)0,
	.rxBuff[5].frameType = (CAN_FrameTypeAndIDFormat_Def)0,
	.rxBuff[6].frameType = (CAN_FrameTypeAndIDFormat_Def)0,
	.rxBuff[7].frameType = (CAN_FrameTypeAndIDFormat_Def)0,
	.rxBuff[8].frameType = (CAN_FrameTypeAndIDFormat_Def)0,
	.rxBuff[9].frameType = (CAN_FrameTypeAndIDFormat_Def)0,
};

static CANMsgTxFifoTypeDef canTxMsgBuffer = 
{
	.ptTxWrite = 0,
	.ptTxRead  = 0,
	.txBuff[0].frameType = (CAN_FrameTypeAndIDFormat_Def)0,
	.txBuff[1].frameType = (CAN_FrameTypeAndIDFormat_Def)0,
	.txBuff[2].frameType = (CAN_FrameTypeAndIDFormat_Def)0,
	.txBuff[3].frameType = (CAN_FrameTypeAndIDFormat_Def)0,
	.txBuff[4].frameType = (CAN_FrameTypeAndIDFormat_Def)0,
	.txBuff[5].frameType = (CAN_FrameTypeAndIDFormat_Def)0,
	.txBuff[6].frameType = (CAN_FrameTypeAndIDFormat_Def)0,
	.txBuff[7].frameType = (CAN_FrameTypeAndIDFormat_Def)0,
	.txBuff[8].frameType = (CAN_FrameTypeAndIDFormat_Def)0,
	.txBuff[9].frameType = (CAN_FrameTypeAndIDFormat_Def)0,
};


/**
  * @brief  Write a CAN message to TX buffer.If user want to send a CAN message,just call this func one time.
  * @param  pWtMsg, the pointer to a CAN message which will be sent.
  * @retval 0, Write to CAN TX buffer successfully; 
  * @retval -1, Write to CAN TX buffer unsuccessfully;
  */
int WriteCanTxMessageBuffer(CAN_MessageTypeDef* pWtMsg)
{
	if (pWtMsg == NULL)return (-1);

	if (canTxMsgBuffer.txBuff[canTxMsgBuffer.ptTxWrite].frameType != (CAN_FrameTypeAndIDFormat_Def)0)
	{
		return (-1);
	}
	else
	{
		canTxMsgBuffer.txBuff[canTxMsgBuffer.ptTxWrite].frameId = pWtMsg->frameId;
		canTxMsgBuffer.txBuff[canTxMsgBuffer.ptTxWrite].dLc     = pWtMsg->dLc;

		for (uint8_t i = 0; i < canTxMsgBuffer.txBuff[canTxMsgBuffer.ptTxWrite].dLc; i++)
		{
			canTxMsgBuffer.txBuff[canTxMsgBuffer.ptTxWrite].data[i] = pWtMsg->data[i];
		}

		canTxMsgBuffer.txBuff[canTxMsgBuffer.ptTxWrite].frameType = pWtMsg->frameType;

		canTxMsgBuffer.ptTxWrite++;
		if (canTxMsgBuffer.ptTxWrite >= CAN_TXMSG_FIFO_SIZE)
		{
			canTxMsgBuffer.ptTxWrite = 0;
		}

		return (0);
	}
}

/**
  * @brief  Read out a CAN message from TX buffer. If there is a valid CAN message,then user should call CAN message send function. 
  * @param  pRdMsg, the pointer to a CAN message which will be read out.
  * @retval 0, Read out CAN message successfully and the message is valid; 
  * @retval -1, Read out CAN message unsuccessfull and the message is invalid;
  */
int ReadCanTxMessageBuffer(CAN_MessageTypeDef* pRdMsg)
{
	if (pRdMsg == NULL)return (-1);

	if (canTxMsgBuffer.txBuff[canTxMsgBuffer.ptTxRead].frameType == (CAN_FrameTypeAndIDFormat_Def)0)
	{
		return (-1);
	}
	else
	{
		pRdMsg->frameId = canTxMsgBuffer.txBuff[canTxMsgBuffer.ptTxRead].frameId;
		canTxMsgBuffer.txBuff[canTxMsgBuffer.ptTxRead].frameId = 0;

		pRdMsg->dLc = canTxMsgBuffer.txBuff[canTxMsgBuffer.ptTxRead].dLc;
		canTxMsgBuffer.txBuff[canTxMsgBuffer.ptTxRead].dLc = 0;

		for (uint8_t i = 0; i < (pRdMsg->dLc); i++)
		{
			pRdMsg->data[i] = canTxMsgBuffer.txBuff[canTxMsgBuffer.ptTxRead].data[i];
			canTxMsgBuffer.txBuff[canTxMsgBuffer.ptTxRead].data[i] = 0;
		}

		pRdMsg->frameType = canTxMsgBuffer.txBuff[canTxMsgBuffer.ptTxRead].frameType;
		canTxMsgBuffer.txBuff[canTxMsgBuffer.ptTxRead].frameType = (CAN_FrameTypeAndIDFormat_Def)0;

		canTxMsgBuffer.ptTxRead++;
		if (canTxMsgBuffer.ptTxRead >= CAN_TXMSG_FIFO_SIZE) 
		{
			canTxMsgBuffer.ptTxRead = 0;
		}

		return (0);
	}
}

/**
  * @brief  Write a CAN message to RX buffer.If RX ISR routine received a CAN message from CAN bus,user should call this func right now.
  * @param  pWtMsg, the pointer to a CAN message which have been received.
  * @retval 0, Write to CAN RX buffer successfully; 
  * @retval -1, Write to CAN RX buffer unsuccessfully;
  */
int WriteCanRxMessageBuffer(CAN_MessageTypeDef* pWtMsg)
{
	if (pWtMsg == NULL)return (-1);

	if (canRxMsgBuffer.rxBuff[canRxMsgBuffer.ptRxWrite].frameType != (CAN_FrameTypeAndIDFormat_Def)0) 
	{
		return (-1);
	}
	else 
	{
		canRxMsgBuffer.rxBuff[canRxMsgBuffer.ptRxWrite].frameId = pWtMsg->frameId;
		canRxMsgBuffer.rxBuff[canRxMsgBuffer.ptRxWrite].dLc = pWtMsg->dLc;

		for (uint8_t i = 0; i < canRxMsgBuffer.rxBuff[canRxMsgBuffer.ptRxWrite].dLc; i++)
		{
			canRxMsgBuffer.rxBuff[canRxMsgBuffer.ptRxWrite].data[i] = pWtMsg->data[i];
		}

		canRxMsgBuffer.rxBuff[canRxMsgBuffer.ptRxWrite].frameType = pWtMsg->frameType;

		canRxMsgBuffer.ptRxWrite++;
		if (canRxMsgBuffer.ptRxWrite >= CAN_RXMSG_FIFO_SIZE) 
		{
			canRxMsgBuffer.ptRxWrite = 0;
		}

		return (0);
	}
}

/**
  * @brief  Read out a CAN message from RX buffer.If user want to see whethere there is a valid CAN message in RX buffer,just call this func.
  * @param  pRdMsg, the pointer to a CAN message which will be read out.
  * @retval 0, Read out CAN message successfully and the message is valid; 
  * @retval -1, Read out CAN message unsuccessfull and the message is invalid;
  */
int ReadCanRxMessageBuffer(CAN_MessageTypeDef* pRdMsg)
{
	if (pRdMsg == NULL)return (-1);

	if (canRxMsgBuffer.rxBuff[canRxMsgBuffer.ptRxRead].frameType == (CAN_FrameTypeAndIDFormat_Def)0) 
	{
		return (-1);
	}
	else 
	{
		pRdMsg->frameId = canRxMsgBuffer.rxBuff[canRxMsgBuffer.ptRxRead].frameId;
		canRxMsgBuffer.rxBuff[canRxMsgBuffer.ptRxRead].frameId = 0;

		pRdMsg->dLc = canRxMsgBuffer.rxBuff[canRxMsgBuffer.ptRxRead].dLc;
		canRxMsgBuffer.rxBuff[canRxMsgBuffer.ptRxRead].dLc = 0;

		for (uint8_t i = 0; i < (pRdMsg->dLc); i++)
		{
			pRdMsg->data[i] = canRxMsgBuffer.rxBuff[canRxMsgBuffer.ptRxRead].data[i];
			canRxMsgBuffer.rxBuff[canRxMsgBuffer.ptRxRead].data[i] = 0;
		}

		pRdMsg->frameType = canRxMsgBuffer.rxBuff[canRxMsgBuffer.ptRxRead].frameType;
		canRxMsgBuffer.rxBuff[canRxMsgBuffer.ptRxRead].frameType = (CAN_FrameTypeAndIDFormat_Def)0;

		canRxMsgBuffer.ptRxRead++;
		if (canRxMsgBuffer.ptRxRead >= CAN_RXMSG_FIFO_SIZE)
		{
			canRxMsgBuffer.ptRxRead = 0;
		}

		return (0);
	}
}
