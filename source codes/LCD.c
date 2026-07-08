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
        IOCLR0=1<<RW;
        IOPIN0=(IOPIN0&~(0xff<<pin)) | (data<<pin);
        IOSET0=1<<EN;
        delay_us(1);
        IOCLR0=1<<EN;
        delay_ms(2);
}
void cmdLCD(unsigned char cmd)
{
        IOCLR0=1<<RS;
        writeLCD(cmd);
}

//displaying the characters

void charLCD(unsigned char ascii)
{
        IOSET0=1<<RS;
        writeLCD(ascii);
}

void InitLCD(void)
{
        IODIR0=(0xff<<pin) | (1<<RS) | (1<<RW) | (1<<EN);
        delay_ms(15);
        cmdLCD(MODE_8BIT_1LINE);
        delay_ms(5);
        cmdLCD(MODE_8BIT_1LINE);
        delay_us(100);
        cmdLCD(MODE_8BIT_1LINE);
        cmdLCD(MODE_8BIT_2LINE);
        cmdLCD(DISP_ON_CUR_BLINK);
        cmdLCD(CLEAR_LCD);
        cmdLCD(SHIFT_CUR_RIGHT);
}

// for displaying the string

void strLCD(signed char *p)
{
        while(*p)
				{
                charLCD(*p++);
				}
}

//atoi conversion

void U32LCD(unsigned int num)
{
        char a[10];
        int i=0;
        if(num==0)
        {
                charLCD('0');
        }
        while(num)
				{
             a[i++]=(num%10)+48;
						 num/=10;
         }
         for(i=-1;i>=0;i--)
					charLCD(a[i]);
}

//display integers in negative

void S32LCD(signed int num)
{
        if(num<0)
        {
                charLCD('-');
                num=-num;
                U32LCD(num);
        }
}

//display the float values in decimal point

void F32LCD(float num,unsigned char nDecimalp)
{
        unsigned int n;
        signed int i;
        if(num<0)
        {
                charLCD('-');
        }
        else
					  {
                n=num;
                U32LCD(n);
                charLCD('.');
                for(i=0;i<nDecimalp;i++)
                {
                        num=(num-n)*10;
                        n=num;
                        charLCD(n+48);
                }
        }
}

void BuildCGRAM(unsigned char nb,unsigned char *p)
{
	int i;
    cmdLCD(GOTO_CGRAM);
				// Set CGRAM address
    for(i = 0; i <= nb; i++) 
		{
        charLCD(p[i]); // Write pattern row
    }
    cmdLCD(GOTO_LINE1_POS0); // Reset to DDRAM

}

void HEXLCD(unsigned int n)
{
	unsigned char a[8],rem;
	signed int i=0;
	if(n==0)
	{
		charLCD('0');
	}
	else
	{
		while(n)
		{
			rem=n%16;
			(rem<10) ? (rem+=48) : (rem+=55);
			a[i++]=rem;
			n/=16;
		}
		for(--i;i>=0;i--)
		{
			charLCD(a[i]);
		}
	}
}

void OctalLCD(unsigned int n)
{
	unsigned char a[8],rem;
	signed int i=0;
	if(n==0)
	{
		charLCD('0');
	}
	else
	{
		while(n)
		{
			rem=n%8;
			(rem<10) ? (rem+=48) : (rem+=55);
			a[i++]=rem;
			n/=8;
		}
		for(--i;i>=0;i--)
		{
			charLCD(a[i]);
		}
	}
}

void BinLCD(unsigned int n, unsigned char nbd)
{
	signed int i;
	for(i=7;i>=0;i--)
	{
		charLCD(((n>>i)&1)+48);
	}
}
