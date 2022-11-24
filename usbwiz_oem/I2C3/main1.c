/*				Digital Rate Meter Ranging 0.001Hz  --  1000Hz
				Using PCA Interrupt Only
				Handling Noise Pulse
				Control Action Performed
            Programmer: Sajjad Hussain S.E.
            Dated: 	21-05-2009
*/
#include "at89c51rc2.h"
#include "usbwiz_oem.h"
#include <intrins.h>
// Main Program Starts Here
void main()
{

	TMOD = 0x11;

	IT0 = 1;
	EX0 = 1;
	EA = 1;
   // Initial Screen
	LcdInit();
	LcdGotoXY(1,1);
	Delay5ms();
	
	while(I2C_READY)
		SD_Read(); 	
	Putc('I');
	
	CCON = 0x00;
	CMOD = 0x00;
	CCAPM1 = 0x21;// Capture +ve Edge
	//EC = 1;
	state = 0;
	isInit = 1;
	//Running Forever
	while(1)
	{
		switch(state)
		{
		 	case 0: //idle
		 		Putc('A');
		 		break;
		 	case 1:
		 		
		 		CCAPM1 = 0x00;
		 		while(SD_Read()!=13);
				CCAPM1 = 0x21;
				Putc('B');
				state = Next;
				break;
			case 2:
				
				SD_WriteCmd("FM S\r");
				Putc('C');
				Next = 3;
				break;
			case 3:
				Putc('D');
				break;
		}//sw
	}//wh
}//main

void PCATimers() interrupt 6  using 1
{

	if(isInit&&CCF1)
	{
		CCF1 = 0;
		//rising = 1;
		state = 1;
	}
}

void KeyInterrupt() interrupt 0
{
	Key = P2&0x1F;
	if(isInit)
	{
	 	switch(Key)
	 	{
	 	 	case 0:
	 	 		LcdClear();
	 	 		Putc(SD_Read());
	 	 		Putc(SD_Read());
	 	 		Putc(SD_Read());
	 	 		Putc(SD_Read());
	 	 		break;	 	 	
	 	 	case 1:
 	 			Next = 2;
 	 			EC = 1;
	 	 		SD_WriteCmd("SD\r");
	 	 		break;
	 	 	case 2:
	 	 		SD_WriteCmd("FM S\r");
	 	 		break;
	 	 	case 3:
	 	 		SD_WriteCmd("OF 0W>TEST.TXT\r");
	 	 		break;
	 	 	case 4:
	 	 		SD_WriteCmd("WF 0>9\r");
	 	 		break;
	 	 	case 5:
	 	 		//for(h=0;h<100;h++)
	 	 		//	SD_Write(h+48);
	 	 		SD_WriteCmd("123456789");
	 	 		break;
	 	 	case 6:
	 	 		SD_WriteCmd("CF 0\r");
	 	 		break;
	 	 	case 7:
	 	 		SD_Write('S');
	 	 		SD_Write('D');
	 	 		SD_Write(13);
				for(dly=0;dly<100;dly++)
					Delay30ms();
				while(I2C_READY)
		 			SD_Read();
		 			
	 	 		SD_Write('F');
	 	 		SD_Write('M');
	 	 		SD_Write(' ');
	 	 		SD_Write('S');
	 	 		SD_Write(13);
				for(dly=0;dly<100;dly++)
					Delay30ms();
				while(I2C_READY)
		 			SD_Read();
		 			

	 	 		SD_Write('O');
	 	 		SD_Write('F');
	 	 		SD_Write(' ');
	 	 		SD_Write('0');
	 	 		SD_Write('W');
	 	 		SD_Write('>');
	 	 		SD_Write('T');
	 	 		SD_Write('E');
	 	 		SD_Write('S');
	 	 		SD_Write('T');
	 	 		SD_Write('.');
	 	 		SD_Write('T');
	 	 		SD_Write('X');
	 	 		SD_Write('T');
	 	 		SD_Write(13);
				for(dly=0;dly<100;dly++)
					Delay30ms();
				while(I2C_READY)
		 			SD_Read();
	 	 		
	 	 		SD_Write('W');
	 	 		SD_Write('F');
	 	 		SD_Write(' ');
	 	 		SD_Write('0');
	 	 		SD_Write('>');
	 	 		//SD_Write('3');
	 	 		//SD_Write('E');
	 	 		//SD_Write('8');
	 	 		SD_Write('6');
	 	 		SD_Write('4');
	 	 		SD_Write(13);
				for(dly=0;dly<100;dly++)
					Delay30ms();
				while(I2C_READY)
		 			SD_Read();
					
	 	 		for(h=0;h<100;h++)
	 	 			SD_Write(h%255);
				for(dly=0;dly<100;dly++)
					Delay30ms();
				while(I2C_READY)
		 			SD_Read();
	 	 		/*
	 	 		SD_Write('0');
	 	 		SD_Write('1');
	 	 		SD_Write('2');
	 	 		SD_Write('3');
	 	 		SD_Write('4');
	 	 		SD_Write('5');
	 	 		SD_Write('6');
	 	 		SD_Write('7');
	 	 		SD_Write('8');
	 	 		SD_Write('9');
	 	 		SD_Write('A');
	 	 		SD_Write('B');
	 	 		SD_Write('C');
	 	 		SD_Write('D');
	 	 		SD_Write('E');
	 	 		SD_Write('F');
	 	 		*/
	 	 		SD_Write('C');
	 	 		SD_Write('F');
	 	 		SD_Write(' ');
	 	 		SD_Write('0');
	 	 		SD_Write(13);
				for(dly=0;dly<100;dly++)
					Delay30ms();
				while(I2C_READY)
		 			SD_Read();
	 	 		Putc('7');
	 	 		break;
	 	 	case 8:
	 	 		break;
	 	 	case 9:
	 	 		break;
	 	 	case 12:
	 	 		LcdClear();
	 	 		break;
		}		
	}
}

















































































































