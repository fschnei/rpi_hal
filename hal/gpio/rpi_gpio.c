
#include "rpi_gpio.h"

// gpio access works via memory mapped io
// this is the base address for gpio pins
#define HAL_GPIO_BASE ( HAL_RPI_PERIPHERAL_BASE + 0x200000UL )

// a mask to be able to clear the bits in the register before setting the value we require
// see page 92 https://www.raspberrypi.org/documentation/hardware/raspberrypi/bcm2835/BCM2835-ARM-Peripherals.pdf
#define HAL_GPIO_FUNC_SEL_CLR_MASK (7)





static volatile hal_gpio_regs_t * const hal_gpio_regs = ( hal_gpio_regs_t * ) ( HAL_GPIO_BASE );


// provide pinter to gpio registers via function
// -> read only access to hal_gpio_regs
// -> no change of hal_gpio_regs by mistake
volatile hal_gpio_regs_t * const hal_gpio_GetRegs(void)
{
    return hal_gpio_regs;
}


hal_error_status_t hal_gpio_SetPullUpDown( rpi_gpio_pin_t GpioNo, hal_gpio_pullupdown_t PullMode )
{
	volatile hal_base_t i;
    hal_base_t index = GpioNo / 32;
	hal_base_t mask = ( 1 << ( GpioNo - ( 32 * index ) ) );

	if( GpioNo >= HAL_GPIO_PIN_LASTENTRY )
	{
		// invalid gpio pin
		return HAL_ERROR_GENERAL_ERROR;
	}

    // https://www.raspberrypi.org/documentation/hardware/raspberrypi/bcm2835/BCM2835-ARM-Peripherals.pdf
	// page 100
	if( PullMode == HAL_GPIO_PULLMODE_UP )
	{
		hal_gpio_regs->GPPUD[0] = ( 1 << 1 );
	}
	else if ( PullMode == HAL_GPIO_PULLMODE_DOWN )
	{
		hal_gpio_regs->GPPUD[0] = ( 1 << 0 );
	}
	else if ( PullMode == HAL_GPIO_PULLMODE_DISABLE )
	{
		hal_gpio_regs->GPPUD[0] = 0x00;
	}
	else
	{
		// invalid updown mode
		return HAL_ERROR_GENERAL_ERROR;
	}
    for( i = 0; i < 150; i++ )
    {
    	// nop
    }
    hal_gpio_regs->GPPUDCLK[index] = mask;
    for( i = 0; i < 150; i++ )
    {
    	// nop
    }
    hal_gpio_regs->GPPUDCLK[index] = 0;

	return HAL_ERROR_NO_ERROR;
}


hal_error_status_t hal_gpio_SetPinFunction( rpi_gpio_pin_t GpioNo, rpi_gpio_alt_function_t Function )
{
	hal_v_base_t gpfsel_Buffer;
    hal_base_t index = GpioNo / 10;
	hal_base_t shift = ( GpioNo % 10 ) * 3;

	if( GpioNo >= HAL_GPIO_PIN_LASTENTRY || Function >= HAL_GPIO_FUNCSEL_LASTENTRY )
	{
		// invalid gpio pin or invalid alternative function
		return HAL_ERROR_GENERAL_ERROR;
	}

	// get current gpfsel register
	gpfsel_Buffer = hal_gpio_regs->GPFSEL[index];
	// clear function select
	gpfsel_Buffer &= ~ ( HAL_GPIO_FUNC_SEL_CLR_MASK << shift );
	// set new function
	gpfsel_Buffer |= ( Function << shift );
	// write back buffer to gpfsel register
    hal_gpio_regs->GPFSEL[index] = gpfsel_Buffer;

    return HAL_ERROR_NO_ERROR;
}


hal_error_status_t hal_gpio_SetOutput( rpi_gpio_pin_t GpioNo )
{
	return hal_gpio_SetPinFunction( GpioNo, HAL_GPIO_FUNCSEL_OUTPUT );
}


