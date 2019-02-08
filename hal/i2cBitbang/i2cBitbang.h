/*
 * i2cBitbang.h
 *
 *  Created on: 06.02.2019
 *      Author: Fabian
 */

#ifndef I2CBITBANG_H_
#define I2CBITBANG_H_

#include "../gpio/rpi_gpio.h"



typedef enum {
	HAL_I2CBITBANG_ERROR_NO_ERROR = 0,
	HAL_I2CBITBANG_ERROR_FIFO_EMPTY,
	HAL_I2CBITBANG_ERROR_NO_SLAVE_ACKNOWLEDGE,
	HAL_I2CBITBANG_ERROR_TIMEOUT,
	HAL_I2CBITBANG_ERROR_TRANSACTION_ALREADY_RUNNING,
	HAL_I2CBITBANG_ERROR_MESSAGE_TO_LARGE,
	HAL_I2CBITBANG_ERROR_BAD_FREQUENCY,
	HAL_I2CBITBANG_ERROR_WHILE_SETTINGUP_GPIO,
	HAL_I2CBITBANG_ERROR_CLK_STRETCH,
} hal_i2cBitbang_error_t;


hal_i2cBitbang_error_t hal_i2cBitbang_Init( rpi_gpio_pin_t SCLPin, rpi_gpio_pin_t SDAPin );

hal_i2cBitbang_error_t hal_i2cBitbang_SendStart( void );
hal_i2cBitbang_error_t hal_i2cBitbang_SendStop( void );

hal_i2cBitbang_error_t hal_i2cBitbang_SendBit( uint8_t Bit, uint8_t ReleaseSdaAfter );
hal_i2cBitbang_error_t hal_i2cBitbang_ReceiveBit( uint8_t * Bit, uint8_t ReleaseSdaAfter );

hal_i2cBitbang_error_t hal_i2cBitbang_SendByte( uint8_t Byte );

hal_i2cBitbang_error_t hal_i2cBitbang_WriteTransaction( uint8_t SlaveAddress, uint8_t Data );

#endif /* I2CBITBANG_H_ */
