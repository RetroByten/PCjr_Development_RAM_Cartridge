/* Program test the PCjr Development RAM cartridge
* 
*/

/*** System/Compiler Libraries ***/
#ifndef __STDIO_H__
#define __STDIO_H__
#include <stdio.h> /* for fprintf */
#endif

/*** Project Specific Libraries ***/
#include "RAMCART.H"

/*** Globals ***/

/*** Function definitions ***/
unsigned char flood_write_read(data)
unsigned char data;
{
	unsigned int i;
	unsigned char ret_data;
	unsigned char failure_count;

	failure_count = 0;

	for (i = 0; i < CART_RAM_SIZE; i++) {
		ret_data = write_read_cart_ram_byte(i, data);
		if (data != ret_data) {
			fprintf(stdout, "Failed: @%u,(%02X != %02X)\r\n", i, data, ret_data);
			failure_count += 1;
		}
	}
	return failure_count;
}

/*** Main Function ***/
main(argc, argv, envp)
int argc;
char** argv;
char** envp;
{
	/* LOCAL VARS */
	
	unsigned char failure_count;

	fprintf(stdout, "Testing PCjr Development RAM cart!\r\n");
	
	fprintf(stdout, "Testing 0A5h writes...\r\n");
	failure_count = flood_write_read((unsigned char)0xA5);
	fprintf(stdout,"Failure count: %u\r\n", (unsigned int)failure_count);

	fprintf(stdout, "Testing 05Ah writes...\r\n");
	failure_count = flood_write_read((unsigned char)0x5A);
	fprintf(stdout, "Failure count: %u\r\n", (unsigned int)failure_count);

	return 0;
}