hal_error_status_t hal_gpio_SetInput( rpi_gpio_pin_t GpioNo )
{
	return hal_gpio_SetPinFunction( GpioNo, HAL_GPIO_FUNCSEL_INPUT );
}


hal_error_status_t hal_gpio_GetValue( rpi_gpio_pin_t GpioNo, hal_gpio_level_t * Level )
{
	hal_gpio_level_t result;
    hal_base_t index = GpioNo / 32;
	hal_base_t mask = ( 1 << ( GpioNo - ( 32 * index ) ) );

	if( GpioNo >= HAL_GPIO_PIN_LASTENTRY )
	{
		// invalid gpio pin
		return HAL_ERROR_GENERAL_ERROR;
	}

    result = hal_gpio_regs->GPLEV[index] & mask;

    if ( result != 0 )
    {
        *Level = HAL_GPIO_LVL_HI;
    }
    else
    {
        *Level = HAL_GPIO_LVL_LO;
    }

    return HAL_ERROR_NO_ERROR;
}


hal_error_status_t hal_gpio_Toggle( rpi_gpio_pin_t GpioNo )
{
	hal_gpio_level_t level;
	if( GpioNo >= HAL_GPIO_PIN_LASTENTRY )
	{
		// invalid gpio pin
		return HAL_ERROR_GENERAL_ERROR;
	}

	if ( hal_gpio_GetValue( GpioNo, &level ) != HAL_ERROR_NO_ERROR )
	{
		return HAL_ERROR_GENERAL_ERROR;
	}

    if( level == HAL_GPIO_LVL_HI )
    {
    	return hal_gpio_SetLo( GpioNo );
    }
    else
    {
    	return hal_gpio_SetHi( GpioNo );
    }
}


hal_error_status_t hal_gpio_SetHi( rpi_gpio_pin_t GpioNo )
{
    hal_base_t index = GpioNo / 32;
	hal_base_t mask = ( 1 << ( GpioNo - ( 32 * index ) ) );

	if( GpioNo >= HAL_GPIO_PIN_LASTENTRY )
	{
		// invalid gpio pin
		return HAL_ERROR_GENERAL_ERROR;
	}

	hal_gpio_regs->GPSET[index] = mask;

    return HAL_ERROR_NO_ERROR;
}


hal_error_status_t hal_gpio_SetLo( rpi_gpio_pin_t GpioNo )
{
    hal_base_t index = GpioNo / 32;
	hal_base_t mask = ( 1 << ( GpioNo - ( 32 * index ) ) );

	if( GpioNo >= HAL_GPIO_PIN_LASTENTRY )
	{
		// invalid gpio pin
		return HAL_ERROR_GENERAL_ERROR;
	}

	hal_gpio_regs->GPCLR[index] = mask;

    return HAL_ERROR_NO_ERROR;
}


hal_error_status_t hal_gpio_SetValue( rpi_gpio_pin_t GpioNo, hal_gpio_level_t Value )
{
	if( GpioNo >= HAL_GPIO_PIN_LASTENTRY || Value >= HAL_GPIO_LVL_LASTENTRY )
	{
		// invalid gpio pin or invalid value
		return HAL_ERROR_GENERAL_ERROR;
	}
    if( Value == HAL_GPIO_LVL_LO )
    {
    	return hal_gpio_SetLo( GpioNo );
    }
    else if( Value == HAL_GPIO_LVL_HI )
    {
    	return hal_gpio_SetHi( GpioNo );
    }
	return HAL_ERROR_GENERAL_ERROR;
}


