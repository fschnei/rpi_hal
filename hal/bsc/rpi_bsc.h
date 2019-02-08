/*
 * rpi_bsc.h
 *
 *  Created on: 04.02.2019
 *      Author: Fabian
 */

#ifndef RPI_BSC_H_
#define RPI_BSC_H_

#include "../rpi_base.h"


typedef struct{
	hal_reg_rw_t 	Control;
	hal_reg_rw_t 	Status;
    hal_reg_rw_t 	DataLength;
    hal_reg_rw_t 	SlaveAddress;
    hal_reg_rw_t 	DataFifo;
    hal_reg_rw_t 	ClockDivider;
    hal_reg_rw_t 	DataDelay;			// Number of core clock cycles to wait after the rising/falling edge of SCL before reading the next bit of data
    hal_reg_rw_t 	ClkStretchTimeout;	// Number of SCL clock cycles to wait after the rising edge of SCL before deciding that the slave is not responding
} hal_bsc_regs_t;


typedef enum {
	HAL_BSC_ERROR_NO_ERROR = 0,
	HAL_BSC_ERROR_FIFO_EMPTY,
	HAL_BSC_ERROR_NO_SLAVE_ACKNOWLEDGE,
	HAL_BSC_ERROR_TIMEOUT,
	HAL_BSC_ERROR_TRANSACTION_ALREADY_RUNNING,
	HAL_BSC_ERROR_MESSAGE_TO_LARGE,
	HAL_BSC_ERROR_BAD_FREQUENCY,
	HAL_BSC_ERROR_WHILE_SETTINGUP_GPIO,
} hal_bsc_error_t;



hal_bsc_error_t hal_bsc_Init_I2C0( hal_base_t I2CFrequency );

hal_bsc_error_t hal_bsc_WriteTransaction_I2C0(
		const uint8_t SlaveAddress,
		uint8_t * Buffer,
		uint8_t BufferSize );

hal_bsc_error_t hal_bsc_ReadTransaction_I2C0(
		const uint8_t SlaveAddress,
		uint8_t * Buffer,
		uint8_t BufferSize );

hal_bsc_error_t hal_bsc_Init_I2C1( hal_base_t I2CFrequency );

hal_bsc_error_t hal_bsc_WriteTransaction_I2C1(
		const uint8_t SlaveAddress,
		uint8_t * Buffer,
		uint8_t BufferSize );

hal_bsc_error_t hal_bsc_ReadTransaction_I2C1(
		const uint8_t SlaveAddress,
		uint8_t * Buffer,
		uint8_t BufferSize );


#endif // RPI_BSC_H_
