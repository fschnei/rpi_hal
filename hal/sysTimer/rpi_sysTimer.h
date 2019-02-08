
#ifndef RPI_SYSTIMER_H
#define RPI_SYSTIMER_H

#include "../rpi_base.h"

typedef struct {
	hal_reg_rw_t control_status;
	hal_reg_rw_t counter_lo;
	hal_reg_rw_t counter_hi;
	hal_reg_rw_t compare0;
	hal_reg_rw_t compare1;
	hal_reg_rw_t compare2;
	hal_reg_rw_t compare3;
} hal_sysTimer_regs_t;



volatile hal_sysTimer_regs_t * hal_sysTimer_GetRegs( void );

void hal_sysTimer_WaitMicroSeconds( hal_base_t MicroSeconds );


#endif
