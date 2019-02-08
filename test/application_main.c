
#include "hal/hal.h"




void application_main()
{
	uint8_t Buffer[2];
	uint8_t receiveBit;
	volatile hal_bsc_error_t ret;
	uint8_t slaveAdr;
	slaveAdr = 0;

	//hal_Init();

	hal_i2cBitbang_Init( HAL_GPIO_PIN_3, HAL_GPIO_PIN_2 );

    while( 1 )
    {
    	/*slaveAdr++;
    	if ( slaveAdr > 0x27 )
    	{
    		slaveAdr = 0x20;
    	}
    	ret = hal_bsc_ReadTransaction_I2C1( slaveAdr, Buffer, 16 );

    	if ( ret == HAL_ERROR_NO_ERROR )
    	{
    		//slaveAdr = 0;
    		ret = 1;
    	}*/

/*
    	Buffer[0] = 0xAA;
    	Buffer[1] = 0xAA;
    	ret = hal_bsc_WriteTransaction_I2C1( 0x20, Buffer, sizeof(Buffer) );*/



/*
    	hal_i2cBitbang_SendStart();
    	hal_i2cBitbang_SendByte( ( 0x20 << 1 ) & 0xFE );
    	hal_i2cBitbang_SendStop();*/

    	hal_i2cBitbang_error_t ret = hal_i2cBitbang_WriteTransaction( 0x20, 0xAA );

    	//hal_i2cBitbang_SendByte( 0x40 );

/*
    	hal_i2cBitbang_SendStart();

    	hal_i2cBitbang_SendBit( 0x00, 1 );
    	hal_i2cBitbang_SendBit( 0x01, 1 );
    	hal_i2cBitbang_SendBit( 0x00, 1 );
    	hal_i2cBitbang_SendBit( 0x00, 1 );
    	hal_i2cBitbang_SendBit( 0x00, 1 );
    	hal_i2cBitbang_SendBit( 0x00, 1 );
    	hal_i2cBitbang_SendBit( 0x00, 1 );
    	hal_i2cBitbang_SendBit( 0x01, 1 );

    	hal_i2cBitbang_ReceiveBit( &receiveBit, 0 );

    	hal_i2cBitbang_SendStop();*/



    	/*hal_gpio_SetInput(HAL_GPIO_PIN_2);

    	hal_gpio_SetHi(HAL_GPIO_PIN_2);

    	hal_gpio_SetOutput(HAL_GPIO_PIN_2);

    	hal_gpio_SetInput(HAL_GPIO_PIN_2);

    	hal_gpio_SetLo(HAL_GPIO_PIN_2);

    	hal_gpio_SetOutput(HAL_GPIO_PIN_2);*/



    	//LED_ON();
    	//RPI_SetGpioLo( RPI_GPIO5 );
    	hal_sysTimer_WaitMicroSeconds( 10000 );
    	//LED_OFF();
    	//RPI_SetGpioHi( RPI_GPIO5 );
    	//RPI_WaitMicroSeconds( 1000000 );

    	//hal_auxiliaries_MiniUartWrite( 'A' );


    }
}
