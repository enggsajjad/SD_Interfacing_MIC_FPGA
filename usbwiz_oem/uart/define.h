#define Putc LcdWriteChar

sbit RW = P1^2;
sbit EN = P1^3;
sbit BS = P0^7;
sbit RS 	= P3^6;

/*
sbit SEL0 = P2^7;
sbit SEL1 = P2^6;
sbit SEL2 = P2^5;
sbit BLT = P1^0; 
sbit RS  = P1^1; 
sbit RW  = P1^2;
sbit EN  = P1^3;
sbit BS  = P0^7;
*/
unsigned char Key;
unsigned char temp;
unsigned char RecChar;
unsigned char i;
bit isInit=0;
bit rxflag=0;

//Function Prototypes
void LcdInit();
void DelayOneMs(void);
void LcdClear(void);
void SendStr(char *s);
void LcdWriteStr(char *var);
void SendChar(unsigned char c);
void LcdWriteCmd(unsigned char c);
void LcdWriteChar(unsigned char var);
void LongToAscii(unsigned long Value);
void LcdGotoXY(unsigned char r,unsigned char c);

