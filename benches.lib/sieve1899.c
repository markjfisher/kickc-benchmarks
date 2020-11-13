
#include <atari-xl.h>
#include <string.h>
#include <6502.h>
#include "atari-system.h"
#include "counter.h"
#include "gr.h"
#include "sieve.h"

void runSieve1899() {
	memset(lms, 0, 0x1ff0);
	prepareCounter("Sieve 1899 10x            \x1d");

	counterOn(1);
	word count = benchmarkSieve1899();
	counterOn(0);

	showAltValue(count);
	waitFrames(120);
	counterPrint();

}

word benchmarkSieve1899() {
	word k, prime;
	word count = 0;
	for (char loop: 0..9) {
		clearSieveDataAsm();
		count = 0;
		word i = 0;
		for (word i = 0; i < 0x2000; i++) {
			if (sieveFlags[i] != 0) {
				prime = 3 + i * 2;
				count++;
				for (k = i + prime; k < 0x2000; k += prime) {
					sieveFlags[k] = 0;
				}
			}
		}
	}
	return count;
}
