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
   state = 0;
	TMOD = 0x11;

	IT0 = 1;
	EX0 = 1;
	EA = 1;
   // Initial Screen
	LcdInit();
	LcdGotoXY(1,1);
	
	while(I2C_READY)
		SD_Read(); 	
	Delay5ms();
	Putc('I');
	
	CCON = 0x00;
	CMOD = 0x00;
	CCAPM1 = 0x21;
	EC = 1;
	isInit = 1;
	//Running Forever
	while(1)
	{
		if(sdRead_Start)
		{
		 	sdRead_Start = 0;
		 	CCAPM1 = 0x00;
		 	while(SD_Read()!=13);
			CCAPM1 = 0x21;
			sdRead_End = 1;	
			//Putc('A');
		}
		if(sdRead_End)
		{
		 	sdRead_End = 0;
		 	switch(state)
		 	{
		 		case 0:
		 			break;
		 		case 1:
		 		 	Putc('B');
		 		 	state = 2;
		 			SD_WriteCmd("FM S\r");
		 			break;
		 		case 2:
		 			Putc('C');
		 			state = 3;
		 			SD_WriteCmd("OF 0W>TEST.TXT\r");
		 			break;
		 		case 3:
		 			Putc('D');
		 			state = 4;
		 			//SD_WriteCmd("WF 0>9\r");
		 			//SD_WriteCmd("WF 0>64\r");	//100
		 			//SD_WriteCmd("WF 0>3E8\r");	//1000
		 			//SD_WriteCmd("WF 0>2710\r");	//10000
		 			SD_WriteCmd("WF 0>C350\r");	//50000
		 			break;
		 		case 4:
		 			Putc('E');
		 			state = 5;
		 			//SD_WriteCmd("012345678");
		 			for(h=0;h<50000;h++)
		 				SD_Write('1');
		 			break;
		 		case 5:
		 			Putc('F');
		 			sdRead_Start = 1;
		 			state = 6;
		 			break;
		 		case 6:
		 			Putc('G');
		 			state = 7;
		 			SD_WriteCmd("CF 0\r");
		 			break;
		 		case 7:
		 			break;
		 	}//state
		}//sdRead_End
	}//wh
}//main

void PCATimers() interrupt 6  using 1
{
	if(isInit&&CCF1)
	{
		CCF1 = 0;
		sdRead_Start = 1;
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
	 	 		state = 1;
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

































































































































