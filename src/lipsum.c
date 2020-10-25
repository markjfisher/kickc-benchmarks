#pragma target(atarixl)
#pragma encoding(atascii)
#pragma zp_reserve(0x00..0x7f)

#include <atari-xl.h>
#include "atari-system.h"
#include "counter.h"
#include "gr.h"

void runLipsum() {
	memset(lms, 0, 0x1ff0);
	prepareCounter("Lorem Ipsum GR8");
	counterOn(1);
	benchmarkLipsum();
	counterOn(0);
	waitFrames(10);
	counterPrint();
}

void benchmarkLipsum() {
	char *text = "Lorem ipsum dolor sit amet orci aliquam.";
	strToCode(text);
	
	mode8();
	for (signed char iter = 28; iter >= 0; iter--) {
		writeGr8(0, (char) iter, text);
	}
}

void writeGr8(char x, char y, char *text) {
	word y320 = ((word) y) * 320;
	char * screenPosition = (lms + y320) + x - 1;
	for (char i2 = (char) strlen(text); i2 >= 1; i2--) {
		// char ord = convertCodeToAtascii(*(text + i2));
		char ord = *(text + i2);
		char * textChar = charset + ord * 8;
		char * t = screenPosition + i2;
		for (signed char i1 = 7; i1 >= 0; i1--) {
			char * a = t + i1 * 40;
			char * b = textChar + i1;
			*a = *b;
		}
	}
}
