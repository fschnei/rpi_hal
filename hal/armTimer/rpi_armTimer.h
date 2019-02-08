
#ifndef RPI_ARMTIMER_H
#define RPI_ARMTIMER_H

#include "../rpi_base.h"


typedef struct{
	hal_reg_rw_t 	Load;
	hal_reg_r_t 	Value;
    hal_reg_rw_t 	Control;
    hal_reg_w_t 	IrqClear;
    hal_reg_r_t 	RAWIRQ;
    hal_reg_r_t 	MaskedIrq;
    hal_reg_rw_t 	Reload;
    hal_reg_rw_t 	PreDivider;
    hal_reg_r_t 	FreeRunningCounter;
} hal_armTimer_regs_t;

typedef enum
{
	HAL_ARMTIMER_PRESCALE_1,
	HAL_ARMTIMER_PRESCALE_16,
	HAL_ARMTIMER_PRESCALE_256,
	HAL_ARMTIMER_PRESCALE_LASTENTRY,
} hal_armTimer_prescale_r;

typedef enum
{
	HAL_ARMTIMER_COUNTERWIDTH_16,
	HAL_ARMTIMER_COUNTERWIDTH_23,
	HAL_ARMTIMER_COUNTERWIDTH_LASTENTRY,
} hal_armTimer_counterWidth_r;


volatile hal_armTimer_regs_t * hal_armTimer_GetRegs( void );

hal_error_status_t hal_armTimer_Init( hal_armTimer_counterWidth_r Width, hal_armTimer_prescale_r Prescale, hal_base_t Load );

void hal_armTimer_ClearIrq( void );



#endif
