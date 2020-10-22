#include <atari-xl.h>
#include "counter.h"

void runBsort() {
	memset(lms, 0, 0x1ff0);
	prepareCounter("bsort");
	counterOn(1);
	benchmarkBsort();
	counterOn(0);
	waitFrames(5);
	counterPrint();
}

void benchmarkBsort() {
	waitFrames(25);
}
