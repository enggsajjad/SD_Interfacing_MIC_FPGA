/**
 * @file $RCSfile: spi_master_ss.c,v $
 *
 * Copyright (c) 2004 Atmel.
 *
 * Please read file license.txt for copyright notice.
 *
 * @brief This file is an example to use spi in master mode.
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
char serial_data;
char data_example=0x55;
char data_save;
bit transmit_completed= 0;

/**
 * FUNCTION_PURPOSE: This file set up spi in master mode with 
 * Fclk Periph/128 as baud rate and with slave select pin.
 * FUNCTION_INPUTS: P1.5(MISO) serial input
 * FUNCTION_OUTPUTS: P1.7(MOSI) serial output
 */
void main(void)
{
SPCON |= 0x10;                /* Master mode */
P1_1=1;                       /* enable master */
SPCON |= 0x82;                /* Fclk Periph/128 */
SPCON &= ~0x08;               /* CPOL=0; transmit mode example */
SPCON |= 0x04;                /* CPHA=1; transmit mode example */
IEN1 |= 0x04;                 /* enable spi interrupt */
SPCON |= 0x40;                /* run spi */
EA=1;                         /* enable interrupts */

	while(1)			   			/* endless  */
   {
   SPDAT=data_example;        /* send an example data */
   while(!transmit_completed);/* wait end of transmition */
   transmit_completed = 0;    /* clear software transfert flag */

   SPDAT=0x00;                /* data is send to generate SCK signal */
   while(!transmit_completed);/* wait end of transmition */
   transmit_completed = 0;    /* clear software transfert flag */
   data_save = serial_data;   /* save receive data */  
   }

}


/**
 * FUNCTION_PURPOSE:interrupt
 * FUNCTION_INPUTS: void
 * FUNCTION_OUTPUTS: transmit_complete is software transfert flag
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
}


