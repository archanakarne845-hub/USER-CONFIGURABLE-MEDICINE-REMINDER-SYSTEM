#include<lpc21xx.h>
//#include "kpm.c"
#include "kpm_defines.h"

#define AH_4_AL_4 8

int main()
{
	unsigned int keyvalue;
	InitKPM();
	IODIR0=0xff<<AH_4_AL_4;
	while(1)
	{
		//take the key value from 4*4 KPM
		keyvalue=keyScan();
		
		IOPIN0=((IOPIN0&~(0Xff<<AH_4_AL_4)) | (keyvalue^0x0f)<<AH_4_AL_4);
	}
}

