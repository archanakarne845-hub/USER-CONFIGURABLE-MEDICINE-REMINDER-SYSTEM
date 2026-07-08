void delay_us(unsigned int dlyus);
void delay_ms(unsigned int dlyms);
void writeLCD(unsigned char data);
void cmdLCD(unsigned char cmd);
void charLCD(unsigned char ascii);
void InitLCD(void);
void strLCD(signed char *p);// for displaying the string
void U32LCD(unsigned int num);  //atoi conversion
void S32LCD(signed int num);  //display integers in negative
void F32LCD(float num,unsigned char nDecimalp);
void BuildCGRAM(unsigned char nb,unsigned char *p);





