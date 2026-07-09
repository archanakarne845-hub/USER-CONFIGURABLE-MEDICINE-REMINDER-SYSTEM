#include <lpc21xx.h>
#include "lcd_defines.h"
#include "LCD.h"

//delay for micro seconds

void delay_us(unsigned int dlyus)
{
        for(dlyus*=12;dlyus>0;dlyus--);
}

//delay for milli seconds

void delay_ms(unsigned int dlyms)
{
        for(dlyms*=12000;dlyms>0;dlyms--);
}

void writeLCD(unsigned char data)
{
	//select write mode (RW = 0)
        IOCLR0=1<<RW;

	//place data on LCD data pins
        IOPIN0=(IOPIN0&~(0xff<<pin)) | (data<<pin);

	//Enable High
        IOSET0=1<<EN;

	//small delay
        delay_us(1);

	//Enable low
        IOCLR0=1<<EN;

	//wait for LCD execution
        delay_ms(2);
}

//Sends command to LCD
void cmdLCD(unsigned char cmd)
{
	//RS = 0 (Command register)
        IOCLR0=1<<RS;
        writeLCD(cmd);
}

//Sends one character to LCD
void charLCD(unsigned char ascii)
{
	//RS = 1 (Data Register)
        IOSET0=1<<RS;
        writeLCD(ascii);
}

//LCD Initialization
void InitLCD(void)
{
	//configure LCD data and control pins as output
        IODIR0=(0xff<<pin) | (1<<RS) | (1<<RW) | (1<<EN);

	//LCD power-on delay
        delay_ms(15);

	//Initialize sequence
        cmdLCD(MODE_8BIT_1LINE);
        delay_ms(5);

        cmdLCD(MODE_8BIT_1LINE);
        delay_us(100);

        cmdLCD(MODE_8BIT_1LINE);

	//set 8-bit, 2line mode
        cmdLCD(MODE_8BIT_2LINE);

	//Display ON,Cursor Blink
        cmdLCD(DISP_ON_CUR_BLINK);

	//Clear LCD display
        cmdLCD(CLEAR_LCD);

	//Cursor moves to right
        cmdLCD(SHIFT_CUR_RIGHT);
}

// for displaying the string
void strLCD(signed char *p)
{
        while(*p)
	{
		//Displaying one character at a time
                charLCD(*p++);
	}
}

//Displays unsigned integer on LCD
void U32LCD(unsigned int num)
{
        char a[10];
        int i=0;
        if(num==0)
        {
                charLCD('0');
        }

	//store digits in reverse order
        while(num)
	{
            	a[i++]=(num%10)+48;
		num/=10;
        }

	//Displaying the digits in correct order
        for(i=-1;i>=0;i--)
		charLCD(a[i]);
}

//Displaying signed integer on LCD
void S32LCD(signed int num)
{
        if(num<0)
        {
                charLCD('-');//Display minus sign
                num=-num;
	}
        U32LCD(num);
        
}

//display the float values in decimal point
void F32LCD(float num,unsigned char nDecimalp)
{
        unsigned int n;
        signed int i;
        if(num<0)
        {
                charLCD('-');
		num=-num;
        }
        else
	{
		//Display integer part
                n=num;
                U32LCD(n);

                charLCD('.');

		//display fractional part
                for(i=0;i<nDecimalp;i++)
                {
                        num=(num-n)*10;
                        n=num;
                        charLCD(n+48);
                }
        }
}

