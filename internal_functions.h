#ifndef INTERNAL_FUNCTIONS
#define INTERNAL_FUNCTIONS


#include "write_functions.h"
#include "timer_functions.h"

#define OUTPUT_MSG(str) UART_write((uint8_t *)str, sizeof(str)-1) 

/**
 output the response according to the status
 status the status of the input
 */
void output_response(int status){
	switch (status)
	{
		case 0: OUTPUT_MSG("Value accepted\n"); break;
		case 1: OUTPUT_MSG("[ERR]invalid because not corrected terminated as '\\r\\n'\n"); break;
		case 2: OUTPUT_MSG("[ERR]invalid because first digit is not '+' or '-'\n"); break;
		case 3: OUTPUT_MSG("[ERR]invalid because the value part are not digits\n"); break;
		case 4: OUTPUT_MSG("[ERR]invalid because the value is out of range\n"); break;
		default: OUTPUT_MSG("[ERR]Should not execute."); break;
	}
}

/*
 convert the input to the correct int value 
 input_buffer the buffer to store the input
 length the length of the input
 */
int convert_str_2_value(uint8_t *input_buffer, int length){
	int value = 0;
	for (int i = 1; i < length - 2; i++)
	{
		value = value * 10 + input_buffer[i] - '0';
	}
	if (input_buffer[0] == '-')
	{
		value = -value;
	}
	return value;
}

/*
 check if the input is valid 
 input_buffer the buffer to store the input
 length the length of the input
 return the result of the check 0: invalid 1: valid
 */
int is_valid_input(uint8_t *input_buffer, int length, int maximum){
	// check if the input is valid
	int status = 1, reason = 0, value;
	if (input_buffer[length - 2] != '\r' || input_buffer[length - 1] != '\n')
	{
		status = 0;
		reason = 1;
	}
	else if (input_buffer[0] != '+' && input_buffer[0] != '-')
	{
		status = 0;
		reason = 2;
	}
	else
	{
		for (int i = 1; i < length - 2; i++)
		{
			if (input_buffer[i] < '0' || input_buffer[i] > '9')
			{
				status = 0;
				reason = 3;
				break;
			}
		}
	}

	// convert the input to the corresponding value
	if (status == 1)
	{
		value = convert_str_2_value(input_buffer, length);
		if (value < - maximum || value > maximum)
		{
			status = 0;
			reason = 4;
		}
	}

	// output the response
	output_response(reason);

	// return the status
	return status;
}

/*Open-loop
 update the motor command value by the input 
 input_buffer the buffer to store the input
 length the length of the input
 */
/*void input_update_vel(uint8_t *input_buffer, int length){
	int value = convert_str_2_value(input_buffer, length);
	//open loop control
	//Motor_PWM_update(value);
	//closed loop
	update_vel_desire(value);
}
*/
//closed loop
//update desired position based on uart input
void input_update_pos(uint8_t *input_buffer, int length){
	int value = convert_str_2_value(input_buffer, length);
	update_pos_desire(value);       
}

#endif