/*
 * ams3932.c
 *
 *  Created on: 2019.1.4
 *      Author: wj1080
 */

#include "ams3932.h"

/**
  * @brief  transmit one byte data and receive a respond byte data by SPI port.
  * @param  wtData, the transmitted data.
  * @retval received data by spi.
  */
static void HAL_Ams3932CommPortsInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	HAL_GPIO_WritePin(AS3932_CS_GPIO_Port, AS3932_CS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(AS3932_SCK_GPIO_Port, AS3932_SCK_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(AS3932_MOSI_GPIO_Port, AS3932_MOSI_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : PtPin */
	GPIO_InitStruct.Pin = AS3932_CS_Pin | AS3932_SCK_Pin | AS3932_MOSI_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = AS3932_MISO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);	
}

/**
  * @brief  transmit one byte data and receive a respond byte data by SPI port.
  * @param  wtData, the transmitted data.
  * @retval received data by spi.
  */
static void ams3932DelayUs(uint16_t delayCnt)
{
	uint16_t k;

	for (k = 0; k < delayCnt; k++)
	{
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
	}
}

/**
  * @brief  transmit one byte data and receive a respond byte data by SPI port.
  * @param  wtData, the transmitted data.
  * @retval received data by spi.
  */
static void ams3932WriteDirectCommand(uint8_t cmd)
{
	uint8_t i;

	LF_CS_High();

	cmd |= 0xC0;			/* Two leading ONE bit indicate direct command. */

	for (i = 0; i < 8; i++)
	{
		LF_SCK_High();

		if (cmd & 0x80)
		{
			LF_MOSI_High();
		}
		else
		{
			LF_MOSI_Low();
		}

		cmd <<= 1;

		LF_SCK_Low();
	}

	LF_MOSI_Low();

	ams3932DelayUs(20);

	LF_CS_Low();
}

/**
  * @brief  transmit one byte data and receive a respond byte data by SPI port.
  * @param  wtData, the transmitted data.
  * @retval received data by spi.
  */
void ams3932WriteSingleRegister(uint8_t regAddr, uint8_t wData)
{
	uint8_t i;
	uint16_t value;

	LF_CS_High();

	value = ((uint16_t)(regAddr & 0x3F) << 8) + wData;

	for (i = 0; i < 16; i++)
	{
		LF_SCK_High();

		if (value & 0x8000)
		{
			LF_MOSI_High();
		}
		else
		{
			LF_MOSI_Low();
		}

		value <<= 1;

		LF_SCK_Low();
	}

	LF_MOSI_Low();

	ams3932DelayUs(20);

	LF_CS_Low();
}

/**
  * @brief  transmit one byte data and receive a respond byte data by SPI port.
  * @param  wtData, the transmitted data.
  * @retval received data by spi.
  */
uint8_t ams3932ReadSingleRegister(uint8_t regAddr)
{
	uint8_t i, value;

	LF_CS_High();

	regAddr &= 0x3F;
	regAddr |= 0x40;			/* 01 pattern indicates READ mode. */

	for (i = 0; i < 8; i++)
	{
		LF_SCK_High();

		if (regAddr & 0x80)
		{
			LF_MOSI_High();
		}
		else
		{
			LF_MOSI_Low();
		}

		regAddr <<= 1;

		LF_SCK_Low();
	}

	value = 0;

	for (i = 0; i < 8; i++)
	{
		LF_SCK_High();

		value <<= 1;

		LF_SCK_Low();

		if (LF_MISO_Read() == GPIO_PIN_SET)
		{
			value |= 0x01;
		}
	}

	LF_MOSI_Low();

	ams3932DelayUs(20);

	LF_CS_Low();

	return (value);
}

/**
  * @brief  transmit one byte data and receive a respond byte data by SPI port.
  * @param  wtData, the transmitted data.
  * @retval received data by spi.
  */
void ams3932Initial(void)
{
	HAL_Ams3932CommPortsInit();							/* Initialize AS3932 hardware communication ports. */
	
	ams3932WriteDirectCommand(AMS3932_PRESET_DEFAULT);
	ams3932WriteDirectCommand(AMS3932_CLEAR_WAKE);

	ams3932WriteSingleRegister(AMS3932_R0, 0x2A);			/* 通道使能ON/OFF模式,禁止扫描模式,且通道1和2打开,通道3关闭.  */
	
	ams3932WriteSingleRegister(AMS3932_R1, 0x2A);			/* 使能曼彻斯特编码,不使能天线阻尼器,双向使能AGC,AGC只在第一个载波触发,不使能数据限幅器,晶体振荡器不使能. */
	
	ams3932WriteSingleRegister(AMS3932_R2, 0x21);

	ams3932WriteSingleRegister(AMS3932_R3, 0xF8);			/* 比较器设置成20mA,数据限幅器在上升沿作用,最小前导码（Preamble）长度3.5mS. */

	ams3932WriteSingleRegister(AMS3932_R4, 0x20);			/* ON/OFF模式下OFF时长1mS,天线阻尼电阻10K,无增益衰减. */

	ams3932WriteSingleRegister(AMS3932_R5, 0x69);			/* 第二个唤醒前导码0X69. */
	ams3932WriteSingleRegister(AMS3932_R6, 0x96);			/* 第一个唤醒前导码0X96. */

	ams3932WriteSingleRegister(AMS3932_R7, 0x2B);			/* R7寄存器设置：超时设置50mS,波特率12. */

	ams3932WriteSingleRegister(AMS3932_R8, 0x00);			/* R8寄存器设置：唤醒频率95-150kHz. */

	ams3932WriteDirectCommand(AMS3932_CLEAR_WAKE);
	ams3932WriteDirectCommand(AMS3932_CLEAR_FALSE);
}



