Repository Description:
PCjr Development RAM Cartridge schematic and related programs to test, read, and write to the cartridge. This cartridge is a development tool to aid in developing and testing software intended to run from the cartridge port without needing to constantly burn ROM chips. 

KiCad Project:
PCjr Development RAM Cartridge circuit schematic. 

Program Files:
ASRC/CARTBOOT.ASM - a MASM 2.0 DOS program to invoke int18h for cartridge programs that configure it and return.
ASRC/CARTINIT.ASM - a MASM 2.0 DOS program to execute the cartidge initialization function at D800:0003. Depending on the cartridge program, it may either immediately take control of the system, or return after configuring int18h.

CSRC/RAMCART.H - a K&R C header library containing functions to read and write to the PCjr Development RAM Cartridge
CSRC/RAMCDUMP.C - a K&R C DOS program to dump the current contents of the PCjr Development RAM Cartridge to a binary file (RCDUMP.BIN)
CSRC/RAMCTEST.C - a K&R C DOS program to test whether the PCjr Development RAM Cartridge is working by writing patterns (A5 and then 5A) to the cartridge and then reading them back. 
CSRC/RAMCWRIT.C - a K&R C DOS program to write a binary data file to PCjr Development RAM Cartridge. 

Languages:
IBM/Microsoft C (K&R C)
IBM/Microsoft MACRO Assembly Language (MASM 2.0)

Compiler:
IBM Personal Computer C Compiler (1.00)
IBM Personal Computer MACRO Assembler (2.00)

Manual Build Instructions for K&R C programs:
1.) Copy the .h and .c files into the compiler folder
2.) cc.exe <main>.c
3.) clink.exe <main>.obj

Manual Build Instructions for MASM programs:
1.) Copy the .asm files into the assembler folder
2.) masm.exe <main>.asm
3.) link.exe <main>.obj
4.) exe2bin.exe <main>.exe <main>.com

Notes:
gcc on Linux doesn't seem to like the ancient K&R syntax

