#ifndef WRITE_FUNCTIONS
#define WRITE_FUNCTIONS

/////////////////////////////UART_part /////////////////////////////////
struct io_descriptor *pc_io_out;

/**
function to write output to UART
write_buffer the buffer to store the data to be written
length the desired length of the written data
 */
void UART_write(uint8_t *write_buffer, int length){
	while(io_write(pc_io_out, write_buffer, length)==0);
}

//motor control
#include <pwm.h>
#define TCC_PWM TCC1
#define VALUE_FOR_01ms 1200
#define VALUE_PERIOD VALUE_FOR_01ms

/**
 Function to update the PWM value of the motor
 param value = value between -100 and 100
 */
void Motor_PWM_update(int value){
	//channels: input 3 & 4
    int valueChannelPos = 0;
    int valueChannelNeg = 0;
	//check the limits
	value = (value>100?100:value);
	value = (value<-100?-100:value);

    // the value of duty cycle is inverted.
    if(value > 0){
        valueChannelPos = VALUE_PERIOD * value / 100;
    }else if(value < 0){
        valueChannelNeg = VALUE_PERIOD * (-value) / 100;
    }
	//set PWM to channels input 4 and 3
    tcc_set_compare_value(TCC_PWM, valueChannelPos, 0);
	tcc_set_compare_value(TCC_PWM, valueChannelNeg, 1);
}

#endif