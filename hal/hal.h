/*
 * hal.h
 *
 *  Created on: 11.01.2019
 *      Author: Fabian
 */

#ifndef _HAL_H_
#define _HAL_H_


#include "armTimer/rpi_armTimer.h"
#include "auxiliaries/rpi_aux.h"
#include "gpio/rpi_gpio.h"
#include "interrupt/rpi_interrupts.h"
#include "bsc/rpi_bsc.h"

#include "i2cBitbang/i2cBitbang.h"
#include "sysTimer/rpi_sysTimer.h"

void hal_Init(void);


#endif /* _HAL_H_ */
