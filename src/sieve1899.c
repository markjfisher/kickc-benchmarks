#pragma target(atarixl)
#pragma encoding(atascii)

#include <atari-xl.h>
#include <string.h>
#include "atari-system.h"
#include "counter.h"
#include "gr.h"
#include "sieve.h"

void runSieve1899() {
	memset(lms, 0, 0x1ff0);
	prepareCounter("Sieve 1899 10x");

	counterOn(1);
	word count = benchmarkSieve1899();
	counterOn(0);

	showAltValue(count);
	waitFrames(120);
	counterPrint();

}

word benchmarkSieve1899() {
	word count = 0;
	for (char loop: 9..0) {
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
