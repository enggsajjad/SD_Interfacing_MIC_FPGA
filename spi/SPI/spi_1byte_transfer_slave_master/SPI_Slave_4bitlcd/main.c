//LCD Interface 8 bit; using Busy Flag Technique
//Programmer:	Sajjad Hussain, JE.
#include "at89c51rc2.h"
sbit BKLT = P2^3; 
sbit RS  = P2^4; 
sbit RW  = P2^5;
sbit EN  = P2^6;
sbit LED = P2^7;

sbit BS  = P0^7;

#define Putc LcdWriteChar

bit isInit=0;
unsigned char RecChar,g,temp;
bit spi_intr=0;
char serial_data;

//Function Prototypes
void LcdInit();
//void LcdBusy(void);
void LcdReset();
void DelayUs(void);
void LcdDelayMS(void);
void LcdClear(void);
void LcdWriteCmd(unsigned char c);
void LcdWriteChar(unsigned char var);
void LcdWriteStr(char *var);
void LcdGotoXY(unsigned char r,unsigned char c);
void SendChar(unsigned char c);
void SendStr(char *s);


void main()
{
	
	LcdInit();
	LcdClear();
	LcdGotoXY(1,4);
	LcdWriteStr("SPI Slave");
	LcdGotoXY(2,4);
	LcdWriteStr("1 Byte Test");
	
	EX0 = 1;
	IT0=1; 
   SPDAT = '3';
   SPCON = 0x20;                /* P1.1 is available as standard I/O pin */
	SPCON &= ~0x10;               /* slave mode */
	SPCON &= ~0x08;               /* CPOL=0; transmit mode example*/
	SPCON |= 0x04;                /* CPHA=1; transmit mode example*/
	//IEN1 |= 0x04;                 /* enable spi interrupt */
	//SPCON |= 0x40;                /* spi run */
	EA=1;                         /* enable interrupts */

	isInit = 1;
	while(1)
	{
		if(spi_intr)
		{
			spi_intr = 0;
			LcdClear();
			LcdGotoXY(1,1);
			switch	( temp )         /* read and clear spi status register */
			{
				case 0x80:
      	   serial_data=SPDAT;   /* read receive data */
   	      Putc(serial_data);
 				break;
			case 0x10:
   	      /* put here for mode fault tasking */	
				Putc('F');
				break;
			case 0x40:
   	      /* put here for overrun tasking */	
      	   Putc('O');
				break;
			}
		SPDAT = serial_data+32;
		}//spi_intr
	}//while
}//main

void it_SPI(void) interrupt 9 /* interrupt address is 0x004B */
{
	
	temp = SPSTA;
	spi_intr = 1;
}
void KeyInt() interrupt 0 
{  
	if(isInit)
	{
	LcdClear();
	LcdGotoXY(2,8);
	LcdWriteStr("Ready");
	IEN1 |= 0x04;                 /* enable spi interrupt */
	SPCON |= 0x40;                /* spi run */
	}//if Init
}

void Serial() interrupt 4
{
	if (RI)
	{
		RI = 0;
    	RecChar = SBUF;
	}
}
void LcdClear()
{
 LcdWriteCmd(0x01);
}
//Function Definitions
void DelayUs(void)
{
	TL1 = 0x90;TH1 = 0xFE;//200 usec at 22.1184MHz
	//TL1 = 0x48;TH1 = 0xFF;//100 usec at 22.1184MHz
	//TL1 = 0xA4;TH1 = 0xFF;//50 usec at 22.1184MHz
	//TL1 = 0xD2;TH1 = 0xFF;//25 usec at 22.1184MHz
	//TL1 = 0xF7;TH1 = 0xFF;//5 usec at 22.1184MHz
	//TL1 = 0xEE;TH1 = 0xFF;//10 usec at 22.1184MHz
	TR1  = 1;
	while(!TF1);
	TR1  = 0;
	TF1  = 0;
}
//8bit Interface
void LcdInit()
{
   LcdWriteCmd(0x38);	//Function Set 0x38
   LcdWriteCmd(0x38);	//Function Set
   LcdWriteCmd(0x38);	//Function Set
	LcdWriteCmd(0x06);	//Entry Mode Set 0x06
   LcdWriteCmd(0x0C);	//Display On  Off Control 0x0C
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
void LcdWriteStr(char *var)
{
	while(*var)       //till string ends send characters one by one
   	LcdWriteChar(*var++);
}

void LcdGotoXY(unsigned char row, unsigned char col)
{
	//LcdWriteCmd(0x80 | (row << 6) | col);
	switch (row)
	{
		case 1: LcdWriteCmd(0x80 + col - 1); break;
		case 2: LcdWriteCmd(0xc0 + col - 1); break;
		case 3: LcdWriteCmd(0x94 + col - 1); break;
		case 4: LcdWriteCmd(0xd4 + col - 1); break;
		default: break;
	}
}

void SendChar(unsigned char c)
{

	TI=1;
	while (!TI);	TI=0;	SBUF = c;
	while (!TI);	TI=0;
}
void SendStr(char *s)
{
	while (*s)
	{
		SendChar(*s);
		s++;
	}
}
































































































































































































































