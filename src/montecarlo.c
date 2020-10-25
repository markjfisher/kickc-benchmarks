#pragma target(atarixl)
#pragma encoding(atascii)
#pragma zp_reserve(0x00..0x7f)

// As kickc has no native dynamic multiplication, use the sqr libray it provides instead
#include <sqr.h>
#include "atari-system.h"
#include "counter.h"

void runMontecarlo() {
	memset(lms, 0, 0x1ff0);
	prepareCounter("Monte Carlo Pi 10K");
	init_squares();
	counterOn(1);
	word pi = benchmarkMontecarlo();
	counterOn(0);

	char digits[5];
	numberToDigits(pi, digits);
	for(char i: 0..4) {
		*(counterLms + 0x23 + i) = digits[i];
	}
	waitFrames(120);
	counterPrint();
}

word benchmarkMontecarlo() {
	char n;
	word bingo = 0;
	word r = 127 * 127;
	for (signed word probe = 9999; probe >= 0; probe--) {
		n = (*RANDOM) & 0x7F;
		word x = sqr(n);
		n = (*RANDOM) & 0x7F;
		word y = sqr(n);
		if ((x + y) < r) bingo++;
	}
	word pi = bingo << 2;
	return pi;
}
