#pragma target(atarixl)
#pragma encoding(atascii)
#pragma zp_reserve(0x00..0x7f)

#include <stdlib.h>
#include <atari-xl.h>
#include "atari-system.h"
#include "gr.h"

// The current benchmark's name
char benchName[25];

// Where to put the next score, start at scoreLms, then increment row by row when printing
char *currentPrintPosition = scoreLms;

// counter on off flag, used in the VBL, so make it volatile
volatile char counterOnValue = 0;

// 0 = off, 1 = on
void counterOn(char onoff) {
	counterOnValue = onoff;
}

interrupt(hardware_clobber) void vblCounter() {
	char * const a = 0x20;
	char * const b = 0x21;
	char * const c = 0x22;
	char * const d = 0x23;
	char * const e = 0x24;

	if (counterOnValue != 0) {
		(*e)++;
		if (*e == 10) { *e = 0; (*d)++; }
		if (*d == 10) { *d = 0; (*c)++; }
		if (*c == 10) { *c = 0; (*b)++; }
		if (*b == 10) { *b = 0; (*a)++; }
	}
}

void initCounter() {
	// Create a second copy of charset at 0x8400
	memcpy(charset + 0x400, charset, 0x400);

	// Copy antic char 16..25 (the numbers 0..9) into start of 0x8400
	// This is so char/byte value of 0 will show the character "0" on screen without having to translate it.
	memcpy(charset + 0x400, charset + 0x80, 80);
	
	// single character at the very end of 2nd charset = inverted 0.
	// This means any 0xff value will show as "0" (is there double inversion here?).
	for(char i: 0..7) {
		*(charset + 0x800 - 0x08 + i) = *(charset + 0x80 + i) ^ 0xff;
	}
	
	// Set 2nd last character to "inverted space" (all 0xff)
	// i.e. value 0xfe shows as inverted? space
	memset(charset + 0x800 - 0x10, 0xff, 8);
}

void prepareCounter(char *name) {
	waitFrame();

	// change graphics mode to simple 1 line 40 byte message
	counterRow();

	// Use our 2nd copy of the characterset with changed values for displaying numbers and spaces.
	*CHBASE = >(charset + 0x400);

	// copy name into benchName, preserving the original string
	memset(benchName, 0, 25);
	memcpy(benchName, name, strlen(name));
	// change and fix the atascii string to be displayable in our charset where spaces are at 0xfe, and rest have to be antic codes.
	strToCode(benchName);

	// 0xfe = space in our charset, so this blanks the counterLms memory with 40 spaces.
	memset(counterLms, 0xfe, 0x28);
	
	// value 0 = character "0" in our charset, so display "00000"
	memset(counterLms, 0, 5);
	
	// display the bench name 1 char after the counter.
	memcpy(counterLms + 6, benchName, strlen(name));
}

void counterPrint() {
	char *position = currentPrintPosition;
	
	// benchName was changed to be displayed on 2nd charset where spaces are 0xfe, so change them back
	for (char i = 0; i<25; i++) {
		if (*(benchName + i) == 0xfe) *(benchName + i) = 0;
	}

	memcpy(position, benchName, 25);
	position += 26;
	for(char i: 0..4) {
		// this adjusts for the charset differences. counterLms has binary 0 for "0",
		// but to display in original charset, add 16 to get to antic codes 16..25 for "0" to "9"
		*(position + i) = *(counterLms + i) + 16;
	}
	// move to next line for next score
	currentPrintPosition += 40;
}

void counterOverwrite() {
	// copy the last 5 chars of the counter display (the temporary counter for some benchmars)
	// and put it in the first 5 so it gets copied correctly at end of test into the score screen.
	memcpy(counterLms, counterLms + 0x23, 5);
}
