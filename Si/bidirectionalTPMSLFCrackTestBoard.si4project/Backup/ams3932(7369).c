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
static void Ams3932_DelayUs(uint16_t delayCnt)
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
static void Ams3932_WriteDirectCommand(uint8_t cmd)
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

	Ams3932_DelayUs(10);

	LF_CS_Low();
}


/**
  * @brief  transmit one byte data and receive a respond byte data by SPI port.
  * @param  wtData, the transmitted data.
  * @retval received data by spi.
  */
void Ams3932_Initial(void)
{
	
}



