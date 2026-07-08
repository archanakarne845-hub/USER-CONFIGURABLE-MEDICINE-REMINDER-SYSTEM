#include <lpc21xx.h>	
#include "kpm_defines.h"
#include "LCD.h"
#include "lcd_defines.h"

/*unsigned int KPMLUT[4][4]={{1,2,3,4},
														{5,6,7,8},
														{9,10,11,12},
														{13,14,15,16}};*/
unsigned char KPMLUT[4][4]={{'7','8','9','/'},
															{'4','5','6','*'},
															{'1','2','3','-'},
															{'c','0','=','+'}};

void InitKPM(void)
{
	IODIR1=15<<ROW0;
}

int colscan(void)
{
	return ((((IOPIN1>>COL0)&15)<15)?0:1);
}

int RowCheck(void)
{
	int rno;
	for(rno=0;rno<4;rno++)
	{
		IOPIN1= (~(1<<rno)<<ROW0);
		if(colscan()==0)
		{
			break;
		}
	}
	//make rows as default
	IOPIN1=0x0<<ROW0;
	return rno;
}

int ColCheck(void)
{
	int cno;
	for(cno=0;cno<4;cno++)
	{
		if(((IOPIN1>>(COL0 + cno))&1)==0)
		{
			break;
		}
	}
	return cno;
}
char keyScan(void)
{
	int rno,cno,keyv;
	
	//wait for switch press
	while(colscan());
	
	//find the rno
	rno=RowCheck();
	
	//find the colno
	cno=ColCheck();
	
	//get the key value using KPMLUT
	keyv= KPMLUT[rno][cno];
	
	//wait for switch release
	while(!colscan()); //without col switch release it will not return key value
	return keyv;
}

int ReadNum(int n)
{
	char key;
	int sum=0,i=0;
	for(;n>0;)
	{
		key=keyScan();
		if(key>='0' && key<='9')
		{
			charLCD(key);
			sum=(sum*10) + (key - '0');
			i++;
			n--;
		}
		else if(key=='c')
		{
			if(i>0)
			{
				i--;
				n++; //allow to rentry the digit
				sum=sum/10;
			cmdLCD(DISP_ON_CUR_BLINK);
			cmdLCD(SHIFT_CUR_LEFT);
			charLCD('0');
			cmdLCD(SHIFT_CUR_LEFT);
			}
		}			
		else
		{
			cmdLCD(CLEAR_LCD);
			cmdLCD(GOTO_LINE1_POS0);
			strLCD((signed char *)"Invalid input");
			delay_ms(1000);
			sum=0;
			break;
		}
	}
	return sum;
}



