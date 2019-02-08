
#ifndef RPI_AUX_H
#define RPI_AUX_H

#include "../rpi_base.h"


// TODO: check this stuff (register ordering, count of reserved bytes)
typedef struct {
	hal_reg_r_t Irq;				// 0xXX215000
	hal_reg_rw_t Enable;			// 0xXX215004

    // create offset
	hal_reg_rw_t reserved1[((0x40 - 0x04) / 4) - 1];	// 0xXX215008 - 0xXX21503C

	// mini uart
    hal_reg_rw_t MiniUart_Io;		// 0xXX215040
    hal_reg_rw_t MiniUart_IER;		// 0xXX215044	// http://elinux.org/BCM2835_datasheet_errata
    hal_reg_rw_t MiniUart_IIR;		// 0xXX215048	// http://elinux.org/BCM2835_datasheet_errata
    hal_reg_rw_t MiniUart_LCR;		// 0xXX21504C
    hal_reg_rw_t MiniUart_MCR;		// 0xXX215050
    hal_reg_r_t MiniUart_LSR;		// 0xXX215054
    hal_reg_r_t MiniUart_MSR;		// 0xXX215058
    hal_reg_rw_t MiniUart_SCRATCH;	// 0xXX21505C
    hal_reg_rw_t MiniUart_CNTL;		// 0xXX215060
    hal_reg_r_t MiniUart_STAT;		// 0xXX215064
    hal_reg_rw_t MiniUart_BAUD;		// 0xXX215068

    // create offset
    hal_reg_rw_t reserved2[(0x80 - 0x68) / 4];			// 0xXX21506C - 0xXX21507C

    // SPI 0
    hal_reg_rw_t SPI1_CNTL0;		// 0xXX215080
    hal_reg_rw_t SPI1_CNTL1;		// 0xXX215084
    hal_reg_rw_t SPI1_STAT;			// 0xXX215088
    hal_reg_rw_t reserved2_;		// 0xXX21508C
    hal_reg_r_t SPI1_IO;			// 0xXX215090 or 0xXX21508C ? // http://elinux.org/BCM2835_datasheet_errata
    hal_reg_rw_t SPI1_PEEK;			// 0xXX215094 or 0xXX215090 ? // http://elinux.org/BCM2835_datasheet_errata

    // create offset
    hal_reg_rw_t reserved3[(0xC0 - 0x94) / 4];			// 0xXX2150A4 - 0xXX2150BC

    // SPI 1
    hal_reg_rw_t SPI2_CNTL0;		// 0xXX2150C0
    hal_reg_rw_t SPI2_CNTL1;		// 0xXX2150C4
    hal_reg_rw_t SPI2_STAT;			// 0xXX2150C8
    hal_reg_rw_t reserved3_;		// 0xXX2150CC
    hal_reg_r_t SPI2_IO;			// 0xXX2150D0 or 0xXX2150CC ? // http://elinux.org/BCM2835_datasheet_errata
    hal_reg_rw_t SPI2_PEEK;			// 0xXX2150D4 or 0xXX2150D0 ? // http://elinux.org/BCM2835_datasheet_errata
} hal_auxiliaries_regs_t;


hal_auxiliaries_regs_t * hal_auxiliaries_GetRegs( void );

void hal_auxiliaries_MiniUartInit( hal_base_t Baud, hal_base_t Bits );

void hal_auxiliaries_MiniUartWrite( char c );


#endif // RPI_AUX_H
