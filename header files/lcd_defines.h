#define pin 8
#define RS 16  // Register Select
#define RW 17  // Read/Write
#define EN 18  // Enable pin

//LCD commands

#define CLEAR_LCD 0x01  // Clear LCD 
#define RET_CUR_HOME 0x02  // Return cursor to home position

//Display Lcds

#define DISP_OFF 0x08  // Turn OFF display
#define DISP_ON 0x0c  // Turn ON display, cursor OFF
#define DISP_ON_CUR_ON 0x0E  // Display ON, cursor ON
#define DISP_ON_CUR_BLINK 0x0F  // Display ON, cursor blinking

//Mode bits

#define MODE_8BIT_1LINE 0x30  // 8-bit interface, 1-line display
#define MODE_8BIT_2LINE 0x38  // 8-bit interface, 2-line display
#define MODE_4BIT_1LINE 0x20  // 4-bit interface, 1-line display
#define MODE_4BIT_2LINE 0x28  // 4-bit interface, 2-line display

//position bits

#define GOTO_LINE1_POS0 0x80  // Move cursor to Line 1, Position 0
#define GOTO_LINE2_POS0 0xc0  // Move cursor to Line 2, Position 0
#define GOTO_LINE3_POS0 0x94  // Move cursor to Line 3, Position 0
#define GOTO_LINE4_POS0 0xD4  // Move cursor to Line 4, Position 0

#define SHIFT_CUR_RIGHT 0x06  // Move cursor to the right
#define SHIFT_CUR_LEFT 0x10  // Move cursor one position to the left
#define SHIFT_DISP_RIGHT 0x14  // Shift the entire display to the right

//display symbols

#define GOTO_CGRAM 0x40

