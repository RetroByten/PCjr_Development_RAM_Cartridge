TITLE Jump into Cartridge Init function

; TODO: Was learning PTR and remote segments, simplify this whole program
; And write a utility to check vectors instead

; Program equates
int_video						EQU	10h
	svc_video_write_char_tty	EQU	0Eh
	tty_page						EQU 00h
int_dos_21						EQU 21h
	svc_dos_print_str			EQU 09h
	svc_dos_terminate			EQU 4Ch
		val_dos_terminate_0			EQU 00h
int_18h							EQU 18h
	int18_offset					EQU int_18h * 04h
	int18_segment					EQU int18_offset + 02h


; Ram Cart equates
interrupt_vector_segment		EQU 0000h
system_ram_segment				EQU 0050h
;cart_ram_read_segment			EQU	0E000h ; OLD RAM CART
cart_ram_read_segment			EQU 0D800h ; NEW DEV RAM CART
cart_ram_entry_offset			EQU 0003h

XXDATA SEGMENT AT 50h 
	db 14 dup(?)
	IO_ROM_INIT DW ?
	IO_ROM_SEG DW ?
XXDATA ENDS

CODESEG SEGMENT
	assume CS:CODESEG, DS:CODESEG, SS:CODESEG
ORG 100h
PROGRAM: ; Needed for COM to later terminate the program
jmp	short	START_FUNCTION

UTIL_PRINT_HEX_NIBBLES PROC NEAR
	; CX = # Of nibbles to print
	; AX = 1-4 nibbles to print from AH high -> AL Low
	push	bx
UTIL_PRINT_HEX_NIBBLES_LOOP:
	rol		ax,1 ; Grab the highest nibble by Rotating left 4x
	rol		ax,1 ;
	rol		ax,1 ;
	rol		ax,1 ; 
	push	ax ; Save off the Nth rotated AX
	and		al,00Fh ; Isolate the nibble
	add		al,'0' ; Shift into the ASCII Numeric Range
	cmp		al,'9' ; Check if <= 9
	jle		UTIL_PRINT_HEX_NIBBLES_CONT
	add		al,'A'-'9'-1 ; If not, shift up to ASCII 'A' range
UTIL_PRINT_HEX_NIBBLES_CONT:
	mov		ah,svc_video_write_char_tty ; Select DOS Print TTY service
	mov		bh,tty_page
	int		int_video ; Print current character
	pop		ax ; Restore Nth rotated AX
	loop	UTIL_PRINT_HEX_NIBBLES_LOOP
	pop		bx
	ret
UTIL_PRINT_HEX_NIBBLES ENDP

UTIL_PRINT_BYTE MACRO
	; Print 2 Nibbles from AH
	; AH = byte
	mov		cx,02h
	call 	UTIL_PRINT_HEX_NIBBLES
ENDM

UTIL_PRINT_WORD MACRO
	; Print 4 Nibbles from AX
	; AX = word
	mov		cx,04h
	call 	UTIL_PRINT_HEX_NIBBLES
ENDM

UTIL_PRINT_NEWLINE PROC NEAR ; Print a /r/n
	;; Print \r\n
	push	bx
	mov		ax,(svc_video_write_char_tty SHL 8) OR 0Dh
	mov		bh,tty_page
	int		int_video
	mov		ax,(svc_video_write_char_tty SHL 8) OR 0Ah;
	mov		bh,tty_page
	int		int_video
	pop		bx
	ret
UTIL_PRINT_NEWLINE ENDP

START_FUNCTION PROC NEAR
	; Print welcome message
	mov		dx,offset WELCOME_MESSAGE
	mov		ah,svc_dos_print_str
	int		int_dos_21

	; Save Current Segments
	push	es
	push	ds

	mov		ax,interrupt_vector_segment
	mov		es,ax
	;; Print Current int18h value
	mov		ax,es:int18_segment
	UTIL_PRINT_WORD
	mov		ax,(svc_video_write_char_tty SHL 8) OR ':'
	int		int_video
	mov		ax,es:int18_offset
	UTIL_PRINT_WORD
	call UTIL_PRINT_NEWLINE

	; Print getting to work message
	mov		dx,offset UPDATING_INT18_VALUE
	mov		ah,svc_dos_print_str
	int		int_dos_21
	
	; Setup Segments for cartridge and temporary data area
	mov		ax,cart_ram_read_segment
	mov		ds,ax
	mov		ax,XXDATA
	mov		es,ax

	;; TODO - NEED TO REPLICATE THE ROM_CHECK STACK
	mov		ax,cart_ram_entry_offset
	push	ax ; Simulate the DX count
	
	; Invoke the cartridge entry point
	mov		es:IO_ROM_INIT,cart_ram_entry_offset
	mov		es:IO_ROM_SEG,ds
	call	DWORD PTR ES:IO_ROM_INIT

	;; Pop "DX"
	pop		ax

	; Restore DS,
	pop		ds
	
	; Print Int 18 Value message
	mov		dx,offset CURRENT_INT18_VALUE
	mov		ah,svc_dos_print_str
	int		int_dos_21
	
	;  Set ES to interrupt vector table
	mov		ax,interrupt_vector_segment
	mov		es,ax 
	
	;; Print Current int18h value
	mov		ax,es:int18_segment
	UTIL_PRINT_WORD
	mov		ax,(svc_video_write_char_tty SHL 8) OR ':'
	int		int_video
	mov		ax,es:int18_offset
	UTIL_PRINT_WORD
	call UTIL_PRINT_NEWLINE
	
	;; Restore ES
	pop		es

	;; Return to DOS
	mov ax,(svc_dos_terminate SHL 8) OR val_dos_terminate_0
	int int_dos_21
START_FUNCTION ENDP

WELCOME_MESSAGE db "PCjr Cartridge Init"
db,0Dh,0Ah
CURRENT_INT18_VALUE db "Current int18h value: "
db "$"
UPDATING_INT18_VALUE db "Updating int18h value...",0Dh,0Ah,"$"

CODESEG ENDS
END PROGRAM
