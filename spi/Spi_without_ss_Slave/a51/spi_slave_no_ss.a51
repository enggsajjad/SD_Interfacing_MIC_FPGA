$INCLUDE   (reg_c51.INC)
transmit_completed BIT 20H.1; software flag
serial_data DATA 08H

org 000h
ljmp begin

org 4Bh
ljmp it_SPI

;/**
; * FUNCTION_PURPOSE: This file set up spi in slave mode with 
; * Fclk Periph/128 as baud rate and without slave select pin.
; * FUNCTION_INPUTS: P1.5(MISO) serial input  
; * FUNCTION_OUTPUTS: P1.7(MOSI) serial output
; *                   P1.1
; */
org 0100h
begin:

ORL SPCON,#20h;                  /* P1.1 is available as standard I/O pin */
;/* SPCON.5(SSDIS) has no effect if CPHA=0 in slave mode then P1.1 is used to slave select */ 
ANL SPCON,#0EFh;                 /* slave mode */
ANL SPCON,#0F7h;                 /* CPOL=0; transmit mode example */
ORL SPCON,#04h;                  /* CPHA=1; transmit mode example */
ORL IEN1,#04h;                   /* enable spi interrupt */
ORL SPCON,#40h;                  /* run spi */
CLR transmit_completed;          /* clear software transfert flag */
SETB EA;                         /* enable interrupts */


loop:             				   /* endless  */
CPL P1.1;                        /* P1.1 is available as standard I/O pin */
JNB transmit_completed,end_if

   MOV SPDAT,serial_data;        /* echo data to master */
   CLR transmit_completed;       /* clear software transfert flag */

end_if:
LJMP loop


;/**
; * FUNCTION_PURPOSE: spi interrupt, receive data to master
; * FUNCTION_INPUTS: void
; * FUNCTION_OUTPUTS: void
; */
it_SPI:;                         /* interrupt address is 0x004B */

MOV R7,SPSTA;
MOV ACC,R7
JNB ACC.7,break1;case 0x80:
    MOV serial_data,SPDAT;       /* read receive data */
    SETB transmit_completed;     /* set software flag */
break1:

JNB ACC.4,break2;case 0x10:
;         /* put here for mode fault tasking */	
break2:;
	
JNB ACC.6,break3;case 0x40:
;         /* put here for overrun tasking */	
break3:;

MOV SPDAT,serial_data;        /* needed to complete clearing sequence */

RETI

end

