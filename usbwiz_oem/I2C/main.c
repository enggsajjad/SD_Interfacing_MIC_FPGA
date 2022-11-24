/*            Programmer: Sajjad Hussain S.E.
            Dated: 	21-05-2009
*/
#include "at89c51rc2.h"
#include "usbwiz_oem.h"
#include <intrins.h>
// Main Program Starts Here
void main()
{
	state = 1;
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
	
	isInit = 1;
	//Running Forever
	while(1)
	{
	 	while(I2C_READY)
		 	SD_Read();
	}
}//main


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
	 	 		SD_WriteCmd("SD\r");
	 	 		break;
	 	 	case 2:
	 	 		SD_WriteCmd("FM S\r");
	 	 		break;
	 	 	case 3:
	 	 		SD_WriteCmd("OF 0W>TEST.TXT\r");
	 	 		break;
	 	 	case 4:
	 	 		SD_WriteCmd("WF 0>C350\r");
	 	 		break;
	 	 	case 5:
	 	 		for(h=0;h<50000;h++)
	 	 			SD_Write(48);
	 	 		//SD_WriteCmd("123456789");
	 	 		break;
	 	 	case 6:
	 	 		SD_WriteCmd("CF 0\r");
	 	 		break;
	 	 	case 12:
	 	 		LcdClear();
	 	 		break;
		}		
	}
}







































































































