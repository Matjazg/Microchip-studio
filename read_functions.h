#ifndef READ_FUNCTIONS
#define READ_FUNCTIONS

/////////////////////////////UART_part /////////////////////////////////
struct io_descriptor *pc_io_in;

/**
function to read input commands from UART
param read_buffer the buffer to store the read data
param length the desired length of the read data
return int the actual length of the read data
 */
int UART_read(uint8_t *read_buffer, int length){
	int read_status;
	read_status = io_read(pc_io_in, read_buffer, length);
	return read_status;
}



#endif