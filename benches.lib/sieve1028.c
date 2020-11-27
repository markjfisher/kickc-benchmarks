#pragma code_seg(Code)

#include <atari-xl.h>
#include <string.h>
#include <stdlib.h>
#include "atari-system.h"
#include "counter.h"
#include "gr.h"
#include "sieve.h"

void runSieve1028() {
	memset(lms, 0, 0x1ff0);
	prepareCounter("Sieve 1028 10x            \x1d");

	counterOn(1);
	word count = benchmarkSieve1028();
	counterOn(0);

	showAltValue(count);
	waitFrames(120);
	counterPrint();

}

word benchmarkSieve1028() {
	word count = 0;
	word k;
	for (char loop: 0..9) {
		clearSieveDataAsm();
		for (char n: 2..91) {
			if (sieveFlags[n] != 0) {
				for (k = n << 1; k < 0x2000; k += n) {
					sieveFlags[k] = 0;
				}
			}
		}
	}
	
	for (word k = 2; k < 0x2000; k++) {
		if (sieveFlags[k] == 1) count++;
	}
	return count;
}
