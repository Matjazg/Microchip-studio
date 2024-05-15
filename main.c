#include <atmel_start.h>
#include "read_functions.h"
#include "timer_functions.h"
#include "internal_functions.h"
#include "write_functions.h"

static void rx_cb_dummy(const struct usart_async_descriptor *const io_descr)
{
	/* transfer completed */
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	/* Initialize the UART IO */
	struct io_descriptor *pc_io;
	usart_async_register_callback(&PC_IO, USART_ASYNC_RXC_CB, rx_cb_dummy);
	usart_async_get_io_descriptor(&PC_IO, &pc_io);
	usart_async_enable(&PC_IO);
	pc_io_in = pc_io;
	pc_io_out = pc_io;
	//initialize the UART timer
	NVIC_EnableIRQ(TC6_IRQn);
	
	//set enable high
	gpio_set_pin_level(ENABLE, 1);
	/* Initialize the PWM */
	PWM_0_init();
	tcc_enable(TCC_PWM);
	Motor_PWM_update(50);
	//initialize cascaded control timer
	NVIC_EnableIRQ(TCC1_0_IRQn);
	
	//Initialize the timer for encoder loop 
	NVIC_EnableIRQ(TC4_IRQn);
	
	//Initialize the timer for ultrasonic detector 
	NVIC_EnableIRQ(TC0_IRQn);

	/* Main part of the code */
	while (1) {

	}
}
