// Main Program Starts Here
#include <at89c51rc2.h>
#include "ratemeter.h"
void main()
{
	for (i=0; i<200; i++)
		DelayOneMs();
	//SelectLines(1);
	
	EX0 = 1;
	IT0=1;

	// Set Timer1 used for Delay
	TMOD = 0x12;
        
	// Set Serial  Port Settings
   SCON 	= 0x50;
   BDRCON&=0xEC;
   BDRCON|=0x0C;
   BRL	=0xFA;	//9600
   BDRCON|=0x10;
	//ES = 1;
	EA = 1;  
	
   // Initial Screen
	LcdInit();
	LcdGotoXY(1,3);
	LcdWriteStr("USBWIZ_OEM_UART");
   
   while(Getc()!='!');
   LcdGotoXY(2,3);
   LcdWriteStr("Good!");
   Putc(Getc());
   Putc(Getc());
   SendStr("I\r");
   while(Getc()!='!');
   Putc(Getc());
   Putc(Getc());
   SendStr("B 43F2\r");
   while(Getc()!='!');
   Putc(Getc());
   Putc(Getc());
   SetBaudRate(0);
   while(Getc()!='!');
   Putc(Getc());
   Putc(Getc());

	isInit = 1;
	//Running Forever
	while(1)
	{
		if(rxflag)
		{
		   rxflag = 0;
		   LcdGotoXY(1,1);
		   Putc(RecChar);
		}	
	}//while
}//main

void Serial() interrupt 4
{
	if (RI)
	{
		RI = 0;
		RecChar = SBUF;
		rxflag = 1;
	}//end RI

}

void KeyInt() interrupt 0 
{  
	if(isInit)
	{
		Key = P2&0x1F;
		
		switch(Key)
		{
		case 0:
			Putc('0');
			break;
		case 1:
			break;
		default:
			break;
		}
	}
}























































