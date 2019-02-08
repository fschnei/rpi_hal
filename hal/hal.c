/*
 * hal.c
 *
 *  Created on: 11.01.2019
 *      Author: Fabian
 */

#include "hal.h"


void hal_Init(void)
{
	hal_gpio_SetInput( HAL_GPIO_PIN_5 );
	//hal_gpio_SetPinFunction( HAL_GPIO_PIN5, HAL_GPIO_FUNC_SEL_INPUT );


	// Initialise the UART
	hal_auxiliaries_MiniUartInit( 9600, 8 );



	hal_armTimer_Init(HAL_ARMTIMER_COUNTERWIDTH_23, HAL_ARMTIMER_PRESCALE_256, 0x4000);



	hal_bsc_Init_I2C1( 100000UL );


	hal_gpio_EnablePinInterrupt(HAL_GPIO_PIN_5, HAL_GPIO_DETECT_RISING);


	hal_interrupt_Init();


	// enable interrupts
	hal_interrupt_UnblockIrq();

}









