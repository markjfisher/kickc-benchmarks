#pragma target(atarixl)
#pragma encoding(atascii)

#include "atari-system.h"
#include "counter.h"

word * const squares = 0x6000; // reuse the sieve data area

void runMontecarlo() {
	memset(lms, 0, 0x1ff0);
	prepareCounter("Monte Carlo Pi 10K");
	init_squares();

	counterOn(1);
	word pi = benchmarkMontecarlo();
	counterOn(0);

	showAltValue(pi);
	waitFrames(120);
	counterPrint();
}

word benchmarkMontecarlo() {
	char n;
	word bingo = 0;
	word r = 127 * 127;
	for (word probe = 0; probe < 10000; probe++) {
		n = (*RANDOM) & 0x7F;
		word x = sqr(n);
		n = (*RANDOM) & 0x7F;
		word y = sqr(n);
		if ((x + y) <= r) bingo++;
	}
	word pi = bingo << 2;
	return pi;
}

// Pre-calculate squares of integers 0..127 (max value used)
// Create a 127 word table for the sqr function.
void init_squares() {
	word * sp = squares;
    word sqr = 0;
    for(char i: 0..127) {
        *sp++ = sqr;
        sqr += i*2+1;
    }
}

word sqr(char n) {
	return *(squares + n);
}
