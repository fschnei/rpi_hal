
#include "../sysTimer/rpi_sysTimer.h"


#define HAL_SYSTIMER_BASE       ( HAL_RPI_PERIPHERAL_BASE + 0x3000UL )



static volatile hal_sysTimer_regs_t * const hal_sysTimer_regs = ( hal_sysTimer_regs_t * ) HAL_SYSTIMER_BASE;


volatile hal_sysTimer_regs_t * hal_sysTimer_GetRegs( void )
{
    return hal_sysTimer_regs;
}

void hal_sysTimer_WaitMicroSeconds( hal_base_t MicroSeconds )
{
	hal_v_base_t counterValue = hal_sysTimer_regs->counter_lo;

    while( ( hal_sysTimer_regs->counter_lo - counterValue ) < MicroSeconds )
    {
        // nop
    }
}
