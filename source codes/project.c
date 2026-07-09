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
int hour,minute,second,day,m1hr=-1,m2hr=-1,m3hr=-1,m1min=-1,m2min=-1,m3min=-1;   //-1 indicates that medicine time is not set
char key;
int date,month,year,dow,flag1=0,flag2=0,flag3=0;

void configure()
{
	while(1)
	{
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
						case '1':flag3=1; // Indicate that RTC settings have been modified
						hr:cmdLCD(CLEAR_LCD);    // Clear the LCD screen
						cmdLCD(GOTO_LINE1_POS0);
						strLCD((signed char *)"Enter hour:");
						cmdLCD(GOTO_LINE2_POS0);
						strLCD((signed char *)"00:00:00"); 
						cmdLCD(GOTO_LINE2_POS0);
						hour=ReadNum(2);    // Read two-digit hour from keypad
						delay_ms(200);
						// Validate hour (Valid range: 0 to 23)
						if(hour>23)
						{
							 cmdLCD(CLEAR_LCD);
							 cmdLCD(GOTO_LINE1_POS0);
							 strLCD((signed char *)"Invalid entry");
							 delay_ms(500);
	 						 goto hr;  // Ask user to enter hour again
						}
						else
						{						
			 				// Store updated hour in RTC					 
							SetRTCTimeInfo(hour,minute,second);
						}
						break;   // Return to RTC Edit Menu 
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
						// Validate minute (Valid range: 0 to 59)
						if(minute>59)
						{
							cmdLCD(CLEAR_LCD);
							cmdLCD(GOTO_LINE1_POS0);
							strLCD((signed char *)"Invalid entry:");
							delay_ms(500);
							goto min;  // Ask user to re-enter minute
						}
						else
						{
							// Update RTC with the new minute value
							SetRTCTimeInfo(hour,minute,second);
						}
						break;    // Return to RTC Edit Menu
										
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
						 // Validate second (Valid range: 0 to 59)
						if(second>59)
						{
							cmdLCD(CLEAR_LCD);
							cmdLCD(GOTO_LINE1_POS0);
							strLCD((signed char *)"Invalid entry:");
							delay_ms(500);
							goto sec;  // Ask user to re-enter second
						}
						else
						{
							 // Update RTC with the new second value
							SetRTCTimeInfo(hour,minute,second);
						}
						break;    // Return to RTC Edit Menu											
						//for day edit
						case '4':
						      while(1)
						      {
							cmdLCD(CLEAR_LCD);
							cmdLCD(GOTO_LINE1_POS0);
							strLCD((signed char *)"0.Su1.mo2.tu3.we");  // Display Sunday to Wednesday options
							cmdLCD(GOTO_LINE2_POS0);
							strLCD((signed char *)"4.Th5.Fr6.Sa7.e");	
							key=keyScan();  // Read key pressed from keypad
							if(key>='0' && key<='6')
							{
								day=key-'0';  // Convert ASCII digit to integer
								SetRTCDay(day);  // Update the RTC day
								break;   // Exit Day Edit menu
							}
							// Exit without changing the day
							else if(key=='7')
							{
								break;
							}
							// Invalid key pressed
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
							strLCD((signed char *)"00/00/0000");  // Display date format (DD/MM/YYYY)
							cmdLCD(GOTO_LINE2_POS0);
							date=ReadNum(2);   // Read two-digit date from keypad
							delay_ms(200);

							 // Validate date (Valid range: 1 to 31)
							if(date<=0 || date>31)
							{
								cmdLCD(CLEAR_LCD);
								cmdLCD(GOTO_LINE1_POS0);
								strLCD((signed char *)"Invalid entry:");  //Display error message
								delay_ms(500);  // Wait before asking again
								goto date;   // Ask user to re-enter the date
							}
							else
							{
								// Update the RTC with the new date
								SetRTCDateInfo(date,month,year);
							}
							break;  // Return to RTC Edit Menu
													
							//for month edit
						case '6':
							month:cmdLCD(CLEAR_LCD);  // Clear the LCD screen
							cmdLCD(GOTO_LINE1_POS0);  // Move cursor to Line 1
							strLCD((signed char *)"Enter month");  // Display prompt
							cmdLCD(GOTO_LINE2_POS0);  // Move cursor to Line 2
 							strLCD((signed char *)"00/00/0000");						                                                                            cmdLCD(GOTO_LINE2_POS0+3);									                                                                month=ReadNum(2);  // Read two-digit month from keypad
							delay_ms(200);  // Small delay after input

							// Validate month (Valid range: 1 to 12)
							if(month<=0 || month>12)
							{
								cmdLCD(CLEAR_LCD);
								cmdLCD(GOTO_LINE1_POS0);
								strLCD((signed char *)"Invalid entry:");  // Display error message
								delay_ms(500);  // Wait before asking again
								goto month;  //Ask user to re-enter the month
							}	
							else
							{
								// Update the RTC with the new month
								SetRTCDateInfo(date,month,year);
							}	 
							break;
							//for year edit
						case '7':
							year:
							cmdLCD(CLEAR_LCD);  // Clear the LCD screen
							cmdLCD(GOTO_LINE1_POS0);  // Move cursor to Line 1
							strLCD((signed char *)"Enter year");  // Display prompt
							cmdLCD(GOTO_LINE2_POS0);  // Move cursor to Line 2
							strLCD((signed char *)"00/00/0000");  // Display date format (DD/MM/YYYY)
							cmdLCD(GOTO_LINE2_POS0+6);   // Position cursor at the year 
							year=ReadNum(4);  // Read four-digit year from keypad
							delay_ms(200);  // Small delay after input
							// Validate year (Valid range: 1 to 4095)
							if(year<=0 || year>=4096)
							{
								cmdLCD(CLEAR_LCD);
								cmdLCD(GOTO_LINE1_POS0);
								strLCD((signed char *)"Invalid entry:");  // Display error message
								delay_ms(500);  // Wait before asking again
								goto year;  // Ask user to re-enter the year
							}
							else
							{
								 // Update the RTC with the new year
								SetRTCDateInfo(date,month,year);
							}
							break;  // Return to RTC Edit Menu
						//for exit
						case '8':	
							break;   // Exit from the current RTC Edit switch
						default:cmdLCD(CLEAR_LCD);
							cmdLCD(GOTO_LINE1_POS0);
							strLCD((signed char *)"invalid input:");
							delay_ms(500);
							break;
						}
						// If user selected '8', exit the RTC Edit menu
						if(key=='8')
							break;
					}// End of RTC Edit menu loop
					break;// Return to the Main Configuration Menu

				//------------------------------------------------------------
                // Medicine Schedule Configuration Menu
                //------------------------------------------------------------
				case '2':
					while(1)
					{
						cmdLCD(CLEAR_LCD);
						cmdLCD(GOTO_LINE1_POS0);
						strLCD((signed char *)"1.m1  2.m2  3.m3");  // Display medicine options
						cmdLCD(GOTO_LINE2_POS0);  // Move cursor to Line 2
						strLCD((signed char *)"4.exit");  // Display Exit option
						key=keyScan();   // Read user selection
						switch(key)
						{
			 			//----------------- Medicine 1 Configuration-------------------
							case '1':
								while(1)
								{
									cmdLCD(CLEAR_LCD);  // Clear LCD
									cmdLCD(GOTO_LINE1_POS0);  // Move cursor to Line 1
									// Display Medicine 1 edit menu
									strLCD((signed char*)"1.HH  2.MM  3.exit");
									key=keyScan();  // Read user's choice
									switch(key)
									{	 
									//------------- Medicine 1 - Hour Setting--------------------
		                              case '1':
										m1hour:cmdLCD(CLEAR_LCD);																	
											  cmdLCD(GOTO_LINE1_POS0);
										       // Prompt user to enter medicine hour
										       strLCD((signed char *)"Enter hour:");  
										       cmdLCD(GOTO_LINE2_POS0);
										       // Display time format
										       strLCD((signed char *)"00:00:00");
										       cmdLCD(GOTO_LINE2_POS0);  // Position cursor at hour 
										       m1hr=ReadNum(2);  // Read two-digit hour
										       delay_ms(200);  // Small delay
											// Validate hour (0–23)
											if(m1hr>23)
											{
												cmdLCD(CLEAR_LCD);
												cmdLCD(GOTO_LINE1_POS0);
												// Display error message
												strLCD((signed char *)"Invalid entry");
												delay_ms(500);
												goto m1hour; // Ask user to enter again
											}
											else
											{
												SetRTCTimeInfo(hour,minute,second);
											}
											break;
									//---------------- Medicine 1 - Minute Setting-----------------------
										case '2':
											m1minute:cmdLCD(CLEAR_LCD);  // Clear the LCD screen
											cmdLCD(GOTO_LINE1_POS0);
											// Ask user to enter Medicine 1 minute
											strLCD((signed char *)"Enter minute:");							                                                		    cmdLCD(GOTO_LINE2_POS0);
							                strLCD((signed char *)"00:00:00");  // Display time format
											cmdLCD(GOTO_LINE2_POS0+3);  // Position cursor at the minute
											m1min=ReadNum(2);  // Read two-digit minute
											delay_ms(200);  // Small delay after input
											// Validate minute (Valid range: 0–59)
											if(m1min>59)
											{																				
												cmdLCD(CLEAR_LCD);
											    cmdLCD(GOTO_LINE1_POS0);
											    strLCD((signed char *)"Invalid entry:");// Display error message
											    delay_ms(500);
											    goto m1minute;    // Ask user to re-enter minute
											}
											else
											{
												// Valid Medicine 1 minute entered                                                                                                                          // (Medicine schedule is stored in m1min)
												SetRTCTimeInfo(hour,minute,second);
											}
											break;

									//--------------- Exit Medicine 1 Configuration-------------------
										case '3':break;  // Exit Medicine 1 menu
                                                                                         
										//Invalid menu section
										default:cmdLCD(CLEAR_LCD);
											cmdLCD(GOTO_LINE1_POS0);
											strLCD((signed char *)"invalid input:");
											delay_ms(500);
											break;
									}
									// If user selected Exit ('3'), leave Medicine 1 menu
									if(key=='3')
										 break;
								}// End of Medicine 1 configuration loop
								break;  // Return to Medicine Schedule menu

								//---------------------- Medicine 2 Configuration-----------------------
								case '2':
									while(1)
									{
										cmdLCD(CLEAR_LCD);  
										cmdLCD(GOTO_LINE1_POS0);
										// Display Medicine 2 configuration menu
										strLCD((signed char*)"1.HH  2.MM  3.exit");
										key=keyScan(); // Read user's selection
										switch(key)
										{
											//------------------ Medicine 2 - Hour Setting--------------
										        case '1':
												m2hour:cmdLCD(CLEAR_LCD);// Clear LCD
												cmdLCD(GOTO_LINE1_POS0);
												// Prompt user to enter Medicine 2 hour
												strLCD((signed char *)"Enter hour:");
												cmdLCD(GOTO_LINE2_POS0);
												// Display time format
												strLCD((signed char *)"00:00:00");
												cmdLCD(GOTO_LINE2_POS0);// Position cursor at hour 
												m2hr=ReadNum(2);// Read two-digit hour
												delay_ms(200);// Small delay after input
												// Validate hour (Valid range: 0–23)
												if(m2hr>23)
												{
													cmdLCD(CLEAR_LCD);// Clear LCD
													cmdLCD(GOTO_LINE1_POS0);
													// Display error message
													strLCD((signed char *)"Invalid entry");
													delay_ms(500);
													goto m2hour;// Prompt user to re-enter hour
												}
												else
												{
													// Valid Medicine 2 hour entered
                                                   // (Medicine time is stored in m2hr)
													SetRTCTimeInfo(hour,minute,second);
												}
												break;
											//------------ Medicine 2- Minute setting-----------
										          case '2':
												m2minute:cmdLCD(CLEAR_LCD);
												cmdLCD(GOTO_LINE1_POS0);
												 // Prompt user to enter Medicine 2 minute
												strLCD((signed char *)"Enter minute:");
												cmdLCD(GOTO_LINE2_POS0);
												strLCD((signed char *)"00:00:00");
												cmdLCD(GOTO_LINE2_POS0+3);
												m2min=ReadNum(2); //Read two-digit minute
												delay_ms(200);// Small delay after input
												// Validate hour (Valid range: 1–59)
												if(m2min>59)
												{
													cmdLCD(CLEAR_LCD);
													cmdLCD(GOTO_LINE1_POS0);
													strLCD((signed char *)"Invalid entry:");
													delay_ms(500);
													goto m2minute; //Ask user to re-enter minute
												}
												else
												{
													 // Valid Medicine 2 minute entered
                                                     // (Medicine time is stored in m2minute)
													SetRTCTimeInfo(hour,minute,second);
												}
												break;
											//--------Exit for medicine 2------------
				        						case '3':break;// Exit Medicine 2 configuration menu
                                              //-----invalid menu section------
			              							default:cmdLCD(CLEAR_LCD);
												cmdLCD(GOTO_LINE1_POS0);
												strLCD((signed char *)"invalid input:");
												delay_ms(500);
												break;
										}
										// If user selected Exit ('3'), leave the Medicine 2 menu
										if(key=='3')
											break;
									}// End of Medicine 2 configuration loop
									break;// Return to the Medicine Schedule Configuration menu
									//------------Medicine 3 configuration------------
									case '3':
										while(1)
										{
										      cmdLCD(CLEAR_LCD);
						              			      cmdLCD(GOTO_LINE1_POS0);
										      // Display Medicine 3 configuration menu
									              strLCD((signed char*)"1.HH  2.MM  3.exit");
								         	      key=keyScan();  // Read user's selection
										      switch(key)
										      {
								            		     //hour for medicine 3
											      case '1':
												m3hour:cmdLCD(CLEAR_LCD);
												cmdLCD(GOTO_LINE1_POS0);
												 // Prompt user to enter Medicine 3 hour
												strLCD((signed char *)"Enter hour:");														           
											    cmdLCD(GOTO_LINE2_POS0);
							                    strLCD((signed char *)"00:00:00");  // Display time format
												cmdLCD(GOTO_LINE2_POS0);// Position cursor at the hour
												m3hr=ReadNum(2);  // Read two-digit hour
												delay_ms(200);	// Small delay after input
												 // Validate hour (Valid range: 0 to 23)
												if(m3hr>23)
												{
													cmdLCD(CLEAR_LCD);
													cmdLCD(GOTO_LINE1_POS0);																 
													strLCD((signed char *)"Invalid entry");
													delay_ms(500);
													goto m3hour;   // Prompt user to re-enter hour
												}
												else
												{
													// Valid Medicine 3 hour entered
                											// Medicine 3 hour is stored in m3hr
													SetRTCTimeInfo(hour,minute,second);
												}
												break;
											//-----minute for medicine 3--------
											case '2':
												m3minute:cmdLCD(CLEAR_LCD);
													cmdLCD(GOTO_LINE1_POS0);
													// Prompt user to enter Medicine 3 minute
													strLCD((signed char *)"Enter minute:");
													cmdLCD(GOTO_LINE2_POS0);
							                        strLCD((signed char *)"00:00:00");  // Display time format
													cmdLCD(GOTO_LINE2_POS0+3);// Position cursor at the minute
													m3min=ReadNum(2); // Read two-digit minute
	  												delay_ms(200);	// Small delay after input
													// Validate minute (Valid range: 0 to 59)
													if(m3min>59)
													{
														cmdLCD(CLEAR_LCD);																	   
														cmdLCD(GOTO_LINE1_POS0);
														strLCD((signed char *)"Invalid entry:");
														delay_ms(500);
														goto m3minute; //Ask user to re-enter minute
													}
													else
													{
														// Valid Medicine 3 minute entered
                                                        // Medicine 3 minute is stored in m3min
														SetRTCTimeInfo(hour,minute,second);
													}
													break;																		
											//--------exit for medicine 3----------
											case '3':break; // Exit Medicine 3 configuration menu
											default:cmdLCD(CLEAR_LCD);
												cmdLCD(GOTO_LINE1_POS0);
												strLCD((signed char *)"invalid input:");
												delay_ms(500);
												break;
										      }
										      // If user selected Exit ('3'), leave the Medicine 3 menu
										      if(key=='3')
											      break;
										}// End of Medicine 3 configuration loop
										break;// Return to Medicine Schedule Configuration menu
									//---------Exit-------
									case '4':break;  // Exit Medicine Schedule menu
									//Invalid menu selection
									default:cmdLCD(CLEAR_LCD);
									cmdLCD(GOTO_LINE1_POS0);															           
									strLCD((signed char *)"invalid input:");
									delay_ms(500);								
									break;
								}
						// If user selected Exit ('4'), leave the Medicine Schedule menu
								if(key=='4')
									break;
							}// End of Medicine Schedule configuration loop
					//--------Exit Menu-------
						case '3':break; // Exit the main configuration menu
						//Invalid menu selection
						default:cmdLCD(CLEAR_LCD);
						cmdLCD(GOTO_LINE1_POS0);
						strLCD((signed char *)"invalid input:");								
						delay_ms(500);
						break;
	      	}
		// If user selected Exit ('3'), leave the main configuration menu
		if(key=='3')
			break;
	  }// End of configure() function
	
}
int main()
{
	InitLCD();
	RTC_Init();
	InitKPM();
	IODIR0|=1<<buzzer;  // Configure buzzer pin as output
	IOCLR0=1<<buzzer;  // Initially turn OFF the buzzer
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
		// Read current RTC time
		GetRTCTimeInfo(&hour, &minute, &second);
	
		if(flag1)
		{
			flag2=1; // Configuration mode active
			configure();  // Open configuration menu
			flag1=0;  // Reset configuration request
			flag2=0;
			cmdLCD(CLEAR_LCD);
		}
		
		//-------update every second--------
		if(second !=last_second)
		{
			last_second=second; //update to current time
			if(cnt>0)
			{
				cnt--;
				// Stop alarm after countdown expires
				if(cnt==0)
				{
					IOCLR0=1<<buzzer;  // Turn OFF buzzer
					last_min=-1;  // Reset alarm minute
					flag3=0;   // Disable medicine reminder
					cmdLCD(CLEAR_LCD);
				}
			}
		}
	//check medicine time to take
		if(flag3)
		{
			// Check only once every minute
		if(minute != last_min)
		{
			// Compare RTC time with all medicine schedules
			if(((hour==m1hr)&&(minute==m1min)) || ((hour==m2hr)&&(minute==m2min)) || ((hour==m3hr)&&(minute==m3min)))
			{
				last_min=minute;  // Store alarm minute
				cnt=59;  // Alarm duration (59 seconds)
				cmdLCD(CLEAR_LCD);	
			}
		}
		}
		if(last_min==minute && cnt>0)
		{
			// Turn ON buzzer
		        IOSET0=1<<buzzer;
			cmdLCD(CLEAR_LCD);
			cmdLCD(GOTO_LINE1_POS0);
			strLCD((signed char *)"TAKE MEDICINE");  // Display reminder message
			cmdLCD(CLEAR_LCD);
	        }
		else
		{
			IOCLR0=1<<buzzer;		
			cmdLCD(GOTO_LINE1_POS0);  // Display current time
			GetRTCTimeInfo(&hour, &minute, &second);
			DisplayRTCTime(hour,minute, second);
			cmdLCD(GOTO_LINE2_POS0);  // Display current date
			GetRTCDateInfo(&date, &month, &year);
			DisplayRTCDate(date, month, year);
			cmdLCD(GOTO_LINE1_POS0+10);  // Display current day
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
	
	// Check whether the buzzer is currently ON
	if(((IOPIN0>>buzzer)&1)==1)
	{
	//clearing the buzzer
	IOCLR0=1<<buzzer;
	 // Reset buzzer timer/count
	cnt=0;
	cmdLCD(CLEAR_LCD);
	strLCD((signed char *)"medicine taken");  // Display confirmation message
	delay_ms(200);
	cmdLCD(CLEAR_LCD);  // Clear the LCD again
	}
	else if(flag2==1)
	{
	}
	else
	{
		cmdLCD(CLEAR_LCD);
		strLCD((signed char*)"No Reminders");//Visual configuration
		delay_ms(1000);
		cmdLCD(CLEAR_LCD);
	}
	
}



