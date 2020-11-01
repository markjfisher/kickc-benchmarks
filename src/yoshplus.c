#include "atari-system.h"
#include "counter.h"

void benchmarkYoshplus() {
	clearAltScore();
	*(RTCLOK + 2) = 0;

	while (*(RTCLOK + 2) < 100) {
		(*scoreA4)++;
		if (*scoreA4 == 10) { *scoreA4 = 0; (*scoreA3)++; }
		if (*scoreA3 == 10) { *scoreA3 = 0; (*scoreA2)++; }
		if (*scoreA2 == 10) { *scoreA2 = 0; (*scoreA1)++; }
		if (*scoreA1 == 10) { *scoreA1 = 0; (*scoreA0)++; }
	}
}
