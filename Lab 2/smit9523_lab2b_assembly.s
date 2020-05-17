.include "xc.inc"

.text                       ;BP (put the following data in ROM(program memory))

; This is a library, thus it can *not* contain a _main function: the C file will
; define main().  However, we
; we will need a .global statement to make available ASM functions to C code.
; All functions utilized outside of this file will need to have a leading 
; underscore (_) and be included in a comment delimited list below.
.global _delay_100us, _delay_1ms, _write_0, _write_1

; (100*10^-6)/(62.5*10^-9)
_delay_100us:
    repeat #1593
    nop
    return
; 5 high and 15 low
    
_delay_1ms:
    repeat #15993
    nop
    return

_write_0:		; 2 (low)
    inc	    LATA	; 1 (high?)
    repeat #2		; 1 
    nop			; 2+1 = 3
    clr	    LATA	; 1 (low)
    repeat #7		; 1
    nop			; 7+1 = 8
    return		; 3
   
; 11 high, 9 low
_write_1:		; 2 (low)
    inc	    LATA	; 1 (high)
    repeat #9		; 1
    nop			; 8+1 = 9
    clr	    LATA	; 1
    ;repeat #1		; 1
    nop
    nop			; 1+1 = 2
    return		; 3


