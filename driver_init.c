/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hal_init.h>

/*! The buffer size for USART */
#define PC_IO_BUFFER_SIZE 16

struct usart_async_descriptor PC_IO;

static uint8_t PC_IO_buffer[PC_IO_BUFFER_SIZE];

/**
 * \brief USART Clock initialization function
 *
 * Enables register interface and peripheral clock
 */
void PC_IO_CLOCK_init()
{

	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM2_GCLK_ID_CORE, CONF_GCLK_SERCOM2_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM2_GCLK_ID_SLOW, CONF_GCLK_SERCOM2_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBBMASK_SERCOM2_bit(MCLK);
}

/**
 * \brief USART pinmux initialization function
 *
 * Set each required pin to USART functionality
 */
void PC_IO_PORT_init()
{

	gpio_set_pin_function(PB25, PINMUX_PB25D_SERCOM2_PAD0);

	gpio_set_pin_function(PB24, PINMUX_PB24D_SERCOM2_PAD1);
}

/**
 * \brief USART initialization function
 *
 * Enables USART peripheral, clocks and initializes USART driver
 */
void PC_IO_init(void)
{
	PC_IO_CLOCK_init();
	usart_async_init(&PC_IO, SERCOM2, PC_IO_buffer, PC_IO_BUFFER_SIZE, (void *)NULL);
	PC_IO_PORT_init();
}

void TIMER_ULTRASONIC_CLOCK_init(void)
{
	hri_mclk_set_APBAMASK_TC0_bit(MCLK);

	hri_gclk_write_PCHCTRL_reg(GCLK, TC0_GCLK_ID, CONF_GCLK_TC0_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
}

void PWM_TRIGGER_PORT_init(void)
{

	gpio_set_pin_function(PA06, PINMUX_PA06E_TC1_WO0);
}

void PWM_TRIGGER_CLOCK_init(void)
{
	hri_mclk_set_APBAMASK_TC1_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, TC1_GCLK_ID, CONF_GCLK_TC1_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
}

void TIMER_ENCODER_CAPTURE_CLOCK_init(void)
{
	hri_mclk_set_APBCMASK_TC4_bit(MCLK);

	hri_gclk_write_PCHCTRL_reg(GCLK, TC4_GCLK_ID, CONF_GCLK_TC4_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
}

void TIMER_UART_CLOCK_init(void)
{
	hri_mclk_set_APBDMASK_TC6_bit(MCLK);

	hri_mclk_set_APBDMASK_TC7_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, TC6_GCLK_ID, CONF_GCLK_TC6_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
}

void PWM_0_PORT_init(void)
{

	gpio_set_pin_function(PB28, PINMUX_PB28F_TCC1_WO4);

	gpio_set_pin_function(PB29, PINMUX_PB29F_TCC1_WO5);
}

void PWM_0_CLOCK_init(void)
{
	hri_mclk_set_APBBMASK_TCC1_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, TCC1_GCLK_ID, CONF_GCLK_TCC1_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
}

void system_init(void)
{
	init_mcu();

	// GPIO on PA07

	// Set pin direction to input
	gpio_set_pin_direction(ENCODE_B, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(ENCODE_B,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(ENCODE_B, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB04

	gpio_set_pin_level(TRIG_PIN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(TRIG_PIN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(TRIG_PIN, GPIO_PIN_FUNCTION_OFF);
	
	// Capture pins
			
	// PB08
	gpio_set_pin_direction(PB08, GPIO_DIRECTION_IN);
	gpio_set_pin_pull_mode(PB08, GPIO_PULL_OFF);
	gpio_set_pin_function(PB08, GPIO_PIN_FUNCTION_E);
	// PA04 05
	gpio_set_pin_direction(PA04, GPIO_DIRECTION_IN);
	gpio_set_pin_pull_mode(PA04, GPIO_PULL_OFF);
	gpio_set_pin_function(PA04, GPIO_PIN_FUNCTION_E);
	gpio_set_pin_direction(PA05, GPIO_DIRECTION_IN);
	gpio_set_pin_pull_mode(PA05, GPIO_PULL_OFF);
	gpio_set_pin_function(PA05, GPIO_PIN_FUNCTION_E);

	PC_IO_init();

	TIMER_ULTRASONIC_CLOCK_init();

	TIMER_ULTRASONIC_init();

	PWM_TRIGGER_CLOCK_init();

	PWM_TRIGGER_PORT_init();

	PWM_TRIGGER_init();

	TIMER_ENCODER_CAPTURE_CLOCK_init();

	TIMER_ENCODER_CAPTURE_init();

	TIMER_UART_CLOCK_init();

	TIMER_UART_init();

	PWM_0_CLOCK_init();

	PWM_0_PORT_init();

	PWM_0_init();
}
