/* Program to dump the current Development RAM cart contents
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

/*** Main Function ***/
main(argc, argv, envp)
int argc;
char** argv;
char** envp;
{
	/* LOCAL VARS */
	char* file_name;
	FILE* file;
	unsigned int i;
	unsigned char data;

	fprintf(stderr, "Dump PCjr Development RAM cartridge contents to RCDUMP.BIN\r\n");
	/* TODO - allow picking file name */
	file_name = "RCDUMP.BIN";

	file = fopen(file_name, "wb");

	for (i = 0; i < CART_RAM_SIZE; i++) {
		data = read_cart_ram_byte(i, data);
		fwrite(&data, sizeof(unsigned char), 1, file);
	}
	fclose(file);
	return 0;
}

