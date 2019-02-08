/*
 * rpi_interrups.h
 *
 *  Created on: 11.01.2019
 *      Author: Fabian
 */

#ifndef _RPI_INTERRUPS_H_
#define _RPI_INTERRUPS_H_

#include "../rpi_base.h"


// page 112 https://www.raspberrypi.org/documentation/hardware/raspberrypi/bcm2835/BCM2835-ARM-Peripherals.pdf
// pending register indicated all other interrupts which are not shown by basicPending register
typedef struct {
	hal_reg_r_t		IRQBasicPending;	// Pending (2) basic: Bit8->pending1 interrupt ; bit9->pending2 interrupt
	hal_reg_r_t		IRQPending1;		// Pending 0
	hal_reg_r_t		IRQPending2;		// Pending 1
	hal_reg_rw_t	FIQCtrl;			// fast interrupt selection
	hal_reg_rw_t	EnableIRQs1;		// Enable 0
	hal_reg_rw_t	EnableIRQs2;		// Enable 1
	hal_reg_rw_t	EnableBasicIRQs;	// Enable (2) basic interrupts
	hal_reg_rw_t	DisableIRQs1;		// Disable 0
	hal_reg_rw_t	DisableIRQs2;		// Disable 1
	hal_reg_rw_t	DisableBasicIRQs;	// Disable (2) basic interrupts
} hal_interrupt_regs_t;

//
typedef struct {
	uint32_t Reserved;
} hal_interrupt_parms_t;

typedef enum {
	HAL_INTERRUPT_ID_AUX 				= 29,
	// GPU interrupt, do not enable
	HAL_INTERRUPT_ID_SPI_SLAVE			= 43,
	// GPU interrupt, do not enable
	HAL_INTERRUPT_ID_PWA0				= 45,
	HAL_INTERRUPT_ID_PWA1 				= 46,
	// GPU interrupt, do not enable
	HAL_INTERRUPT_ID_SMI 				= 48,
	HAL_INTERRUPT_ID_GPIO_0 			= 49,
	HAL_INTERRUPT_ID_GPIO_1 			= 50,
	HAL_INTERRUPT_ID_GPIO_2 			= 51,
	HAL_INTERRUPT_ID_GPIO_3 			= 52,
	HAL_INTERRUPT_ID_I2C 				= 53,
	HAL_INTERRUPT_ID_SPI 				= 54,
	HAL_INTERRUPT_ID_PCM				= 55,
	// GPU interrupt, do not enable
	HAL_INTERRUPT_ID_UART				= 57,
	// end of pending register interrupts
	// now direct irg basic register interrupts
	HAL_INTERRUPT_ID_TIMER_0			= 64,
	HAL_INTERRUPT_ID_MAILBOX_0			= 65,
	HAL_INTERRUPT_ID_DOORBELL_0			= 66,
	HAL_INTERRUPT_ID_DOORBELL_1			= 67,
	HAL_INTERRUPT_ID_GPU0_HALTED		= 68,
	HAL_INTERRUPT_ID_GPU1_HALTED		= 69,
	HAL_INTERRUPT_ID_ILLEGAL_ACCESS_1	= 70,
	HAL_INTERRUPT_ID_ILLEGAL_ACCESS_0	= 71,
	HAL_INTERRUPT_ID_lASTENTRY,
} hal_interrupt_source_t;


// create function pointer type for interrupt service routine callbacks
// every isr function should be this type
typedef void ( * hal_interrupt_isr_t ) ( hal_interrupt_source_t irq, void * pParam );


volatile hal_interrupt_regs_t * hal_interrupt_GetRegs( void );

hal_error_status_t hal_interrupt_Init( void );

hal_error_status_t hal_interrupt_RegisterAndEnableIsr ( const hal_interrupt_source_t Irq, hal_interrupt_isr_t pfnHandler, void * pParam );
hal_error_status_t hal_interrupt_DisableIrq			( const hal_interrupt_source_t Irq );
void hal_interrupt_BlockIrq				( void );
void hal_interrupt_UnblockIrq			( void );



#endif /* _RPI_INTERRUPS_H_ */
