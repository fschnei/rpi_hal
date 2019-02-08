
#include "../gpio/rpi_gpio.h"
#include "rpi_aux.h"

// !!! errata:
// http://elinux.org/BCM2835_datasheet_errata
#define HAL_AUXILIARIES_BASE    	( HAL_RPI_PERIPHERAL_BASE + 0x215000 )


// enable register
#define HAL_AUXILIARIES_ENABLE_MINIUART 			( 1 << 0 )
#define HAL_AUXILIARIES_ENABLE_SPI1 				( 1 << 1 )
#define HAL_AUXILIARIES_ENABLE_SPI2 				( 1 << 2 )
// IRQ register
#define HAL_AUXILIARIES_IRQ_MINIUART 				( 1 << 0 )
#define HAL_AUXILIARIES_IRQ_SPI1 					( 1 << 1 )
#define HAL_AUXILIARIES_IRQ_SPI2 					( 1 << 2 )
// mini uart IO (...) register
#define HAL_AUXILIARIES_MINIUART_IO_DATA			( 0xFF << 0 )
// mini uart IER (...) register
#define HAL_AUXILIARIES_MINIUART_IER_RX_ENABLE		( 0x0B << 0 )  // see errata, 1101b = 0x0B = 11
#define HAL_AUXILIARIES_MINIUART_IER_TX_ENABLE		( 1 << 1 )  // see errata
// mini uart IIR (...) register
#define HAL_AUXILIARIES_MINIUART_IIR_IRQ_PENDING	( 1 << 0 )
#define HAL_AUXILIARIES_MINIUART_IIR_IRQ_ID			( 3 << 1 )
#define HAL_AUXILIARIES_MINIUART_IIR_FIFO_CLR		( 3 << 1 )
// mini uart LCR (line control) register
#define HAL_AUXILIARIES_MINIUART_LCR_DATASIZE_8		( 3 << 0 )  // see errata
#define HAL_AUXILIARIES_MINIUART_LCR_BREAK			( 1 << 6 )
#define HAL_AUXILIARIES_MINIUART_LCR_DLAB_ACCESS	( 1 << 7 )
// mini uart MCR (modem control) register
#define HAL_AUXILIARIES_MINIUART_MCR_RTS			( 1 << 1 )
// mini uart LSR (line status) register
#define HAL_AUXILIARIES_MINIUART_LSR_DATA_READY		( 1 << 0 )
#define HAL_AUXILIARIES_MINIUART_LSR_RX_OVERRUN		( 1 << 1 )
#define HAL_AUXILIARIES_MINIUART_LSR_TX_EMPTY		( 1 << 5 )
#define HAL_AUXILIARIES_MINIUART_LSR_TX_IDLE		( 1 << 6 )
// mini uart MSR (modem status) register
#define HAL_AUXILIARIES_MINIUART_MSR_CTS			( 1 << 5 )
// mini uart CNTL (extra control) register
#define HAL_AUXILIARIES_MINIUART_CNTL_RX_ENABLE		( 1 << 0 )
#define HAL_AUXILIARIES_MINIUART_CNTL_TX_ENABLE		( 1 << 1 )
#define HAL_AUXILIARIES_MINIUART_CNTL_RTS_FLOW		( 1 << 2 )
#define HAL_AUXILIARIES_MINIUART_CNTL_CTS_FLOW		( 1 << 3 )
#define HAL_AUXILIARIES_MINIUART_CNTL_RTS_FIFO		( 3 << 4 )
#define HAL_AUXILIARIES_MINIUART_CNTL_RTS_ASSERT	( 1 << 6 )
#define HAL_AUXILIARIES_MINIUART_CNTL_CTS_ASSERT	( 1 << 7 )
// mini uart STAT (extra status) register
#define HAL_AUXILIARIES_MINIUART_STAT_SYMBOL_AV		( 1 << 0 )
#define HAL_AUXILIARIES_MINIUART_STAT_SPACE_AV		( 1 << 1 )
#define HAL_AUXILIARIES_MINIUART_STAT_RX_IDLE		( 1 << 2 )
#define HAL_AUXILIARIES_MINIUART_STAT_TX_IDLE		( 1 << 3 )
#define HAL_AUXILIARIES_MINIUART_STAT_RX_OVERRUN	( 1 << 4 )
#define HAL_AUXILIARIES_MINIUART_STAT_TX_FIFO_FULL	( 1 << 5 )
#define HAL_AUXILIARIES_MINIUART_STAT_RTS			( 1 << 6 )
#define HAL_AUXILIARIES_MINIUART_STAT_CTS			( 1 << 7 )
#define HAL_AUXILIARIES_MINIUART_STAT_TX_EMPTY		( 1 << 8 )
#define HAL_AUXILIARIES_MINIUART_STAT_TX_DONE		( 1 << 9 )
#define HAL_AUXILIARIES_MINIUART_STAT_RX_FIFO_LEVEL	( 7 << 16 )
#define HAL_AUXILIARIES_MINIUART_STAT_TX_FIFO_LEVEL	( 7 << 24 )





