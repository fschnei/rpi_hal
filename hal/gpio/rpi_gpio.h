
#ifndef RPI_GPIO_H
#define RPI_GPIO_H

#include "../rpi_base.h"


// see page 90 https://www.raspberrypi.org/documentation/hardware/raspberrypi/bcm2835/BCM2835-ARM-Peripherals.pdf
typedef struct {
	hal_reg_rw_t	GPFSEL[6];		// Function Select
	hal_reg_r_t	Reserved_1;
	hal_reg_w_t	GPSET[2];		// Pin Output Set
	hal_reg_r_t	Reserved_2;
	hal_reg_w_t	GPCLR[2];		// Pin Output Clear
	hal_reg_r_t	Reserved_3;
	hal_reg_w_t	GPLEV[2];		// Pin Level
	hal_reg_r_t	Reserved_4;
	hal_reg_w_t	GPEDS[2];		// Pin Event Detect Status
	hal_reg_r_t	Reserved_5;
	hal_reg_w_t	GPREN[2];		// Pin Rising Edge Detect Enable
	hal_reg_r_t	Reserved_6;
	hal_reg_w_t	GPFEN[2];		// Pin Falling Edge Detect Enable
	hal_reg_r_t	Reserved_7;
	hal_reg_w_t	GPHEN[2];		// Pin High Detect Enable
	hal_reg_r_t	Reserved_8;
	hal_reg_w_t	GPLEN[2];		// Pin Low Detect Enable
	hal_reg_r_t	Reserved_9;
	hal_reg_w_t	GPAREN[2];		// Pin Async. Rising Edge Detect
	hal_reg_r_t	Reserved_A;
	hal_reg_w_t	GPAFEN[2];		// Pin Async. Falling Edge Detect
	hal_reg_r_t	Reserved_B;
	hal_reg_w_t	GPPUD[1];		// Pin Pull-up/down Enable
	hal_reg_w_t	GPPUDCLK[2];	// Pin Pull-up/down Enable Clock
	hal_reg_r_t	Reserved_C;
} hal_gpio_regs_t;

// TODO: PIN is not a good name, this can confuse with pin number on p1 header
typedef enum {
    HAL_GPIO_PIN_0 = 0,
	HAL_GPIO_PIN_1,
    HAL_GPIO_PIN_2,
    HAL_GPIO_PIN_3,
    HAL_GPIO_PIN_4,
    HAL_GPIO_PIN_5,
    HAL_GPIO_PIN_6,
    HAL_GPIO_PIN_7,
    HAL_GPIO_PIN_8,
    HAL_GPIO_PIN_9,
    HAL_GPIO_PIN_10 = 10,
    HAL_GPIO_PIN_11,
    HAL_GPIO_PIN_12,
    HAL_GPIO_PIN_13,
    HAL_GPIO_PIN_14,
    HAL_GPIO_PIN_15,
    HAL_GPIO_PIN_16,
    HAL_GPIO_PIN_17,
    HAL_GPIO_PIN_18,
    HAL_GPIO_PIN_19,
    HAL_GPIO_PIN_20 = 20,
    HAL_GPIO_PIN_21,
    HAL_GPIO_PIN_22,
    HAL_GPIO_PIN_23,
    HAL_GPIO_PIN_24,
    HAL_GPIO_PIN_25,
    HAL_GPIO_PIN_26,
    HAL_GPIO_PIN_27,
    HAL_GPIO_PIN_28,
    HAL_GPIO_PIN_29,
    HAL_GPIO_PIN_30 = 30,
    HAL_GPIO_PIN_31,
    HAL_GPIO_PIN_32,
    HAL_GPIO_PIN_33,
    HAL_GPIO_PIN_34,
    HAL_GPIO_PIN_35,
    HAL_GPIO_PIN_36,
    HAL_GPIO_PIN_37,
    HAL_GPIO_PIN_38,
    HAL_GPIO_PIN_39,
    HAL_GPIO_PIN_40 = 40,
    HAL_GPIO_PIN_41,
    HAL_GPIO_PIN_42,
    HAL_GPIO_PIN_43,
    HAL_GPIO_PIN_44,
    HAL_GPIO_PIN_45,
    HAL_GPIO_PIN_46,
    HAL_GPIO_PIN_47,
    HAL_GPIO_PIN_48,
    HAL_GPIO_PIN_49,
    HAL_GPIO_PIN_50 = 50,
    HAL_GPIO_PIN_51,
    HAL_GPIO_PIN_52,
    HAL_GPIO_PIN_53,
	HAL_GPIO_PIN_LASTENTRY,
} rpi_gpio_pin_t;

