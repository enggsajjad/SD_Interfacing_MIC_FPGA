//Programmer:	Sajjad Hussain, JE.
#include "at89c51rc2.h"
#define Putc LcdWriteChar
sbit RS = P1^1; 
sbit RW = P1^2;
sbit EN = P1^3;
sbit BS = P0^7;

unsigned char Key,RecChar,i,g,status;
bit isInit=0,spi_tx_done=0;
char serial_data;
unsigned char data_example='A',spi_bytes,spi_tx_bytes;
char data_save;
bit spi_intr = 0;

//Function Prototypes
void LcdInit();
void DelayUs();
//void LcdBusy(void);
void LcdClear(void);
void LcdWriteCmd(unsigned char c);
void LcdWriteChar(unsigned char var);
void LcdWriteStr(char *var);
void LcdGotoXY(unsigned char r,unsigned char c);
void SendChar(unsigned char c);
void SendStr(char *s);

void main()
{
	EX0 = 1;
	IT0=1;
   SPCON = 0x10;                /* Master mode */
	//SPCON |= 0x82;                /* Fclk Periph/128 */
	SPR0=0;SPR1=0;SPR2=0;
	SPCON |= 0x20;                /* P1.1 is available as standard I/O pin */
	SPCON &= ~0x08;               /* CPOL=0; transmit mode example */
	SPCON |= 0x04;                /* CPHA=1; transmit mode example */
	IEN1 |= 0x04;                 /* enable spi interrupt */
	SPCON |= 0x40;                /* run spi */
	EA=1;                         /* enable interrupts */

	LcdInit();
	LcdGotoXY(1,4);
	LcdWriteStr("SPI Master");
	LcdGotoXY(2,2);
	LcdWriteStr("Multi Byte Test");	
	isInit = 1;
	while(1)
	{
		if(spi_intr)
		{
		 	spi_intr = 0;
	 		//LcdClear();
	 		LcdGotoXY(1,spi_tx_bytes+1);
			switch(status)
			{
			case 0x80:
		      serial_data=SPDAT;
   	      Putc(serial_data);
   	      LcdGotoXY(2,spi_tx_bytes+1);
   	      Putc(spi_tx_bytes+48);
   	      if(spi_tx_bytes<spi_bytes)
   	      {
   	      	spi_tx_done = 1;
   	      	spi_tx_bytes++;
   	      	data_example++;
   	      }
 				break;
			case 0x10:
      	   Putc('F');
				break;
			case 0x40:
      	   Putc('O');
				break;
			}//switch
		}//spi_intr
		if(spi_tx_done)
		{
			spi_tx_done = 0;
			SPDAT = data_example;
		}//spi_done
		
	}//while
}//main

void it_SPI(void) interrupt 9 /* interrupt address is 0x004B */
{

	status = SPSTA;
	spi_intr = 1;
}

void KeyInt() interrupt 0 
{  
	if(isInit)
	{
		Key = P2&0x1F;
		switch(Key)
		{
		case 0:
			spi_tx_done = 0;
			spi_bytes = 15;
			spi_tx_bytes = 0;
		   SPDAT=data_example;
			break;
		case 1:
			data_example++;
			break;
		case 2:
			LcdClear();
			data_example='A';
			break;
		case 3:
			break;
		case 4:
			break;
		default:
			break;
		}
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
void LcdInit()
{
   LcdWriteCmd(0x38);	//Function Set 0x38
   LcdWriteCmd(0x38);	//Function Set
   LcdWriteCmd(0x38);	//Function Set
	LcdWriteCmd(0x06);	//Entry Mode Set 0x06
   LcdWriteCmd(0x0C);	//Display On  Off Control 0x0C
}

//Function Definitions
void DelayUs(void)
{
	//TL1 = 0x48;TH1 = 0xFF;//100 usec at 22.1184MHz
	//TL1 = 0xA4;TH1 = 0xFF;//50 usec at 22.1184MHz
	//TL1 = 0xD2;TH1 = 0xFF;//25 usec at 22.1184MHz
	//TL1 = 0xF7;TH1 = 0xFF;//5 usec at 22.1184MHz
	TL1 = 0xEE;TH1 = 0xFF;//10 usec at 22.1184MHz
	TR1  = 1;
	while(!TF1);
	TR1  = 0;
	TF1  = 0;
}
/*
void LcdReset()
{
	P0 = 0xFF;
	for(g=0;g<100;g++)
		DelayUs();
	P0 = 0x03+EN;
	P0 = 0x03;
	for(g=0;g<50;g++)
		DelayUs();
	P0 = 0x03+EN;
	P0 = 0x03;
	for(g=0;g<5;g++)
		DelayUs();
	P0 = 0x03+EN;
	P0 = 0x03;
	for(g=0;g<5;g++)
		DelayUs();
	P0 = 0x02+EN;
	P0 = 0x02;
	for(g=0;g<5;g++)
		DelayUs();
}
 
void LcdWriteCmd(unsigned char cmd)
{ 
	P0 = ((cmd >> 4) & 0x0F)+EN;
	P0 = ((cmd >> 4) & 0x0F);

	P0 = (cmd & 0x0F)+EN;
	P0 = (cmd & 0x0F);

	for(g=0;g<2;g++)
		DelayUs();
}

void LcdWriteChar (unsigned char dat)
{ 
	P0 = (((dat >> 4) & 0x0F)+EN+RS);
	P0 = (((dat >> 4) & 0x0F)+RS);
	
	P0 = ((dat & 0x0F)+EN+RS);
	P0 = ((dat & 0x0F)+RS);

	for(g=0;g<2;g++)
		DelayUs();
}
*/
 //8bit Interface
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
void LcdClear()
{
 unsigned char h;
 LcdWriteCmd(0x01);
 for(h=0;h<100;h++)
		DelayUs();
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







































































































































































































































