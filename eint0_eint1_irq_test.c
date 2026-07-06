#include<lpc21xx.h>

#define EINT0_INPUT_PIN 0x0000000c
#define EINT0_VIC_CHNO 14
#define EINT0_STATUS_LED 16

#define EINT1_INPUT_PIN 0x000000c0
#define EINT1_VIC_CHNO 15
#define EINT1_STATUS_LED 17 //p1.17

void eint0_isr(void) __irq;
void eint1_isr(void) __irq;

unsigned int gCount;
int main()
{
	unsigned int u32;
	//cfg p0.1,p0.3 pins as EINT0,EINT1 as input pins
	//clr bit pairs(2,3) & bit pair (6,7) without affecting other bits.
	PINSEL0 &=((u32)~3<<2) | ((u32)~3<<6);
	
	//update bit 2&3,bit 4&5 for EINT0,EINT1 pin functions
	PINSEL0 |=EINT0_INPUT_PIN | EINT1_INPUT_PIN;
	
	//cfg VIC peripheral/block
	//allow EINT0,EINT1 as irq type
	//VICIntSelect=0(defualt)
	//enable EINT0,EINT1 through channel
	VICIntEnable  =1<<EINT0_VIC_CHNO | 1<<EINT1_VIC_CHNO;
	
	//cfg EINT0 as virq with highest priority(0)
	VICVectCntl0=(1<<5) | EINT0_VIC_CHNO;
	
	//load eint0_isr address into LUT sfr
	VICVectAddr1=(u32)eint0_isr;
	
	//cfg EINT1 as v.irq with next highest priority(1)
	VICVectCntl1=(1<<5) |EINT1_VIC_CHNO;
	
	//load eint1_isr address into LUT sfr
	VICVectAddr0=(u32)eint1_isr;
	
	//cfg EINT0,EINT1 via external interrupts peripheral
	//enable EINT0,EINT1 
	//EXTINT=0(default)
	//cfg EINT0,EINT1 as edge trig interrupt
	EXTMODE = ((1<<1) | (1<<0));
	
	//cfg EINT0,EINT1 as falling edge trig
	//EXTPOLAR=0(default)
	//cfg status led pin for EINT1 as gpio
	IODIR1=1<<EINT0_STATUS_LED | 1<EINT1_STATUS_LED;
	
	while(1)
	{
		gCount++;
	}
}

void eint0_isr(void) __irq
{
	//toggle led as status that interrupt occured
	IOPIN`^=1<<EINT0_STATUS_LED;
	
	//clear EINT0 status in EXTInt peripheral
	EXTINT=1<<0;
	
	//clear EINT0 status in vic peripheral
	VICVectAddr=0;
}

void eint1_isr(void) __irq
{
	//toggle led as status that interrupt occurred
	IOPIN0^=1<<EINT1_STATUS_LED;
	
	//clear EINT1 status in EXT Int peripheral
	EXTINT =1<<1;
	
	//clear EINT1 status in vic peripheral
	VICVectAddr=0;
}