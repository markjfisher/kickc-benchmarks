#include "atari-system.h"
#include "counter.h"

void benchmarkGuessing() {
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
