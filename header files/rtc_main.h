#include "types.h"

// System clock and peripheral clock Macros
#define FOSC 12000000   //12MHZ
#define CCLK (5*FOSC)   //60MHZ 
#define PCLK (CCLK/4)   //15MHZ

// RTC Prescaler Calculation Macros
// RTC requires 32.768 kHz clock for 1-second increment.
// PREINT and PREFRAC registers divide PCLK to generate 32.768 kHz.

// PREINT = int (PCLK / 32768) - 1;
// PREFRAC = PCLK -((PREINT + 1) * 32768);
// Note: This information collected from LPC2129 Manual

// (((15000000) / 32768) - 1) = 456
#define PREINT_VAL (int) ((PCLK / 32768) - 1)
	
//(15000000 - ((456 + 1) * 32768)) = 25024
#define PREFRAC_VAL (PCLK -((PREINT_VAL + 1) * 32768))

//RTC Control Register (CCR) Bit Definitions
// Bit 0 – Clock Enable --> 1 = Enable RTC counters  0 = Disable RTC counters
#define RTC_ENABLE (1<<0)

// Bit 1 – Clock Reset --> 1 = Reset RTC counters    0 = Normal operation
#define RTC_RESET (1<<1)
 

//only for LPC2148
// Bit 4 – Clock Source Select 
// 1 = Use external 32.768 kHz oscillator
// 0 = Use internal PCLK as RTC clock source
#define RTC_CLKSRC (1<<4)


#define SUN 0
#define MON 1
#define TUE 2
#define WED 3
#define THU 4
#define FRI 5
#define SAT 6

// Initializes the Real-Time Clock (RTC)
void RTC_Init(void);

// Reads the current RTC time (Hour, Minute, Second)
void GetRTCTimeInfo(s32 *,s32 *,s32 *);

// Displays the current time on the LCD
void DisplayRTCTime(u32,u32,u32);

// Reads the current RTC date (Date, Month, Year)
void GetRTCDateInfo(s32 *,s32 *,s32 *);

// Displays the current date on the LCD
void DisplayRTCDate(u32,u32,u32);

// Sets the RTC time (Hour, Minute, Second)
void SetRTCTimeInfo(u32,u32,u32);

// Reads the current day of the week
void GetRTCDay(s32 *);

// Displays the current day (Sunday to Saturday) on the LCD
void DisplayRTCDay(u32);

// Sets the current day of the week
void SetRTCDay(u32);


