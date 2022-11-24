#include"define.h"
void SelectLines(unsigned char sel)
{
	//SEL0 = (bit)(sel & 0x01);
	//SEL1 = (bit)((sel & 0x02)>>1);
	//SEL2 = (bit)((sel & 0x04)>>2);
	DelayOneMs();
}
void SetBaudRate(bit low)
{
   BDRCON 	&=0xEC;
   BDRCON 	|=0x0C;
   if(low)
   	BRL		=-6;
   else
   	BRL		=-1;
   BDRCON 	|=0x10;
}
void DelayOneMs(void)
{
	TL1 = 0xCD;
	TH1 = 0xF8;
	TR1  = 1;
	while(!TF1);
	TR1  = 0;
	TF1  = 0;
}

// 8bit LCD Interface
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
 LcdWriteCmd(0x01);
 DelayOneMs();
 DelayOneMs();
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
unsigned char Getc(void)
{
unsigned char c;
while(!RI);	c = SBUF; RI = 0;
return(c);
} 
