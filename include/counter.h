// Constant memory locations for score elements
char * const counterLms = 0x20;

// score digits on left
char * const scoreD0 = counterLms;
char * const scoreD1 = scoreD0 + 1;
char * const scoreD2 = scoreD1 + 1;
char * const scoreD3 = scoreD2 + 1;
char * const scoreD4 = scoreD3 + 1;

// score alt digits on right
char * const scoreA0 = counterLms + 0x23;
char * const scoreA1 = scoreA0 + 1;
char * const scoreA2 = scoreA1 + 1;
char * const scoreA3 = scoreA2 + 1;
char * const scoreA4 = scoreA3 + 1;

// The VBL routine for incrementing the benchmark counter timer
void vblCounter();

// Initialise counter charset etc.
void initCounter();

// setup the name and initialise the counter
void prepareCounter(char *name);

// enable the counter, 0 = off, 1 = on
void counterOn(char onoff);

// Print the current counter data
void counterPrint();

// Convert a word into a 5 digit value
void numberToDigits(word n, char *digits);

// Shows the value of n in the score line as the last 5 digits. Useful for playing back a value.
void showAltValue(word n);

// Clear the alt score
void clearAltScore();
