
.include "xc.inc"          ; required "boiler-plate" (BP)

;the next two lines set up the actual chip for operation - required
config __CONFIG2, POSCMOD_EC & I2C1SEL_SEC & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSECME & FNOSC_FRCPLL & SOSCSEL_LPSOSC & WUTSEL_FST & IESO_OFF
config __CONFIG1, WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & BKBUG_ON & GWRP_ON & GCP_ON & JTAGEN_OFF

    .bss        ; put the following labels in RAM
counter:
    .space 2    ; a variable that takes two bytes (we won?t use
                ; it for now, but put here to make this a generic
                ; template to be used later).
stack:
    .space  32  ; this will be our stack area, needed for func calls

.text           ; BP (put the following data in ROM(program memory))

;because we are using the C compiler to assemble our code, we need a "_main" label
;somewhere. (There's a link step that looks for it.)
.global _main               ;BP
;your functions go here

_main:

    bclr    CLKDIV,#8                 ;BP
    nop
    ;; --- Begin your main program below here ---
    
    
    mov     #0x9fff,w0  
    mov     w0,AD1PCFG            ; Set all pins to digital mode
    mov     #0b1111111111111110,w0  
    mov     w0,TRISA            ; set pin RA0 to output
    mov     #0x0001,w0  
    mov     w0,LATA            ; set pin RA0 high
    call foreverLoop
   
wait_24cycles:     ; 2 cycles for function call
    repeat #14        ; 1 cycle to load and prep
    nop              ; 14+1 cycles to execute NOP 15 times
    return           ; 3 cycles for the return
  
; (100*10^-6)/(62.5*10^-9)
delay_100us:
    repeat #1593
    nop
    return
; 5 high and 15 low

write_0:		; 2 (low)
    inc	    LATA	; 1 (high?)
    repeat #2		; 1 
    nop			; 2+1 = 3
    clr	    LATA	; 1 (low)
    repeat #7		; 1
    nop			; 7+1 = 8
    return		; 3
   
; 11 high, 9 low
write_1:		; 2 (low)
    inc	    LATA	; 1 (high)
    repeat #9		; 1
    nop			; 8+1 = 9
    clr	    LATA	; 1
    ;repeat #1		; 1
    nop
    nop			; 1+1 = 2
    return		; 3
    
    
foreverLoop:
    
    ; F5 Red
    call write_1
    call write_1
    call write_1
    call write_1
    call write_0
    call write_1
    call write_0
    call write_1
   
    
    ; F5 Blue
    call delay_100us
    call write_1
    call write_1
    call write_1
    call write_1
    call write_0
    call write_1
    call write_0
    call write_1
    
    ; 05 Green
    call write_0
    call write_0
    call write_0
    call write_0
    call write_0
    call write_1
    call write_0
    call write_1

    
    
    
    bra foreverLoop
.end 
