/* Program to write a file to the PCjr Development RAM cart
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
	unsigned int rom_size;

	fprintf(stderr, "Write ROM data to PCjr Development RAM cartridge\r\n");
	/* TODO - allow picking file name */
	if (argc != 2) {
		fprintf(stderr, "Arg $1: PCjr ROM file input w/o JRC (ROM.JRB)\r\n");
		return 0;
	}

	file_name = argv[1];
	fprintf(stderr, "PCjr JRB Input ROM: %s\r\n", file_name);
	
	/* Open input file */
	file = fopen(file_name, "rb");

	/* Determine ROM file size */
	rom_size = 0;
	while (1) {
		fgetc(file);
		if (feof(file)) {
			break;
		}
		rom_size++;
	}
	rewind(file);

	/* Error out if file is zero */
	if (rom_size == 0) {
		fprintf(stderr, "Error reading file, got 0 byte size, exiting.\r\n");
		fclose(file);
		return 2;
	}
	/* Error out if file is > CART_RAM_SIZE */
	if (rom_size > CART_RAM_SIZE) {
		fprintf(stderr, "Error, file_size(%u) is larger than CART_RAM_SIZE(%u)\r\n", rom_size, (unsigned int)CART_RAM_SIZE);
		fclose(file);
		return 3;
	}

	/* Read each byte of the ROM file, write it to the ROM */
	for (i = 0; i < rom_size; i++) {
		data = (unsigned char)fgetc(file);
		write_read_cart_ram_byte(i, data);
	}
	fclose(file);
	return 0;
}
