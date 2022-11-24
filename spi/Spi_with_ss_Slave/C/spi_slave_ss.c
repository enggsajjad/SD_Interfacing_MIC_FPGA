/**
 * @file $RCSfile: spi_slave_ss.c,v $
 *
 * Copyright (c) 2004 Atmel.
 *
 * Please read file license.txt for copyright notice.
 *
 * @brief This file is an example to use spi in slave mode.
 *
 * This file can be parsed by Doxygen for automatic documentation
 * generation.
 * Put here the functional description of this file within the software
 * architecture of your program.
 *
 * @version $Revision: 1.0 $ $Name:  $
 */

/* @section  I N C L U D E S */
#include "reg_c51.h"

bit transmit_completed;
char serial_data;

/**
 * FUNCTION_PURPOSE: This file set up spi in slave mode with 
 * Fclk Periph/128 as baud rate and with slave select pin
 * FUNCTION_INPUTS: P1.5(MISO) serial input
 *                  P1.1(/SS)=0 slave selected    
 * FUNCTION_OUTPUTS: P1.7(MOSI) serial output
 */
void main(void)
{
SPCON &= ~0x10;               /* slave mode */
SPCON &= ~0x08;               /* CPOL=0; transmit mode example*/
SPCON |= 0x04;                /* CPHA=1; transmit mode example*/
IEN1 |= 0x04;                 /* enable spi interrupt */
SPCON |= 0x40;                /* spi run */
transmit_completed = 0;       /* clear software transfert flag */
EA=1;                         /* enable interrupts */

while(1)	     					   /* endless  */
{
if(transmit_completed)
   {
   SPDAT = serial_data;       /* echo data to master */
   transmit_completed = 0;    /* clear software transfert flag */
   }
}
}


/**
 * FUNCTION_PURPOSE: spi interrupt, receive data to master
 * FUNCTION_INPUTS: void
 * FUNCTION_OUTPUTS: void
 */
void it_SPI(void) interrupt 9 /* interrupt address is 0x004B */
{
	switch	( SPSTA )         /* read and clear spi status register */
	{
		case 0x80:
         serial_data=SPDAT;   /* read receive data */
         transmit_completed=1;/* set software flag */
 		break;

		case 0x10:
         /* put here for mode fault tasking */	
		break;
	
		case 0x40:
         /* put here for overrun tasking */	
		break;
	}
   SPDAT=serial_data;        /* needed to complete clearing sequence */
}



