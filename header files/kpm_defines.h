//Row pins
#define ROW0 16  //p1.16 - ROW 0
#define ROW1 17  //p1.17 - ROW 1
#define ROW2 18  //p1.18 - ROW 2
#define ROW3 19  //p1.19 - ROW 3

//col pins
#define COL0 20  //p1.20 - column 0
#define COL1 21  //p1.21 - column 1
#define COL2 22  //p1.22 - column 2
#define COL3 23  //p1.23 - column 3

// Initializes the keypad
void InitKPM(void);

// Checks whether any key is pressed
// Returns 0 if a key is pressed, otherwise returns 1
int colscan(void);

// Identifies the row number of the pressed key
int RowCheck(void);

// Identifies the column number of the pressed key
int ColCheck(void);

// Scans the keypad and returns the pressed key value
char keyScan(void);

// Reads an n-digit number from the keypad and returns it
int ReadNum(int);





