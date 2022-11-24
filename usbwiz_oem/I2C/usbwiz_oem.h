#define Putc LcdWriteChar
//Pins Assignments
sbit RS 		= P3^6;
sbit BS 		= P0^7;
sbit RW 		= P1^2;
sbit EN		= P1^3;
sbit I2C_READY = P1^4;
sbit I2C_BUSY 	= P1^5;
sbit I2C_SCL	= P1^6;
sbit I2C_SDA	= P1^7;

unsigned char Key,state,Next;
unsigned char temp=0;
unsigned int dly;
unsigned int h;
bit isInit=0;
bit keyflag = 0;
bit rising=0;
bit falling=0;
bit Flag = 0;
//Function Prototypes
void Init();
void Delay5ms(void);
void Delay30ms(void);
void LcdClear(void);
void SD_WriteCmd(char *cmd);
void LcdWriteStr(char *var);
void LcdWriteCmd(unsigned char c);
void LcdWriteChar(unsigned char var);
void SD_Write(unsigned char val);
unsigned char SD_Read(void);
void LcdGotoXY(unsigned char r,unsigned char c); 

void SD_WriteCmd(char *cmd)
{
 	while(*cmd)
 	{
 		SD_Write(*cmd);
 	 	cmd++;
 	}
}
void Delay5ms(void)
{
	TL0 = 0x00;TH0 = 0xDC;
	TR0  = 1;
	while(!TF0);
	TR0  = 0;
	TF0  = 0;
}
void Delay30ms(void)
{
	TL0 = 0x00;TH0 = 0x28;
	TR0  = 1;
	while(!TF0);
	TR0  = 0;
	TF0  = 0;
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
		case 3: LcdWriteCmd(0x94 + col - 1); break;	//0x90 for 4x16
		case 4: LcdWriteCmd(0xd4 + col - 1); break;	//0xd0 for 4x16
		default: break;
	}
}

void LcdClear(void)
{
 LcdWriteCmd(0x01);
 Delay5ms();
}

void I2C_delay(void)
{
	unsigned char i;
	for(i=0; i<15; i++);
}
void I2C_clock(void)
{
	I2C_delay();
	I2C_SCL = 1;		// Start clock  
	I2C_delay();    
	I2C_SCL = 0;		// Clear I2C_SCL  
}
void I2C_start(void)
{
	if(I2C_SCL)
	I2C_SCL = 0;		// Clear I2C_SCL  
	I2C_SDA = 1;        // Set I2C_SDA  
	I2C_SCL = 1;		// Set I2C_SCL  
	I2C_delay(); 
	I2C_SDA = 0;        // Clear I2C_SDA  
	I2C_delay(); 
	I2C_SCL = 0;        // Clear I2C_SCL  
}
 
void I2C_stop(void)
{
	if(I2C_SCL)	
	I2C_SCL = 0;			// Clear I2C_SCL  
	I2C_SDA = 0;			// Clear I2C_SDA  
	I2C_delay();
	I2C_SCL = 1;			// Set I2C_SCL  
	I2C_delay();
	I2C_SDA = 1;			// Set I2C_SDA  
}
 
bit I2C_write(unsigned char dat)
{
	bit data_bit;		
	unsigned char i;	
	for(i=0;i<8;i++)				// For loop 8 time(send data 1 byte)  
	{
		data_bit = dat & 0x80;		// Filter MSB bit keep to data_bit  
		I2C_SDA = data_bit;				// Send data_bit to I2C_SDA  
		I2C_clock();      			// Call for send data to i2c bus  
		dat = dat<<1;  
	}
	I2C_SDA = 1;			// Set I2C_SDA  
	I2C_delay();	
	I2C_SCL = 1;			// Set I2C_SCL  
	I2C_delay();	
	data_bit = I2C_SDA;   	// Check acknowledge  
	I2C_SCL = 0;			// Clear I2C_SCL  
	I2C_delay();
	return data_bit;	// If send_bit = 0 i2c is valid  		 	
}
 
unsigned char I2C_read(void)
{
	bit rd_bit;	
	unsigned char i, dat;
	dat = 0x00;	
	for(i=0;i<8;i++)		// For loop read data 1 byte  
	{
		I2C_delay();
		I2C_SCL = 1;			// Set I2C_SCL  
		I2C_delay(); 
		rd_bit = I2C_SDA;		// Keep for check acknowledge	 
		dat = dat<<1;		
		dat = dat + rd_bit;	// Keep bit data in dat  
		I2C_SCL = 0;			// Clear I2C_SCL  
	}
	return dat;
}
void I2C_noack()
{
	I2C_SDA = 1;		// Set I2C_SDA  
	I2C_delay();
	I2C_clock();	// Call for send data to i2c bus  
	I2C_SCL = 1;		// Set I2C_SCL  
}
unsigned char SD_Read(void)
{
	unsigned char dat;	
	I2C_start();            // Start i2c bus  
	I2C_write(0xA5);			// Connect to EEPROM for Read  
	dat = I2C_read();			// Receive data  
	I2C_noack();
	I2C_stop();					// Stop i2c bus  
	//Putc(dat);
   return dat;			
}
void SD_Write(unsigned char val)
{
	
	I2C_start(); 
	I2C_write(0xA4);   						// Connect to EEPROM  
	I2C_write(val);						// Write sec on RAM specified address
	I2C_stop();           					// Stop i2c bus
	while(I2C_BUSY);
}

