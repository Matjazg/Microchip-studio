/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef DRIVER_INIT_INCLUDED
#define DRIVER_INIT_INCLUDED

#include "atmel_start_pins.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <hal_atomic.h>
#include <hal_delay.h>
#include <hal_gpio.h>
#include <hal_init.h>
#include <hal_io.h>
#include <hal_sleep.h>

#include <hal_usart_async.h>
#include <tc_lite.h>
#include <tc_lite.h>
#include <tc_lite.h>
#include <tc_lite.h>
#include <tcc_lite.h>

extern struct usart_async_descriptor PC_IO;

void PC_IO_PORT_init(void);
void PC_IO_CLOCK_init(void);
void PC_IO_init(void);

void TIMER_ULTRASONIC_CLOCK_init(void);

int8_t TIMER_ULTRASONIC_init(void);

void PWM_TRIGGER_CLOCK_init(void);

void PWM_TRIGGER_PORT_init(void);

int8_t PWM_TRIGGER_init(void);

void TIMER_ENCODER_CAPTURE_CLOCK_init(void);

int8_t TIMER_ENCODER_CAPTURE_init(void);

void TIMER_UART_CLOCK_init(void);

int8_t TIMER_UART_init(void);

void PWM_0_CLOCK_init(void);

void PWM_0_PORT_init(void);

int8_t PWM_0_init(void);

/**
 * \brief Perform system initialization, initialize pins and clocks for
 * peripherals
 */
void system_init(void);

#ifdef __cplusplus
}
#endif
#endif // DRIVER_INIT_INCLUDED
