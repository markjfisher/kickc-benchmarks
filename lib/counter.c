
#include <stdlib.h>
#include <atari-xl.h>
#include <string.h>
#include "atari-system.h"
#include "counter.h"
#include "gr.h"

// The current benchmark's name - has to be 1 larger than the actual max string (25) for the 0 to terminate the string.
char benchName[26];

// Where to put the next score, start at scoreLms, then increment row by row when printing
char *currentPrintPosition = scoreLms;

// counter on off flag, used in the VBL, so make it volatile
volatile char counterOnValue = 0;

// 0 = off, 1 = on
void counterOn(char onoff) {
	counterOnValue = onoff;
}

void clearAltScore() {
	memset(scoreA0, 0, 5);
}

void vblCounter() {
	if (counterOnValue != 0) {
		(*scoreD4)++;
		if (*scoreD4 == 10) { *scoreD4 = 0; (*scoreD3)++; }
		if (*scoreD3 == 10) { *scoreD3 = 0; (*scoreD2)++; }
		if (*scoreD2 == 10) { *scoreD2 = 0; (*scoreD1)++; }
		if (*scoreD1 == 10) { *scoreD1 = 0; (*scoreD0)++; }
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
	for(register char i: 0..7) {
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
	memset(benchName, 0, 26);
	word len = strlen(name);
	if (len > 25) len = 25; // ensure there's a zero at the end
	memcpy(benchName, name, len);
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
	for (char i: 0..24) {
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

void showAltValue(word n) {
	char digits[5];
	numberToDigits(n, digits);
	for(register char i: 0..4) {
		*(scoreA0 + i) = digits[i];
	}

}

void counterOverwrite() {
	// copy the last 5 chars of the counter display (the temporary counter for some benchmarks)
	// and put it in the first 5 so it gets copied correctly at end of test into the score screen.
	memcpy(counterLms, scoreA0, 5);
}

void numberToDigits(word n, char *digits) {
	memset(digits, 0, 5);
	double_dabble(n, digits);
}

// taken from https://en.wikipedia.org/wiki/Double_dabble
// converts a word into 5 bytes of BCD values, highest number first, no removal of zeroes.
void double_dabble(word n, char *digits) {
	char nbits = 16;
	char nscratch = 5; // nbits / 3
	char scratch[6];   // nscratch + 1
	char smin = 3;     // nscratch - 2
	
	char j, k;

	for(char i: 0..5) scratch[i] = 0;
	
	for(j: 0..15) {
		// this will be shifted in on the right
		word sh = 1 << (15 - j);
		word v = n & sh;
		char shifted_in; // = ((n & (1 << (15 - j))) > 0) ? 1 : 0;
		if (v != 0) {
			shifted_in = 1;
		} else {
			shifted_in = 0;
		}
	
		// add 3 everywhere that scratch[k] >= 5
		for (char k = smin; k < nscratch; k++) {
			if (scratch[k] >= 5) {
				scratch[k] += 3;
			}
		}
		
		// shift scratch to the left by 1
		if (scratch[smin] >= 8) {
			smin--;
		}
		
		for (char k = smin; k < nscratch - 1; k++) {
			scratch[k] <<= 1;
			scratch[k] &= 0xf;
			if (scratch[k+1] >= 8) {
				scratch[k] |= 1;
			}
		}
		
		// shift in the new bit
		scratch[nscratch - 1] <<= 1;
		scratch[nscratch - 1] &= 0xf;
		scratch[nscratch - 1] |= shifted_in;
	}
	
	// This part of the implementation was left out:
	//// Remove leading zeros from scratch space
	//for (k = 0; k < nscratch; k++) {
		//if (scratch[k] != 0) break;
	//}
	//nscratch -= k;
	//memmove(scratch, scratch + k, nscratch + 1);
	
	// copy scratch into outgoing array
	for (char i: 0..4) {
		digits[i] = scratch[i];
	}
	
}
