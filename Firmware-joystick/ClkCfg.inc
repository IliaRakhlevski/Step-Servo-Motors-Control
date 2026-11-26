/*===========================================
; NAME: OPTION.A
; DESC: Configuration options for .S files
; HISTORY:
; 02.28.2002: ver 0.0
; 03.11.2003: ver 0.0	attached for 2440.
; jan E, 2004: ver0.03  modified for 2440A01.
;===========================================
*/
 
/* Start address of each stacks,*/
_STACK_BASEADDRESS	=	0x33ff8000
_MMUTT_STARTADDRESS	=	0x33ff8000
_ISR_STARTADDRESS	=	0x33ffff00

/*(4) Select Clock Division (Fclk:Hclk:Pclk)
FCLK = 100000000
CLKDIV_VAL	EQU	1	; 0=1:1:1, 1=1:1:2, 2=1:2:2, 3=1:2:4, 4=1:4:4, 5=1:4:8, 6=1:3:3, 7=1:3:6.
FCLK = 200000000
CLKDIV_VAL	EQU	3	; 0=1:1:1, 1=1:1:2, 2=1:2:2, 3=1:2:4, 4=1:4:4, 5=1:4:8, 6=1:3:3, 7=1:3:6.
FCLK = 400000000
CLKDIV_VAL	EQU	5	; 0=1:1:1, 1=1:1:2, 2=1:2:2, 3=1:2:4, 4=1:4:4, 5=1:4:8, 6=1:3:3, 7=1:3:6.
FCLK = 304000000 or 271500000
CLKDIV_VAL	EQU	7	; 0=1:1:1, 1=1:1:2, 2=1:2:2, 3=1:2:4, 4=1:4:4, 5=1:4:8, 6=1:3:3, 7=1:3:6.
*/
 
FCLK	=	200000000
M_MDIV	=	92		/* Fin=12.0MHz Fout=400MHz */
M_PDIV	=	1
M_SDIV	=	1

UCLK	=	48000000
U_MDIV	=	56		/* Fin=12.0MHz Fout=48MHz */
U_PDIV	=	2
U_SDIV	=	2		

CLKDIV_VAL	=	5	    /* 0=1:1:1, 1=1:1:2, 2=1:2:2, 3=1:2:4, 4=1:4:4, 5=1:4:8, 6=1:3:3, 7=1:3:6. */


