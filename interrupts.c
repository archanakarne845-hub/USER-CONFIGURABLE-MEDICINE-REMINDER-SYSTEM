#include<lpc21xx.h>

#define EINT0_INPUT_PIN 0x0000000c
#define EINT0_VIC_CHNO 14
#define EINT0_STATUS_LED 16
void eint0_isr(void) __irq;
int gCount;
int main()
{
	unsigned int u32;
	//cfg p0.1 pin as EINT0 input pin
	//clr bit 2&3 without affecting other bits
	PINSEL0 &=~3<<2;
	
	//update bit 2&3 for EINT0 pin function
	PINSEL0|=EINT0_INPUT_PIN;
	
	//cfg VIC peripherial/block
	//allow EINT0 as irq type
	//VICIntSelect=0; (default)
	//enable EINT0 through channel
	VICINTEnable =1<<EINT0_VIC_CHNO;
	
	//cfg EINT0 as v.irq with highest priority
	VICVectCnt10=(1<<5) | EINT0 VIC_CHNO;
	
	//load einto_isr address into LUT sfr
	VICVectAddr0=(u32)eint0_isr;
	
	//cfg EINT0 via external interrupt peripheral
	//enable EINT0
	//EXTINT=0(default)
	//cfg EINT0 as edge triggered interrupt
	EXTMODE =1<<0;
	
	//cfg EINT0 as falling edge triggered
	//EXTPOLAR =0(default)
	//cfg status led pin for EINT0 as gpio 
	IODIR1|=1<<EINT0_STATUS_LED;
	while(1)
	{
		gCount++;
	}
}


void eint0_isr(void) __irq
{
	//toggle led as status that interrupt occured
	IOPIN1^=1<<EINT0_STATUS_LED;
	//clear EINT0 status in EXTINT peripheral
	EXTINT=1<<0;
	//clear EINT0 status in vic peripheral
	VICVectAddr=0;
}

