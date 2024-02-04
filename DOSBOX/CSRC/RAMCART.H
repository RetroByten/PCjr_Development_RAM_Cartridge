/* Ram Cart Header Library for reading/writing data
* 
* 
* * Note: If code is executing from address range E0000-EFFFF this will not function
* Would need to move the write routine to RAM because as it executes the code it would constantly be re-locking 
* (and cannot run from E8000-EFFFF because that would be constant write attempts)
* 
* TODO -- Write actual assembly equivalent / examples
*/

/*** System/Compiler Libraries ***/
#ifndef __DOS_H__
#define __DOS_H__
#include <dos.h> /* for SREGs, */
#endif 

/* Defined by hardware implementation */
#define CART_READ_AND_UNLOCK_SEGMENT	0xD800
#define CART_CACHE_AND_LOCK_SEGMENT		0xE000
#define CART_WRITE_SEGMENT				0xE800
#define CART_RAM_SIZE					0x8000

/*** Globals ***/
struct SREGS* dos_registers;

/* Helper Function definitions */
unsigned char read_cart_ram_byte(address)
unsigned int address;
{
	unsigned int segment;
	unsigned char data;

	segment = (unsigned int)CART_READ_AND_UNLOCK_SEGMENT;
	segread(dos_registers); /* Update current segment regs */
	movedata(segment, address, dos_registers->ds, &data, 1); /* Not sure if &data needs to be global */
	return data;
	/* Assembly equivalent ish
	Read/Unlock
	mov ax,d800
	mov ds,ax
	mov bh,[address]
	*/
}

unsigned char write_cart_ram_byte(address, data)
unsigned int address;
unsigned char data;
{
	unsigned int segment;
	unsigned int dummy_address;
	unsigned char dummy_data;
	
	/* Cache/Lock to cache the current data */
	segment = (unsigned int)CART_CACHE_AND_LOCK_SEGMENT;
	dummy_address = (unsigned int)(0x00 << 8 | data);
	segread(dos_registers); /* Update current segment regs */
	movedata(segment, dummy_address, dos_registers->ds, &dummy_data, 1); /* Not sure if &dummy_data needs to be global */

	/* Read/Unlock - Dummy read to unlock*/
	segment = (unsigned int)CART_READ_AND_UNLOCK_SEGMENT;
	dummy_address = (unsigned int)0x0000;
	segread(dos_registers); /* Update current segment regs */
	movedata(segment, dummy_address, dos_registers->ds, &dummy_data, 1); /* Not sure if &dummy_data needs to be global */

	/* Write to actual address */
	segment = (unsigned int)CART_WRITE_SEGMENT;
	dummy_address = address;
	segread(dos_registers); /* Update current segment regs */
	movedata(segment, dummy_address, dos_registers->ds, &dummy_data, 1); /* Not sure if &dummy_data needs to be global */
	return 0;

	/* Assembly equivalent ish
	Cache/Lock data
	mov ax,e000
	mov ds,ax
	mov bh,[00<data>]

	Read/Unlock
	mov ax,d800
	mov ds,ax
	mov bh,[0000] ; dummy read to unlock

	Write to address
	mov ax,e800
	mov ds,ax
	mov bh,[address]
	*/

}

unsigned char write_read_cart_ram_byte(address, data)
unsigned int address;
unsigned int data;
{
	write_cart_ram_byte(address, data);
	return read_cart_ram_byte(address);
}

