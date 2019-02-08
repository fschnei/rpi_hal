/*
 * i2cBitbang.c
 *
 *  Created on: 06.02.2019
 *      Author: Fabian
 */

#include "i2cBitbang.h"

#include "../sysTimer/rpi_sysTimer.h"


// 100kHz => 1/f = 10µs
#define HAL_I2CBITBANG_IDLE_TIME_MIRCOSECONDS 1


static rpi_gpio_pin_t SCL_Pin;
static rpi_gpio_pin_t SDA_Pin;


hal_error_status_t PullDown( rpi_gpio_pin_t Pin )
{
	hal_gpio_SetLo( Pin );
	return hal_gpio_SetOutput( Pin );
}
hal_error_status_t Release( rpi_gpio_pin_t Pin )
{
	return hal_gpio_SetInput( Pin );
}





hal_i2cBitbang_error_t hal_i2cBitbang_Init( rpi_gpio_pin_t SCLPin, rpi_gpio_pin_t SDAPin )
{
	if ( SCLPin >= HAL_GPIO_PIN_LASTENTRY || SDAPin >= HAL_GPIO_PIN_LASTENTRY  )
	{
		return HAL_I2CBITBANG_ERROR_WHILE_SETTINGUP_GPIO;
	}
	SCL_Pin = SCLPin;
	SDA_Pin = SDAPin;

	if ( 	hal_gpio_SetInput(SCL_Pin) != HAL_ERROR_NO_ERROR ||
			hal_gpio_SetInput(SDA_Pin) != HAL_ERROR_NO_ERROR )
	{
		return HAL_I2CBITBANG_ERROR_WHILE_SETTINGUP_GPIO;
	}
	if ( 	hal_gpio_SetLo(SCL_Pin) != HAL_ERROR_NO_ERROR ||
			hal_gpio_SetLo(SDA_Pin) != HAL_ERROR_NO_ERROR )
	{
		return HAL_I2CBITBANG_ERROR_WHILE_SETTINGUP_GPIO;
	}

	return HAL_I2CBITBANG_ERROR_NO_ERROR;
}


hal_i2cBitbang_error_t hal_i2cBitbang_SendStart( void )
{
	PullDown(SDA_Pin);

	PullDown(SCL_Pin);

	hal_sysTimer_WaitMicroSeconds( HAL_I2CBITBANG_IDLE_TIME_MIRCOSECONDS );

	return HAL_I2CBITBANG_ERROR_NO_ERROR;
}
hal_i2cBitbang_error_t hal_i2cBitbang_SendStop( void )
{
	Release(SCL_Pin);

	Release(SDA_Pin);

	hal_sysTimer_WaitMicroSeconds( HAL_I2CBITBANG_IDLE_TIME_MIRCOSECONDS );

	return HAL_I2CBITBANG_ERROR_NO_ERROR;
}


hal_i2cBitbang_error_t hal_i2cBitbang_SendBit( uint8_t Bit, uint8_t ReleaseSdaAfter )
{
	hal_gpio_level_t scl_level;

	//PullDown(SCL_Pin);

	if ( Bit == 0 )
	{
		PullDown(SDA_Pin);
	}
	else
	{
		Release(SDA_Pin);
	}

	Release(SCL_Pin);

	hal_sysTimer_WaitMicroSeconds( HAL_I2CBITBANG_IDLE_TIME_MIRCOSECONDS );

	// clk strech test
	hal_gpio_GetValue(SCL_Pin, &scl_level);
	if ( scl_level != HAL_GPIO_LVL_HI )
	{
		return HAL_I2CBITBANG_ERROR_CLK_STRETCH;
	}

	PullDown(SCL_Pin);

	hal_sysTimer_WaitMicroSeconds( HAL_I2CBITBANG_IDLE_TIME_MIRCOSECONDS );

	if ( ReleaseSdaAfter != 0)
	{
		// release sda
		Release(SDA_Pin);
	}
	else
	{
		// pull sda low
		PullDown(SDA_Pin);
	}

	return HAL_I2CBITBANG_ERROR_NO_ERROR;
}
hal_i2cBitbang_error_t hal_i2cBitbang_ReceiveBit( uint8_t * Bit, uint8_t ReleaseSdaAfter )
{
	hal_gpio_level_t sda_level;
	hal_error_status_t ret;

	PullDown(SCL_Pin);
	Release(SDA_Pin);


	Release(SCL_Pin);

	hal_sysTimer_WaitMicroSeconds( HAL_I2CBITBANG_IDLE_TIME_MIRCOSECONDS );

	ret = hal_gpio_GetValue(SDA_Pin, &sda_level);
	if ( ret == HAL_ERROR_GENERAL_ERROR )
	{
		// error
		return HAL_I2CBITBANG_ERROR_WHILE_SETTINGUP_GPIO;
	}
	*Bit = sda_level;

	PullDown(SCL_Pin);

	hal_sysTimer_WaitMicroSeconds( HAL_I2CBITBANG_IDLE_TIME_MIRCOSECONDS );

	if ( ReleaseSdaAfter != 0)
	{
		// release sda
		Release(SDA_Pin);
	}
	else
	{
		// pull sda low
		PullDown(SDA_Pin);
	}

	return HAL_I2CBITBANG_ERROR_NO_ERROR;
}


hal_i2cBitbang_error_t hal_i2cBitbang_SendByte( uint8_t Byte )
{
	uint8_t value;

	for ( int i = 7; i >= 0 && i < 8; i--)
	{
		value = ( Byte >> i ) & 0x01;
		if ( value != 0 )
		{
			value = 1;
		}
		hal_i2cBitbang_SendBit(value, 0);
	}
}
hal_i2cBitbang_error_t hal_i2cBitbang_ReceiveByte( uint8_t * Bit, uint8_t ReleaseSdaAfter )
{

}


hal_i2cBitbang_error_t hal_i2cBitbang_WriteTransaction( uint8_t SlaveAddress, uint8_t Data )
{
	uint8_t receiveBitACK;

	hal_i2cBitbang_SendStart();

	hal_i2cBitbang_SendByte( ( SlaveAddress << 1 ) & 0xFE );

	hal_i2cBitbang_ReceiveBit( &receiveBitACK, 0 );

	if ( receiveBitACK != 0 )
	{
		// no ack
		hal_i2cBitbang_SendStop();
		return HAL_I2CBITBANG_ERROR_NO_SLAVE_ACKNOWLEDGE;
	}

	// ack
	hal_i2cBitbang_SendByte( Data );

	hal_i2cBitbang_ReceiveBit( &receiveBitACK, 0 );

	if ( receiveBitACK != 0 )
	{
		// no ack
		hal_i2cBitbang_SendStop();
		return HAL_I2CBITBANG_ERROR_NO_SLAVE_ACKNOWLEDGE;
	}

	hal_i2cBitbang_SendStop();

	if ( receiveBitACK != 0 )
	{
		// no ack
		return HAL_I2CBITBANG_ERROR_NO_SLAVE_ACKNOWLEDGE;
	}

	// ack
	return HAL_I2CBITBANG_ERROR_NO_ERROR;

}






