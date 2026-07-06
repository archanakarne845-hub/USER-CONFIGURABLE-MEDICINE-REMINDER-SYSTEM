#include<lpc214x.h>


#include "kpm_defines.h"
#include "lcd_defines.h"
#include "LCD.h"
#include "rtc_main.h"


#define buzzer 0

//EINT0
#define EINT0_INPUT_PIN 0x0000000c
#define EINT0_VIC_CHNO 14
#define EINT0_STATUS_LED 16

//EINT1
#define EINT1_INPUT_PIN 0x000000c0
#define EINT1_VIC_CHNO 15
#define EINT1_STATUS_LED 17 //p1.17

void eint0_isr(void) __irq;
void eint1_isr(void) __irq;

unsigned int gCount;
int last_min=-1,last_second=0,cnt=0;
int hour,minute,second,day,m1hr=0,m2hr=0,m3hr=0,m1min=0,m2min=0,m3min=0;   //-1 indicates that medicine time is not set
char key;
int date,month,year,dow,flag1=0,flag2=0,flag3=0;

void configure()
{
	while(1)
	{
		//if(((IOPIN0>>sw1) &1)==0)
		//{
			cmdLCD(CLEAR_LCD);
			cmdLCD(GOTO_LINE1_POS0);
			strLCD((signed char*)"1.RTC EDIT 3.EXIT");
			cmdLCD(GOTO_LINE2_POS0);
			strLCD((signed char*)"2.cfg medi sched");
			key=keyScan();
			switch(key)
			{
				//for RTC edit
				case '1':
					while(1)
					{
						cmdLCD(CLEAR_LCD);
						cmdLCD(GOTO_LINE1_POS0);
						strLCD((signed char*)"1.H 2.M 3.S 4.D");
						cmdLCD(GOTO_LINE2_POS0);
						strLCD((signed char *)"5.Dt 6.M 7.Y 8.E");
						key=keyScan();
						switch(key)
						{
							
						    //for hour edit
							 case '1':	flag3=1;
									hr:cmdLCD(CLEAR_LCD);
										 cmdLCD(GOTO_LINE1_POS0);
										 strLCD((signed char *)"Enter hour:");
										 cmdLCD(GOTO_LINE2_POS0);
							       strLCD((signed char *)"00:00:00");
							 
							       cmdLCD(GOTO_LINE2_POS0);
										 hour=ReadNum(2);
										delay_ms(200);
											
										/*	if(keyScan()=='*')
											{
												goto hr;
											}*/
										 if(hour>23)
										 {
												cmdLCD(CLEAR_LCD);
												cmdLCD(GOTO_LINE1_POS0);
												strLCD((signed char *)"Invalid entry");
											 delay_ms(500);
												goto hr;
											}
											else
											{
											
												SetRTCTimeInfo(hour,minute,second);
											}
											break;
											
										//for minute edit
							 case '2':	flag3=1;
									min:cmdLCD(CLEAR_LCD);
											cmdLCD(GOTO_LINE1_POS0);
											strLCD((signed char *)"Enter minute:");
							       cmdLCD(GOTO_LINE2_POS0);
							       strLCD((signed char *)"00:00:00");
							 
										  cmdLCD(GOTO_LINE2_POS0+3);
											minute=ReadNum(2);
											delay_ms(200);
											if(minute>59)
											{
												cmdLCD(CLEAR_LCD);
												cmdLCD(GOTO_LINE1_POS0);
												strLCD((signed char *)"Invalid entry:");
												delay_ms(500);
												goto min;
											}
											else
											{
												SetRTCTimeInfo(hour,minute,second);
											}
											break;
											
											//for second edit
							 case '3':flag3=1;
									sec:cmdLCD(CLEAR_LCD);
											cmdLCD(GOTO_LINE1_POS0);
											strLCD((signed char *)"Enter second");
							        cmdLCD(GOTO_LINE2_POS0);
							        strLCD((signed char *)"00:00:00");
							 
											cmdLCD(GOTO_LINE2_POS0+6);
											second=ReadNum(2);
											delay_ms(200);
											if(second>59)
											{
												cmdLCD(CLEAR_LCD);
												cmdLCD(GOTO_LINE1_POS0);
												strLCD((signed char *)"Invalid entry:");
												delay_ms(500);
												goto sec;
											}
											else
											{
												SetRTCTimeInfo(hour,minute,second);
											}
											break;
											
							 //for day edit
							 case '4':
										while(1)
										{
											cmdLCD(CLEAR_LCD);
											cmdLCD(GOTO_LINE1_POS0);
											strLCD((signed char *)"0.Su1.mo2.tu3.we");
											cmdLCD(GOTO_LINE2_POS0);
											strLCD((signed char *)"4.Th5.Fr6.Sa7.e");	
											key=keyScan();
											if(key>='0' && key<='6')
											{
												day=key-'0';
												SetRTCDay(day);
												break;
											}
											else if(key=='7')
											{
												break;
											}
											else
											{
												cmdLCD(CLEAR_LCD);
												cmdLCD(GOTO_LINE1_POS0);
												strLCD((signed char *)"Invalid entry");
												delay_ms(500);
											}
										}		
										break;
										
										//for date edit
							 case '5':
										date:cmdLCD(CLEAR_LCD);
												 cmdLCD(GOTO_LINE1_POS0);
												 strLCD((signed char *)"Enter date");
												 cmdLCD(GOTO_LINE2_POS0);
							           strLCD((signed char *)"00/00/0000");
												 cmdLCD(GOTO_LINE2_POS0);
												 date=ReadNum(2);
												 delay_ms(200);
												 if(date<=0 || date>31)
												 {
														cmdLCD(CLEAR_LCD);
														cmdLCD(GOTO_LINE1_POS0);
														strLCD((signed char *)"Invalid entry:");
													 delay_ms(500);
														goto date;
													}
													else
													{
														SetRTCDateInfo(date,month,year);
													}
											   break;
													
											//for month edit
								 case '6':
											month:cmdLCD(CLEAR_LCD);
														cmdLCD(GOTO_LINE1_POS0);
														strLCD((signed char *)"Enter month");
														 cmdLCD(GOTO_LINE2_POS0);
							               strLCD((signed char *)"00/00/0000");
														 cmdLCD(GOTO_LINE2_POS0+3);
														 month=ReadNum(2);
														delay_ms(200);
														if(month<=0 || month>12)
														{
															cmdLCD(CLEAR_LCD);
															cmdLCD(GOTO_LINE1_POS0);
															strLCD((signed char *)"Invalid entry:");
															delay_ms(500);
															goto month;
														}
														else
														{
															SetRTCDateInfo(date,month,year);
															//break;
														}
										  	break;
														
											//for year edit
									case '7':
											year:
												cmdLCD(CLEAR_LCD);
												cmdLCD(GOTO_LINE1_POS0);
												strLCD((signed char *)"Enter year");
												
												 cmdLCD(GOTO_LINE2_POS0);
												strLCD((signed char *)"00/00/0000");
									
												 cmdLCD(GOTO_LINE2_POS0+6);
												year=ReadNum(4);
												delay_ms(200);
												if(year<=0 || year>=4096)
												{
													cmdLCD(CLEAR_LCD);
													cmdLCD(GOTO_LINE1_POS0);
													strLCD((signed char *)"Invalid entry:");
													delay_ms(500);
													goto year;
												}
												else
												{
													SetRTCDateInfo(date,month,year);
												}
											 break;
												
										//for exit
										case '8':
															break;
														default:cmdLCD(CLEAR_LCD);
															cmdLCD(GOTO_LINE1_POS0);
															strLCD((signed char *)"invalid input:");
												delay_ms(500);
															break;
									}
									if(key=='8')
										break;
								}
							break;
								
								//for cfg medicine schedule
				case '2':
								while(1)
								{
									cmdLCD(CLEAR_LCD);
								 cmdLCD(GOTO_LINE1_POS0);
								 strLCD((signed char *)"1.m1  2.m2  3.m3");
								 cmdLCD(GOTO_LINE2_POS0);
								 strLCD((signed char *)"4.exit");
								 key=keyScan();
									switch(key)
									{
			
										//medicine 1 Edit
										case '1':
											while(1)
											{
														cmdLCD(CLEAR_LCD);
														 cmdLCD(GOTO_LINE1_POS0);
														 strLCD((signed char*)"1.HH  2.MM  3.exit");
														 key=keyScan();
														 switch(key)
														 {
																//hour edit
																case '1':
																	m1hour:cmdLCD(CLEAR_LCD);
																			cmdLCD(GOTO_LINE1_POS0);
																			strLCD((signed char *)"Enter hour:");
																			cmdLCD(GOTO_LINE2_POS0);
																			strLCD((signed char *)"00:00:00");
																			cmdLCD(GOTO_LINE2_POS0);
																			m1hr=ReadNum(2);
																			delay_ms(200);
																
																			if(m1hr>23)
																			{
																				cmdLCD(CLEAR_LCD);
																				cmdLCD(GOTO_LINE1_POS0);
																				strLCD((signed char *)"Invalid entry");
																				delay_ms(500);
																				goto m1hour;
																			}
																			else
																			{
																				SetRTCTimeInfo(hour,minute,second);
																			}
																		break;
																			
																	//minute for medicine 1
																case '2':
																		m1minute:cmdLCD(CLEAR_LCD);
																			cmdLCD(GOTO_LINE1_POS0);
																			strLCD((signed char *)"Enter minute:");
																
																			cmdLCD(GOTO_LINE2_POS0);
							                        strLCD((signed char *)"00:00:00");
																
																			cmdLCD(GOTO_LINE2_POS0+3);
																			m1min=ReadNum(2);
																			delay_ms(200);
																
																			if(m1min>59)
																			{
																				cmdLCD(CLEAR_LCD);
																				cmdLCD(GOTO_LINE1_POS0);
																				strLCD((signed char *)"Invalid entry:");
																				delay_ms(500);
																				goto m1minute;
																			}
																			else
																			{
																				SetRTCTimeInfo(hour,minute,second);
																			}
																			break;
																			
																	//exit for medicine 1
																case '3':break;
																			default:cmdLCD(CLEAR_LCD);
															cmdLCD(GOTO_LINE1_POS0);
															strLCD((signed char *)"invalid input:");
												delay_ms(500);
															break;
															}
														 if(key=='3')
															 break;
														}
													break;
															
										//medicine 2 edit 
										case '2':
											while(1)
											{
												cmdLCD(CLEAR_LCD);
															cmdLCD(GOTO_LINE1_POS0);
															strLCD((signed char*)"1.HH  2.MM  3.exit");
															key=keyScan();
											
															switch(key)
															{
																//hour for medicine 2
																case '1':
																			m2hour:cmdLCD(CLEAR_LCD);
																			cmdLCD(GOTO_LINE1_POS0);
																			strLCD((signed char *)"Enter hour:");
																			
																			cmdLCD(GOTO_LINE2_POS0);
							                        strLCD((signed char *)"00:00:00");
																
																			cmdLCD(GOTO_LINE2_POS0);
																			m2hr=ReadNum(2);
																			delay_ms(200);
																
																			if(m2hr>23)
																			{
																				cmdLCD(CLEAR_LCD);
																				cmdLCD(GOTO_LINE1_POS0);
																				strLCD((signed char *)"Invalid entry");
																				delay_ms(500);
																				goto m2hour;
																			}
																			else
																			{
																				SetRTCTimeInfo(hour,minute,second);
																			}
																			break;
																			
																	//minute for medicine 2
																case '2':
																			m2minute:cmdLCD(CLEAR_LCD);
																			cmdLCD(GOTO_LINE1_POS0);
																			strLCD((signed char *)"Enter minute:");
																
																			cmdLCD(GOTO_LINE2_POS0);
							                        strLCD((signed char *)"00:00:00");
																
																			cmdLCD(GOTO_LINE2_POS0+3);
																			m2min=ReadNum(2);
																			delay_ms(200);
																
																			if(m2min>59)
																			{
																				cmdLCD(CLEAR_LCD);
																				cmdLCD(GOTO_LINE1_POS0);
																				strLCD((signed char *)"Invalid entry:");
																				delay_ms(500);
																				goto m2minute;
																			}
																			else
																			{
																				SetRTCTimeInfo(hour,minute,second);
																			}
																			break;
																			
																	//exit for medicine 2
																case '3':break;
																			default:cmdLCD(CLEAR_LCD);
															cmdLCD(GOTO_LINE1_POS0);
															strLCD((signed char *)"invalid input:");
												delay_ms(500);
															break;
															}
															 if(key=='3')
															 break;
														}
													break;
															
											//medicine 3 edit
										case '3':
											while(1)
											{
												      cmdLCD(CLEAR_LCD);
															cmdLCD(GOTO_LINE1_POS0);
															strLCD((signed char*)"1.HH  2.MM  3.exit");
															key=keyScan();
															switch(key)
															{
																
																//hour for medicine 3
																case '1':
																	m3hour:cmdLCD(CLEAR_LCD);
																		cmdLCD(GOTO_LINE1_POS0);
																		strLCD((signed char *)"Enter hour:");
																
																		cmdLCD(GOTO_LINE2_POS0);
							                      strLCD((signed char *)"00:00:00");
																
																		cmdLCD(GOTO_LINE2_POS0);
																		m3hr=ReadNum(2);
																		delay_ms(200);
																
																		if(m3hr>23)
																		{
																			cmdLCD(CLEAR_LCD);
																			cmdLCD(GOTO_LINE1_POS0);
																			strLCD((signed char *)"Invalid entry");
																			delay_ms(500);
																			goto m3hour;
																		}
																		else
																		{
																				SetRTCTimeInfo(hour,minute,second);
																		}
																		break;
																		
																	//minute for medicine 3
																case '2':
																			m3minute:cmdLCD(CLEAR_LCD);
																			cmdLCD(GOTO_LINE1_POS0);
																			strLCD((signed char *)"Enter minute:");
																			
																			cmdLCD(GOTO_LINE2_POS0);
							                        strLCD((signed char *)"00:00:00");
																			cmdLCD(GOTO_LINE2_POS0+3);
																			m3min=ReadNum(2);
																      delay_ms(200);
																
																			if(m3min>59)
																			{
																				cmdLCD(CLEAR_LCD);
																				cmdLCD(GOTO_LINE1_POS0);
																				strLCD((signed char *)"Invalid entry:");
																				delay_ms(500);
																				goto m3minute;
																			}
																			else
																			{
																				SetRTCTimeInfo(hour,minute,second);
																			}
																			break;
																			
																		//exit for medicine 3
																case '3':break;
																			default:cmdLCD(CLEAR_LCD);
															cmdLCD(GOTO_LINE1_POS0);
															strLCD((signed char *)"invalid input:");
												delay_ms(500);
															break;
															}
															 if(key=='3')
															 break;
														}
														break;
															 //exit
											case '4':break;
											default:cmdLCD(CLEAR_LCD);
															cmdLCD(GOTO_LINE1_POS0);
															strLCD((signed char *)"invalid input:");
															
														delay_ms(500);
															break;
														}
									if(key=='4')
										break;
									}
					//		break;
						case '3':break;
						default:cmdLCD(CLEAR_LCD);
										cmdLCD(GOTO_LINE1_POS0);
										strLCD((signed char *)"invalid input:");
															
										delay_ms(500);
										break;
		  }
			if(key=='3')
				break;
	  }
	
}
int main()
{
	InitLCD();
	RTC_Init();
	InitKPM();
	IODIR0|=1<<buzzer;
	IOCLR0=1<<buzzer;
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
	VICVectCntl1=(1<<5) | EINT0_VIC_CHNO;
	
	//load eint0_isr address into LUT sfr
	VICVectAddr1=(u32)eint0_isr;
	
	//cfg EINT1 as v.irq with next highest priority(1)
	VICVectCntl0=(1<<5) |EINT1_VIC_CHNO;
	
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
	cmdLCD(CLEAR_LCD);
	cmdLCD(DISP_ON);
	while(1)
	{
		GetRTCTimeInfo(&hour, &minute, &second);
	//	cmdLCD(CLEAR_LCD);
				
		if(flag1)
		{
			flag2=1;
			configure();
			flag1=0;
			flag2=0;
			cmdLCD(CLEAR_LCD);
		}
		
		if(second !=last_second)
		{
			last_second=second; //update to current time
			if(cnt>0)
			{
				cnt--;
			if(cnt==0)
				{
					//cmdLCD(CLEAR_LCD);
					IOCLR0=1<<buzzer;
					last_min=-1;
					flag3=0;
					cmdLCD(CLEAR_LCD);
				}
			}
		}
	//check medicine time to take
		if(flag3)
		{
		if(minute != last_min)
		{
			
			if(((hour==m1hr)&&(minute==m1min)) || ((hour==m2hr)&&(minute==m2min)) || ((hour==m3hr)&&(minute==m3min)))
			{
				last_min=minute;
				cnt=59;
				cmdLCD(CLEAR_LCD);
			
				
			}

		}
	}
		if(last_min==minute && cnt>0)
		//{
			/*if(flag3==1)
			{*/
			//if(second !=last_second)
		{
			IOSET0=1<<buzzer;
			//cmdLCD(CLEAR_LCD);
			cmdLCD(GOTO_LINE1_POS0);
			strLCD((signed char *)"TAKE MEDICINE");
			//cmdLCD(CLEAR_LCD);
			/*cmdLCD(GOTO_LINE2_POS0);
			strLCD((signed char *)"timer:");
			cmdLCD(GOTO_LINE2_POS0+7);
			U32LCD(cnt);
			
			if(cnt<10)
			{
				charLCD('0');
			}
			*/
		}
		
	//}
		else
		{
		//	flag3=0;
			IOCLR0=1<<buzzer;
			
			cmdLCD(GOTO_LINE1_POS0);
			GetRTCTimeInfo(&hour, &minute, &second);
			DisplayRTCTime(hour,minute, second);
			
			cmdLCD(GOTO_LINE2_POS0);
			GetRTCDateInfo(&date, &month, &year);
			DisplayRTCDate(date, month, year);
			
			cmdLCD(GOTO_LINE1_POS0+10);
			GetRTCDay(&dow);
			DisplayRTCDay(dow);
		}
	}
	
}

void eint0_isr(void) __irq
{
	
	//clear EINT0 status in EXTInt peripheral
	EXTINT=1<<0;
	
	//clear EINT0 status in vic peripheral
	VICVectAddr=0;
	flag1=1;
	
}

void eint1_isr(void) __irq
{
	
	//clear EINT1 status in EXT Int peripheral
	EXTINT =1<<1;
	
	//clear EINT1 status in vic peripheral
	VICVectAddr=0;
	
	if(((IOPIN0>>buzzer)&1)==1)
	{
	//clearing the buzzer
	IOCLR0=1<<buzzer;
	cnt=0;
	cmdLCD(CLEAR_LCD);
	strLCD((signed char *)"medicine taken");
	delay_ms(200);
	cmdLCD(CLEAR_LCD);
	}
	else if(flag2==1)
	{
	}
	else
	{
		cmdLCD(CLEAR_LCD);
		strLCD((signed char *)"No Reminders");
		delay_ms(500);
		cmdLCD(CLEAR_LCD);
		
	}
}