typedef enum {
	HAL_GPIO_FUNCSEL_INPUT = 0,
	HAL_GPIO_FUNCSEL_OUTPUT,
	HAL_GPIO_FUNCSEL_ALT5,
	HAL_GPIO_FUNCSEL_ALT4,
	HAL_GPIO_FUNCSEL_ALT0,
	HAL_GPIO_FUNCSEL_ALT1,
	HAL_GPIO_FUNCSEL_ALT2,
	HAL_GPIO_FUNCSEL_ALT3,
	HAL_GPIO_FUNCSEL_LASTENTRY,
} rpi_gpio_alt_function_t;

typedef enum {
	HAL_GPIO_LVL_LO = 0,
	HAL_GPIO_LVL_HI,
	HAL_GPIO_LVL_UNKNOWN,
	HAL_GPIO_LVL_LASTENTRY,
} hal_gpio_level_t;

typedef enum {
	HAL_GPIO_DETECT_RISING,
	HAL_GPIO_DETECT_FALLING,
	HAL_GPIO_DETECT_HIGH,
	HAL_GPIO_DETECT_LOW,
	HAL_GPIO_DETECT_RISING_ASYNC,
	HAL_GPIO_DETECT_FALLING_ASYNC,
	HAL_GPIO_DETECT_ALL,
	HAL_GPIO_DETECT_LASTENTRY
} hal_gpio_detectMode_t;

typedef enum {
	HAL_GPIO_PULLMODE_UP,
	HAL_GPIO_PULLMODE_DOWN,
	HAL_GPIO_PULLMODE_DISABLE,
	HAL_GPIO_PULLMODE_LASTENTRY,
} hal_gpio_pullupdown_t;


volatile hal_gpio_regs_t * const hal_gpio_GetRegs( void );

hal_error_status_t hal_gpio_SetPullUpDown( rpi_gpio_pin_t GpioNo, hal_gpio_pullupdown_t PullMode );

hal_error_status_t hal_gpio_SetPinFunction( rpi_gpio_pin_t GpioNo, rpi_gpio_alt_function_t Function );
hal_error_status_t hal_gpio_SetOutput( rpi_gpio_pin_t GpioNo );
hal_error_status_t hal_gpio_SetInput( rpi_gpio_pin_t GpioNo );

hal_error_status_t hal_gpio_GetValue( rpi_gpio_pin_t GpioNo, hal_gpio_level_t * Level );

hal_error_status_t hal_gpio_SetHi( rpi_gpio_pin_t GpioNo );
hal_error_status_t hal_gpio_SetLo( rpi_gpio_pin_t GpioNo );
hal_error_status_t hal_gpio_SetValue( rpi_gpio_pin_t GpioNo, hal_gpio_level_t Value );
hal_error_status_t hal_gpio_Toggle( rpi_gpio_pin_t GpioNo );

// see p. 96 ff. https://www.raspberrypi.org/app/uploads/2012/02/BCM2835-ARM-Peripherals.pdf
// interrupt functions
hal_error_status_t hal_gpio_EnablePinInterrupt	( rpi_gpio_pin_t GpioNo, hal_gpio_detectMode_t DetectMode );
hal_error_status_t hal_gpio_DisablePinInterrupt	( rpi_gpio_pin_t GpioNo, hal_gpio_detectMode_t DetectMode );
hal_error_status_t hal_gpio_ClearInterrupt		( rpi_gpio_pin_t GpioNo );


#endif
