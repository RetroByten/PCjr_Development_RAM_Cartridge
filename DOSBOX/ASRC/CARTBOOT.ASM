TITLE Invoke int 18h for cartridge

; Program equates
int_18h							EQU 18h

CODESEG SEGMENT
	assume CS:CODESEG, DS:CODESEG, SS:CODESEG
ORG 100h
PROGRAM: ; Needed for COM to later terminate the program
START_FUNCTION PROC NEAR
	int		int_18h
	jmp		START_FUNCTION; Can't return from cartridge, if we somehow get here just go around again...
START_FUNCTION ENDP
CODESEG ENDS
END PROGRAM