hal_error_status_t hal_gpio_EnablePinInterrupt(rpi_gpio_pin_t GpioNo, hal_gpio_detectMode_t DetectMode)
{
	hal_base_t index = GpioNo / 32;
	hal_base_t mask = ( 1 << ( GpioNo - ( 32 * index ) ) );

	if( GpioNo >= HAL_GPIO_PIN_LASTENTRY || DetectMode >= HAL_GPIO_DETECT_LASTENTRY )
	{
		// invalid gpio pin or invalid detect mode
		return HAL_ERROR_GENERAL_ERROR;
	}

	switch( DetectMode )
	{
	case HAL_GPIO_DETECT_RISING:
		hal_gpio_regs->GPREN[index] |= mask;
		break;
	case HAL_GPIO_DETECT_FALLING:
		hal_gpio_regs->GPFEN[index] |= mask;
		break;
	case HAL_GPIO_DETECT_HIGH:
		hal_gpio_regs->GPHEN[index] |= mask;
		break;
	case HAL_GPIO_DETECT_LOW:
		hal_gpio_regs->GPLEN[index] |= mask;
		break;
	case HAL_GPIO_DETECT_RISING_ASYNC:
		hal_gpio_regs->GPAREN[index] |= mask;
		break;
	case HAL_GPIO_DETECT_FALLING_ASYNC:
		hal_gpio_regs->GPAFEN[index] |= mask;
		break;
	case HAL_GPIO_DETECT_ALL:
		// set all detect register
		hal_gpio_regs->GPREN[index] |= mask;
		hal_gpio_regs->GPFEN[index] |= mask;
		hal_gpio_regs->GPHEN[index] |= mask;
		hal_gpio_regs->GPLEN[index] |= mask;
		hal_gpio_regs->GPAREN[index] |= mask;
		hal_gpio_regs->GPAFEN[index] |= mask;
		break;
	}
	return HAL_ERROR_NO_ERROR;
}


hal_error_status_t hal_gpio_DisablePinInterrupt(rpi_gpio_pin_t GpioNo, hal_gpio_detectMode_t DetectMode)
{
	hal_base_t index = GpioNo / 32;
	hal_base_t mask = ~ ( 1 << ( GpioNo - ( 32 * index ) ) );

	if( GpioNo >= HAL_GPIO_PIN_LASTENTRY || DetectMode >= HAL_GPIO_DETECT_LASTENTRY )
	{
		// invalid gpio pin or invalid detect mode
		return HAL_ERROR_GENERAL_ERROR;
	}

	switch( DetectMode )
	{
	case HAL_GPIO_DETECT_RISING:
		hal_gpio_regs->GPREN[index] &= mask;
		break;
	case HAL_GPIO_DETECT_FALLING:
		hal_gpio_regs->GPFEN[index] &= mask;
		break;
	case HAL_GPIO_DETECT_HIGH:
		hal_gpio_regs->GPHEN[index] &= mask;
		break;
	case HAL_GPIO_DETECT_LOW:
		hal_gpio_regs->GPLEN[index] &= mask;
		break;
	case HAL_GPIO_DETECT_RISING_ASYNC:
		hal_gpio_regs->GPAREN[index] &= mask;
		break;
	case HAL_GPIO_DETECT_FALLING_ASYNC:
		hal_gpio_regs->GPAFEN[index] &= mask;
		break;
	case HAL_GPIO_DETECT_ALL:
		// clear all detect register
		hal_gpio_regs->GPREN[index] &= mask;
		hal_gpio_regs->GPFEN[index] &= mask;
		hal_gpio_regs->GPHEN[index] &= mask;
		hal_gpio_regs->GPLEN[index] &= mask;
		hal_gpio_regs->GPAREN[index] &= mask;
		hal_gpio_regs->GPAFEN[index] &= mask;
		break;
	}
	return HAL_ERROR_NO_ERROR;
}


hal_error_status_t hal_gpio_ClearInterrupt(rpi_gpio_pin_t GpioNo)
{
	hal_base_t index = GpioNo / 32;
	hal_base_t mask = ( 1 << ( GpioNo - ( 32 * index ) ) );

	if( GpioNo >= HAL_GPIO_PIN_LASTENTRY )
	{
		// invalid gpio pin
		return HAL_ERROR_GENERAL_ERROR;
	}

	hal_gpio_regs->GPEDS[index] = mask;

	return HAL_ERROR_NO_ERROR;
}



