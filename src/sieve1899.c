#pragma target(atarixl)
#pragma encoding(atascii)
#pragma zp_reserve(0x00..0x7f)

#include <atari-xl.h>
#include <string.h>
#include "atari-system.h"
#include "counter.h"
#include "gr.h"

void runSieve1899() {
	memset(lms, 0, 0x1ff0);
	prepareCounter("Sieve 1899 10x");
	counterOn(1);
	word count = benchmarkSieve1899();
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

word benchmarkSieve1899() {
	word count = 0;
	for (signed char loop = 9; loop >= 0; loop--) {
		memset(sieveFlags, 1, 0x2000);
		count = 0;
		word i = 0;
		while (i < 0x2000) {
			if (sieveFlags[i] == 1) {
				word prime = 3 + (i << 1);
				word k = prime + i;
				while (k < 0x2000) {
					sieveFlags[k] = 0;
					k += prime;
				}
				count++;
			}
			i++;
		}
	}
	return count;
}