static hal_auxiliaries_regs_t * hal_auxillary_regs = (hal_auxiliaries_regs_t *)HAL_AUXILIARIES_BASE;


hal_auxiliaries_regs_t * hal_auxiliaries_GetRegs( void )
{
    return hal_auxillary_regs;
}


void hal_auxiliaries_MiniUartInit( hal_base_t Baud, hal_base_t Bits )
{
    volatile int i;

    // set enable bit to enable mini uart (also register rw access)
    hal_auxillary_regs->Enable = HAL_AUXILIARIES_ENABLE_MINIUART;

    // Disable interrupts for now
    // auxillary->IRQ &= ~AUX_IRQ_MU;

    // diable interrupts
    hal_auxillary_regs->MiniUart_IER = 0;

    // disable flow control, RTS, CTS, TX, RX ...
    hal_auxillary_regs->MiniUart_CNTL = 0;

    // activate 8 or 7 bit mode
    if( Bits == 8 )
    {
    	hal_auxillary_regs->MiniUart_LCR = HAL_AUXILIARIES_MINIUART_LCR_DATASIZE_8;
    }
    else
    {
    	hal_auxillary_regs->MiniUart_LCR = 0;
    }

    // set rts line high
    hal_auxillary_regs->MiniUart_MCR = 0;

    // disable full, empty interrupts for tx and rx fifos
    hal_auxillary_regs->MiniUart_IER = 0;

    //
    hal_auxillary_regs->MiniUart_IIR = HAL_AUXILIARIES_MINIUART_IIR_FIFO_CLR;	//0xC6;

    // set baud rate
    hal_auxillary_regs->MiniUart_BAUD = ( HAL_RPI_CORE_CLOCK_FREQUENCY / ( 8 * Baud ) ) - 1;

    // set rx and tx pin to corresponding alternative function
    hal_gpio_SetPinFunction( HAL_GPIO_PIN_14, HAL_GPIO_FUNCSEL_ALT5 );
    hal_gpio_SetPinFunction( HAL_GPIO_PIN_15, HAL_GPIO_FUNCSEL_ALT5 );

    // set pullup for gpios
    volatile hal_base_t test = hal_gpio_SetPullUpDown( HAL_GPIO_PIN_14, HAL_GPIO_PULLMODE_UP );

    // enable mini uart tx
    hal_auxillary_regs->MiniUart_CNTL = HAL_AUXILIARIES_MINIUART_CNTL_TX_ENABLE;
}


void hal_auxiliaries_MiniUartWrite( char c )
{
    while( ( hal_auxillary_regs->MiniUart_LSR & HAL_AUXILIARIES_MINIUART_LSR_TX_EMPTY ) == 0 )
    {
    	// nop
    }

    hal_auxillary_regs->MiniUart_Io = c;
}




