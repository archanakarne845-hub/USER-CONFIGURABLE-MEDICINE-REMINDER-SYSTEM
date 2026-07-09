// Generates delay in microseconds
void delay_us(unsigned int dlyus);

// Generates delay in milliseconds
void delay_ms(unsigned int dlyms);

// Writes one byte (command or data) to the LCD
void writeLCD(unsigned char data);

// Sends a command to the LCD
void cmdLCD(unsigned char cmd);

// Sends a single ASCII character to the LCD
void charLCD(unsigned char ascii);

// Initializes the LCD in 8-bit mode
void InitLCD(void);

void strLCD(signed char *p);// for displaying the string

// Displays an unsigned 32-bit integer on the LCD
void U32LCD(unsigned int num);  

// Displays a signed 32-bit integer
void S32LCD(signed int num);  

// Displays a floating-point number
void F32LCD(float num,unsigned char nDecimalp);







