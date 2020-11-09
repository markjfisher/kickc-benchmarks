#include "atari-system.h"
#include "counter.h"

void runGuessing() {
	memset(lms, 0, 0x1ff0);
	prepareCounter("Guessing game 10x");

	counterOn(1);
	char d = benchmarkGuessing();
	counterOn(0);

	waitFrames(40);
	counterPrint();

	// stop compiler optimising last digit away
	*scoreA4 = d;
}

char benchmarkGuessing() {
	// Looping more shows signed to signed comparisons are faster than signed to non-signed
	for (char loop: 0..9) {
		clearAltScore();
		for(signed word r = 1; r <= 1000; r++) {
			signed word wg = 500;
			char i = 0;
			while(true) {
				if (wg > r) {
					wg -= (100 - i);
				} else if (wg < r) {
					wg += (100 - i);
				} else {
					(*scoreA4)++;
					if (*scoreA4 == 10) { *scoreA4 = 0; (*scoreA3)++; }
					if (*scoreA3 == 10) { *scoreA3 = 0; (*scoreA2)++; }
					if (*scoreA2 == 10) { *scoreA2 = 0; (*scoreA1)++; }
					break;
				}
				i++;
			}
		}
	}
	return *scoreA4;
}
