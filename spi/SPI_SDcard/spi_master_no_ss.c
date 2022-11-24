#include "at89c51rc2.h"
unsigned char serial_data,cnt;
unsigned char data_example=0x55;
unsigned char data_save;
bit transmit_completed= 0,isInit=0;
unsigned char Key1;
unsigned char count;
//sbit P11 = P1^1;
//LCD Interfacing Pins
sbit RS = P1^1; 
sbit RW = P1^2;
sbit EN = P1^3;
sbit BS = P0^7;
//Clear Display Command
const unsigned char CMD_CLR = 0x01;
//Return Home Command
const unsigned char CMD_HOM = 0x02;
//Entry Mode Set Commands
const unsigned char CMD_ETR = 0x04;
const unsigned char CMD_ETR_S = 0x01;
const unsigned char CMD_ETR_ID = 0x02;
//Display On  Off Control Commands
const unsigned char CMD_DSP = 0x08;
const unsigned char CMD_DSP_B = 0x01;
const unsigned char CMD_DSP_C = 0x02;
const unsigned char CMD_DSP_D = 0x04;
//Cursor & Display Shift Commands
const unsigned char CMD_SHT = 0x10;
const unsigned char CMD_SHT_RL = 0x04;
const unsigned char CMD_SHT_SC = 0x08;
//Function Set Commands
const unsigned char CMD_FN = 0x20;
const unsigned char CMD_FN_F = 0x04;
const unsigned char CMD_FN_N = 0x08;
const unsigned char CMD_FN_DL = 0x10;
//Set CG RAM Address Command
const unsigned char CMD_CG_RAM = 0x40;
//Set DD RAM Address Command
const unsigned char CMD_DD_RAM = 0x80;
//Function Prototypes
void LcdInit();
void LcdBusy(void);
void LcdWriteCmd(unsigned char c);
void LcdWriteChar(unsigned char var);
void LcdWriteStr(unsigned char *var);
void LcdGotoXY(unsigned char r,unsigned char c);
//Function Definitions
void LcdInit()
{
   LcdWriteCmd(CMD_FN|CMD_FN_DL|CMD_FN_N);	//Function Set 0x38
   LcdWriteCmd(CMD_FN|CMD_FN_DL|CMD_FN_N);	//Function Set
   LcdWriteCmd(CMD_FN|CMD_FN_DL|CMD_FN_N);	//Function Set
	LcdWriteCmd(CMD_ETR|CMD_ETR_ID);				//Entry Mode Set 0x06
   LcdWriteCmd(CMD_DSP|CMD_DSP_D);				//Display On  Off Control 0x0C
}
void LcdBusy()
{
	BS   = 1;			//Make D7th bit of LCD as i/p
   EN   = 1;         //Make port pin as o/p
   RS   = 0;         //Selected command register
   RW   = 1;         //We are reading
   while(BS)
   {   					//read busy flag again and again till it becomes 0 Enable H->L
   	EN   = 0;
      EN   = 1;
   }
}
void LcdWriteCmd(unsigned char var)
{
	P0 = var;      	//Commands to be Written
   RS   = 0;        	//Selected command register
   RW   = 0;        	//We are writing in instruction register
   EN   = 1;        	//Enable H->L
   EN   = 0;
   LcdBusy();      //Wait for LCD to process the command
}
void LcdWriteChar(unsigned char var)
{
	P0 = var;      	//Data/Character to be Written
   RS   = 1;         //Selected data register
   RW   = 0;         //We are writing
   EN   = 1;         //Enable H->L
   EN   = 0;
   LcdBusy();      //Wait for LCD to process the command
}
void LcdWriteStr(unsigned char *var)
{
	while(*var)       //till string ends send characters one by one
   	LcdWriteChar(*var++);
}

void LcdGotoXY(unsigned char row, unsigned char col)
{
	LcdWriteCmd(CMD_DD_RAM | (row << 6) | col);
}

void main(void)
{
LcdInit();
LcdWriteCmd(0x01);
LcdWriteStr("Insha Allah!");
IT0 = 1;
EX0 = 1;
SPCON |= 0x10;                /* Master mode */
SPCON |= 0x82;                /* Fclk Periph/128 */
SPCON |= 0x20;                /* P1.1 is available as standard I/O pin */
//SPCON &= ~0x08;               /* CPOL=0; transmit mode example */
SPCON |= 0x08;               /* CPOL=1; IDLE HIGH */
//SPCON |= 0x04;                /* CPHA=1; transmit mode example */
SPCON &= ~0x04;                /* CPHA=0; POSITIVE EDGE TRIGGERED */
IEN1 |= 0x04;                 /* enable spi interrupt */


CMOD = 0x03;
CH = 0;
CL = 0;
CCAPM1 = 0x21;
IEN0 = IEN0 | 0x41;
CR = 1;
SPCON |= 0x40;                /* run spi */	
EA=1;                         /* enable interrupts */

isInit = 1;

	while(1)			   			/* endless  */
   {
   	if(transmit_completed)
   	{
   	 	transmit_completed = 0;
   	 	switch(cnt)
   	 	{
   	 		case 1:
   	 			//SPDAT = 13;
   	 			break;
   	 		case 2:
   	 			//SPDAT = 0;
   	 			break;
   	 		case 3:
   	 			//SPDAT = 0;
   	 			break;
   	 		case 4:
   	 			//SPDAT = 0;
   	 			break;
   	 		default:
   	 			break;
   	 	}
   	 cnt++;
   	}
   //P11=~P11;                /* P1.1 is available as standard I/O pin */ 
   //SPDAT=data_example;        /* send an example data */
   //while(!transmit_completed);/* wait end of transmition */
   //transmit_completed = 0;    /* clear software transfert flag */

   //SPDAT=0x00;                /* data is send to generate SCK signal */
   //while(!transmit_completed);/* wait end of transmition */
   //transmit_completed = 0;    /* clear software transfert flag */
   //data_save = serial_data;   /* save receive data */  
   }

}
void PCATimers() interrupt 6  using 1
{
	
	if(CCF1)
	{
		///IEN0 = IEN0 & 0xBF;
		CCF1 = 0;
		cnt =1;
		SPDAT = 0xFF;
		count++;
		//IEN0 = IEN0 | 0x40;
	}
}
void KeyInt() interrupt 0 
{  
	if(isInit)
	{
		Key1 = P2&0x1F;
		LcdWriteCmd(0x01);
		LcdGotoXY(0,0);
		//LcdWriteChar(Key1+48);
		switch(Key1)
		{
		case 0:
			count = 0;
			break;
		case 1:
			SPDAT=0xFF;
			cnt =1;
   		//while(!transmit_completed);
	   	//transmit_completed = 0;
	   	
	   	//SPDAT=13;
   		//while(!transmit_completed);
	   	//transmit_completed = 0;
			break;
		case 2:
			LcdWriteChar(count+48);
			break;
		case 3:
			SPDAT='I';
			cnt =1;
			break;
		 }
	 }
}
void it_SPI(void) interrupt 9 /* interrupt address is 0x004B */
{
	switch	( SPSTA )         /* read and clear spi status register */
	{
		case 0x80:
       serial_data=SPDAT;   /* read receive data */
		 transmit_completed=1;/* set software flag */
		 LcdWriteChar(serial_data);
		 LcdWriteChar(' ');
 		break;

		case 0x10:
         /* put here for mode fault tasking */	
		break;
	
		case 0x40:
         /* put here for overrun tasking */	
		break;
	}
}













































