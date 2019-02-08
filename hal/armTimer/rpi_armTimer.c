
#include "rpi_armTimer.h"

// see section 14 page 196
#define HAL_ARMTIMER_BASE               ( HAL_RPI_PERIPHERAL_BASE + 0xB400UL )

#define HAL_ARMTIMER_CTRL_WIDTH			( 1 << 1 )

#define HAL_ARMTIMER_CTRL_PRESCALE	( 1 << 2 )
#define HAL_ARMTIMER_CTRL_PRESCALE1		( 0 << 2 )
#define HAL_ARMTIMER_CTRL_PRESCALE16	( 1 << 2 )
#define HAL_ARMTIMER_CTRL_PRESCALE256	( 2 << 2 )

#define HAL_ARMTIMER_CTRL_INTENABLE		( 1 << 5 )

#define HAL_ARMTIMER_CTRL_ENABLE		( 1 << 7 )





static volatile hal_armTimer_regs_t * rpiArmTimer = ( hal_armTimer_regs_t * ) ( HAL_ARMTIMER_BASE );


// if possible use functions instead of direct register access via hal_armTimer_regs_t
volatile hal_armTimer_regs_t * hal_armTimer_GetRegs( void )
{
    return rpiArmTimer;
}


hal_error_status_t hal_armTimer_Init( hal_armTimer_counterWidth_r Width, hal_armTimer_prescale_r Prescale, hal_base_t Load )
{
	hal_base_t Control_Buffer = 0x0000;

	if ( Width >= HAL_ARMTIMER_COUNTERWIDTH_LASTENTRY || Prescale >= HAL_ARMTIMER_PRESCALE_LASTENTRY )
	{
		return HAL_ERROR_GENERAL_ERROR;
	}

	// TODO: calculate control register content out of frequency

	// timer frequency = systemClk / prescaler * load
	rpiArmTimer->Load = Load;

	// setup control register
	switch ( Width )
	{
	case HAL_ARMTIMER_COUNTERWIDTH_16:
		Control_Buffer &= ~HAL_ARMTIMER_CTRL_WIDTH;
		break;
	case HAL_ARMTIMER_COUNTERWIDTH_23:
		Control_Buffer |= HAL_ARMTIMER_CTRL_WIDTH;
		break;
	}

	Control_Buffer &= ~HAL_ARMTIMER_CTRL_PRESCALE;
	switch ( Prescale )
	{
	case HAL_ARMTIMER_PRESCALE_1:
	Control_Buffer |= HAL_ARMTIMER_CTRL_PRESCALE1;
		break;
	case HAL_ARMTIMER_PRESCALE_16:
		Control_Buffer |= HAL_ARMTIMER_CTRL_PRESCALE16;
		break;
	case HAL_ARMTIMER_PRESCALE_256:
		Control_Buffer |= HAL_ARMTIMER_CTRL_PRESCALE256;
		break;
	}

	Control_Buffer |= HAL_ARMTIMER_CTRL_INTENABLE;
	Control_Buffer |= HAL_ARMTIMER_CTRL_ENABLE;

	// write to hardware register
	rpiArmTimer->Control = Control_Buffer;

	return HAL_ERROR_NO_ERROR;
}


void hal_armTimer_ClearIrq( void )
{
	rpiArmTimer->IrqClear = 1;
}

