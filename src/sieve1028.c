#pragma target(atarixl)
#pragma encoding(atascii)
#pragma zp_reserve(0x00..0x7f)

#include <atari-xl.h>
#include <string.h>
#include "atari-system.h"
#include "counter.h"
#include "gr.h"

void runSieve1028() {
	memset(lms, 0, 0x1ff0);
	prepareCounter("Sieve 1028 10x");
	counterOn(1);
	word count = benchmarkSieve1028();
	counterOn(0);

	// show the score and pause
	char digits[5];
	numberToDigits(count, digits);
	for(char i: 0..4) {
		*(counterLms + 0x23 + i) = digits[i];
	}
	waitFrames(120);
	counterPrint();

}

// used by both sieve routines
char sieveFlags[0x2000];

word benchmarkSieve1028() {
	word count = 0;
	for (signed char loop = 9; loop >= 0; loop--) {
		char count = 0;
		memset(sieveFlags, 1, 0x2000);
		for (char n = 2; n < 92; n++) {
			if (sieveFlags[n] == 1) {
				word k = n << 1;
				while (k < 0x2000) {
					sieveFlags[k] = 0;
					k += n;
				}
			}
		}
	}
	
	for (word k = 2; k < 0x2000; k++) {
		if (sieveFlags[k] == 1) count++;
	}
	return count;
}
