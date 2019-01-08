/*
 * ams3932.h
 *
 *  Created on: 2019.1.4
 *      Author: wj1080
 */

#ifndef AMS3932_H_
#define AMS3932_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "main.h"

#define AS3932_CS_Pin GPIO_PIN_12
#define AS3932_CS_GPIO_Port GPIOB
#define AS3932_SCK_Pin GPIO_PIN_13
#define AS3932_SCK_GPIO_Port GPIOB
#define AS3932_MISO_Pin GPIO_PIN_14
#define AS3932_MISO_GPIO_Port GPIOB
#define AS3932_MOSI_Pin GPIO_PIN_15
#define AS3932_MOSI_GPIO_Port GPIOB
#define AS3932_DAT_Pin GPIO_PIN_6
#define AS3932_DAT_GPIO_Port GPIOC
#define AS3932_WAKE_Pin GPIO_PIN_7
#define AS3932_WAKE_GPIO_Port GPIOC


#define  LF_CS_Low()		HAL_GPIO_WritePin(AS3932_CS_GPIO_Port, AS3932_CS_Pin, GPIO_PIN_RESET)
#define  LF_CS_High()		HAL_GPIO_WritePin(AS3932_CS_GPIO_Port, AS3932_CS_Pin, GPIO_PIN_SET)

#define  LF_SCK_Low()		HAL_GPIO_WritePin(AS3932_SCK_GPIO_Port, AS3932_SCK_Pin, GPIO_PIN_RESET)
#define  LF_SCK_High()		HAL_GPIO_WritePin(AS3932_SCK_GPIO_Port, AS3932_SCK_Pin, GPIO_PIN_SET)

#define  LF_MOSI_Low()		HAL_GPIO_WritePin(AS3932_MOSI_GPIO_Port, AS3932_MOSI_Pin, GPIO_PIN_RESET)
#define  LF_MOSI_High()		HAL_GPIO_WritePin(AS3932_MOSI_GPIO_Port, AS3932_MOSI_Pin, GPIO_PIN_SET)

#define  LF_MISO_Read()		HAL_GPIO_ReadPin(AS3932_MISO_GPIO_Port, AS3932_MISO_Pin)



/* AS3932 Register address definetion. */
#define  AMS3932_R0		(0x00)
#define  AMS3932_R1		(0x01)
#define  AMS3932_R2		(0x02)
#define  AMS3932_R3		(0x03)
#define  AMS3932_R4		(0x04)
#define  AMS3932_R5		(0x05)
#define  AMS3932_R6		(0x06)
#define  AMS3932_R7		(0X07)
#define  AMS3932_R8		(0x08)
#define  AMS3932_R9		(0x09)
#define  AMS3932_R10	(0x0A)
#define  AMS3932_R11	(0x0B)
#define  AMS3932_R12	(0x0C)
#define  AMS3932_R13	(0x0D)

/* List of Direct Command. */
#define  AMS3932_CLEAR_WAKE		(0x00)
#define  AMS3932_RESET_RSSI		(0x01)
#define  AMS3932_TRIM_OSC		(0x02)
#define  AMS3932_CLEAR_FALSE	(0x03)
#define  AMS3932_PRESET_DEFAULT	(0x04)



void ams3932WriteSingleRegister(uint8_t regAddr, uint8_t wData);
uint8_t ams3932ReadSingleRegister(uint8_t regAddr);
void ams3932Initial(void);


#ifdef __cplusplus
}
#endif
#endif /* AMS3932_H_ */
