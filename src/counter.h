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
