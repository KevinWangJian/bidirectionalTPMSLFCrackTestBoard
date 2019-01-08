/*
 * canSoftFifo.h
 *
 *  Created on: Jan 7, 2019
 *      Author: wj1080
 */

#ifndef CANSOFTFIFO_H_
#define CANSOFTFIFO_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "can.h"

#define  CAN_RXMSG_FIFO_SIZE	(10)
#define  CAN_TXMSG_FIFO_SIZE	(10)


typedef struct
{
	uint8_t ptRxWrite;
	uint8_t ptRxRead;
	CAN_MessageTypeDef rxBuff[CAN_RXMSG_FIFO_SIZE];
}CANMsgRxFifoTypeDef;

typedef struct
{
	uint8_t ptTxWrite;
	uint8_t ptTxRead;
	CAN_MessageTypeDef txBuff[CAN_TXMSG_FIFO_SIZE];
}CANMsgTxFifoTypeDef;


int WriteCanTxMessageBuffer(CAN_MessageTypeDef* pWtMsg);

int ReadCanTxMessageBuffer(CAN_MessageTypeDef* pRdMsg);

int WriteCanRxMessageBuffer(CAN_MessageTypeDef* pWtMsg);

int ReadCanRxMessageBuffer(CAN_MessageTypeDef* pRdMsg);


#ifdef __cplusplus
}
#endif

#endif /* CANSOFTFIFO_H_ */
