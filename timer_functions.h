#ifndef TIMER_FUNCTIONS
#define TIMER_FUNCTIONS

#include "read_functions.h"
#include "internal_functions.h"
#include "write_functions.h"

///////////////////////// TIMER_ENCODER_CAPTURE /////////////////////////

#define TIMER_CAPTURE TC4
#define MAX_COUNT 65536
#define MAX_RANGE 30000
//resolution of the encoder
#define CPR 1440
#define FREQ_CONST 12000000
//prescaler
#define DIV_CONST 2


volatile int8_t direction_value = 0;
volatile int32_t step_value = 0;
volatile int32_t speed_value = 0;

volatile int handler_count = 0;
volatile int interrupt_before = 0;
volatile int previous_count = 0;

volatile uint8_t test_buffer[10]= {0};
void TC4_Handler(void)
{
	
	// calculate how much time (ticks) the step uses
	if (TIMER_CAPTURE->COUNT16.INTFLAG.bit.OVF)
	{
		TIMER_CAPTURE->COUNT16.INTFLAG.bit.OVF = 1;
		if (interrupt_before == 1)
		{
			handler_count = MAX_COUNT;
		}
		else
		{
			interrupt_before = 1;
			return;
		}
	}
	else if (TIMER_CAPTURE->COUNT16.INTFLAG.bit.MC0)
	{
		TIMER_CAPTURE->COUNT16.INTFLAG.bit.MC0 = 1;
		if (interrupt_before == 1)
		{
			handler_count = TIMER_CAPTURE->COUNT16.CC[0].reg + MAX_COUNT - previous_count;
			previous_count = TIMER_CAPTURE->COUNT16.CC[0].reg;
		}
		else
		{
			handler_count = TIMER_CAPTURE->COUNT16.CC[0].reg - previous_count;
			previous_count = TIMER_CAPTURE->COUNT16.CC[0].reg;
		}
		interrupt_before = 0;
	}
	
	if(handler_count == MAX_COUNT)
	{
		speed_value = 0;
	}
	else
	{
		// recognize this step's direction and update the step
		int encoder_B_value = gpio_get_pin_level(ENCODE_B);
		if (encoder_B_value == 1)
		{
			direction_value = 1;
		step_value = (step_value<MAX_RANGE? step_value + 1: step_value);
		}
		else
		{
			direction_value = -1;
			step_value = (step_value>-MAX_RANGE? step_value - 1: step_value);
		}
	
		// update the speed
		speed_value = direction_value * FREQ_CONST * 60 / (DIV_CONST * CPR * handler_count);
	}
		
}

///////////////////////// TIMER_ULTRASONIC /////////////////////////

#define TIMER_ULTRASONIC TC0

//cpu frequency
#define FREQ_CONST_US 12000000
//max count value
#define MAX_US 65536
//prescaler
#define DIV_CONST_US 16
//speed of sound
#define SOUND_SPEED 340

volatile float measured_distance;
volatile int is_overflow;
volatile int is_overflow_repeat;
volatile int is_count_start;

void TC0_Handler(void)
{
	if (TIMER_ULTRASONIC->COUNT16.INTFLAG.bit.OVF)
	{
		TIMER_ULTRASONIC->COUNT16.INTFLAG.bit.OVF = 1;
		if(is_overflow==1){
			is_overflow_repeat = 1;
		}
		else{
			is_overflow = 1;	
		}
	}
	else if (TIMER_ULTRASONIC->COUNT16.INTFLAG.bit.MC0)
	{
		TIMER_ULTRASONIC->COUNT16.INTFLAG.bit.MC0 = 1;
		// start the count
		is_count_start = 1;
		is_overflow = 0;
		is_overflow_repeat = 0;
	}
	else if (TIMER_ULTRASONIC->COUNT16.INTFLAG.bit.MC1)
	{
		TIMER_ULTRASONIC->COUNT16.INTFLAG.bit.MC1 = 1;
		
		if (is_count_start == 0 || is_overflow_repeat == 1){
			measured_distance = 0;
			return;
		}
		
		int count_diff = 0;
		if (is_overflow == 1)
		{
			count_diff = TIMER_ULTRASONIC->COUNT16.CC[1].reg - TIMER_ULTRASONIC->COUNT16.CC[0].reg + MAX_US;
		}
		else
		{
			count_diff = TIMER_ULTRASONIC->COUNT16.CC[1].reg - TIMER_ULTRASONIC->COUNT16.CC[0].reg;
		}
		
		measured_distance = 1.0 * count_diff * DIV_CONST_US / FREQ_CONST_US * SOUND_SPEED;
		measured_distance = (measured_distance <= 0.1 ? 0.1: measured_distance);
		measured_distance = (measured_distance >= 1 ? 1: measured_distance);
		is_count_start = 0;
	}
}

