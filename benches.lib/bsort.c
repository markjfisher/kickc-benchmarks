#pragma code_seg(Code)

#include <atari-xl.h>
#include "counter.h"
#include "gr.h"

void runBsort() {
	memset(lms, 0, 0x1ff0);
	prepareCounter("Bubble Sort: 255 elements \x1d");
	counterOn(1);
	benchmarkBsort();
	counterOn(0);
	waitFrames(10);
	counterPrint();
}

void benchmarkBsort() {
	char * const sortTable = 0x6000;
	// Make the test repeatable by actually initialising, rather than using kickasm fill
	for(char i: 0..254) {
		sortTable[i] = 0xff - i;
	}

	for (char loop: 253..0) {
		for (char j: 0..253) {
			char n1 = sortTable[j];
			char n2 = sortTable[j+1];
			if (n1 > n2) {
				sortTable[j]   = n2;
				sortTable[j+1] = n1;
			}
		}
	}
}
