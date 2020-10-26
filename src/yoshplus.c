#include "atari-system.h"
#include "counter.h"

void runYoshplus() {
	memset(lms, 0, 0x1ff0);
	prepareCounter("YoshPlus 100 frames");
	counterOn(1);
	char c = benchmarkYoshplus();
	counterOn(0);
	counterOverwrite();
	waitFrames(40);
	counterPrint();
	// force the value not to be optimized away
	*(counterLms + 0x43) = c;
}

char benchmarkYoshplus() {
	// Map the RHS score digits to memory addresses. byte 0-9 displays as "0" to "9" on screen.
	//__address(0x41) char a;
	//__address(0x42) char b;
	__address(0x43) char c;
	__address(0x44) char d;
	__address(0x45) char e;
	__address(0x46) char f;
	__address(0x47) char g;

	memset(counterLms + 0x23, 0, 5);
	*(RTCLOK + 2) = 0;

	while (*(RTCLOK + 2) < 100) {
		g++;
		if (g == 10) { g = 0; f++; }
		if (f == 10) { f = 0; e++; }
		if (e == 10) { e = 0; d++; }
		if (d == 10) { d = 0; c++; }
	}
	// force c to be retained
	return c;

}