///////////////////////// TIMER_UART /////////////////////////////////

#include <stdio.h>

#define STRINGIFY(x) #x

#define TIMER_UART TC6
//input range: 6 for [-100,100], 7 for [-3000,3000]
#define RANGE 3000
#define INPUT_BUF_LEN(range) 3+sizeof(STRINGIFY(range))-1  

volatile uint8_t input_buffer[INPUT_BUF_LEN(RANGE)];
volatile uint8_t output_buffer[19];

/**
 * @brief Handle the UART input and output in 10Hz
 *
 */
void TC6_Handler(void)
{
	if (TIMER_UART->COUNT32.INTFLAG.bit.MC0)
	{
		TIMER_UART->COUNT32.INTFLAG.bit.MC0 = 1;
		
		// read the input and handle the command
		volatile int length;
		for (int i = 0; i < INPUT_BUF_LEN(RANGE); i++)
		{
			input_buffer[i] = 0;
		}
		//get the length of the input
		length = UART_read(input_buffer, INPUT_BUF_LEN(RANGE));
		if (length!=0 && is_valid_input(input_buffer, length, RANGE))
		{	
			//open loop
			//input_update_vel(input_buffer, length);
			//closed loop
			input_update_pos(input_buffer, length);
		}

		// send output via UART
		sprintf(output_buffer, "[%+6d, %+4dRPM, %+3dcm]\n", (int)step_value, (int)speed_value, (int)(measured_distance*100));
		UART_write(output_buffer, 25);	
		//za ultrasonic dodaj: (int)(measured_distance*100)	
	}
}

///////////////////////// TIMER_CONTROL_LOOP /////////////////////////
//define Kp and Kv
//Kp=0.003
//Kvel=0.09
float K_P= 0.003;
float K_VEL= 0.09;
float SPEED_MAX= 100;
float SPEED_MIN= 1;

volatile float pwm_state = 0;
volatile int pwm_int=0;
volatile float vel_desire = 0;
volatile int pos_desire = 0;

//update position based on the input
void update_pos_desire(int pos){
	pos_desire = pos;
}

//update the speed
void update_vel_desire(int vel){
	vel_desire = vel;
}


//closed loop timer control
void TCC1_0_Handler(void){
	if(TCC_PWM->INTFLAG.bit.OVF)
	{
		TCC_PWM->INTFLAG.bit.OVF = 1;
		//calculate desired position based on ultrasonic sensor distance
		pos_desire = measured_distance * 1000;
		//calculate desired velocity based on position error
		vel_desire = K_VEL * (pos_desire - step_value);
		vel_desire = (vel_desire>0 && vel_desire<SPEED_MIN ? SPEED_MIN : vel_desire);
		vel_desire = (vel_desire>-SPEED_MIN && vel_desire<0 ? -SPEED_MIN : vel_desire);
		vel_desire = (vel_desire>SPEED_MAX ? SPEED_MAX : vel_desire);
		vel_desire = (vel_desire<-SPEED_MAX ? -SPEED_MAX : vel_desire);
		//calculate new PWM value
		pwm_state = pwm_state + K_P * (vel_desire - speed_value);
		//convert to int
		pwm_int=(int)pwm_state;
		Motor_PWM_update(pwm_state);
	}
}

#endif
