#pragma code_seg(Code)

#include <atari-xl.h>
#include "atari-system.h"
#include "counter.h"
#include "gr.h"

void runQR1d() {
	memset(lms, 0, 0x1ff0);
	prepareCounter("QR 1D Array 200 frames    \x1c");
	counterOn(1);
	benchmarkQR1d();
	counterOn(0);
	waitFrames(10);
	counterOverwrite();
	counterPrint();
}

void benchmarkQR1d() {
	mode4();
	clearAltScore();

	char y = 0;
	*(RTCLOK + 2) = 0;
	while (*(RTCLOK + 2) < 200) {
		char *p = lms;
		for (char x: 0..174) {
			*(p + y++) = *(qr + x);
			if (y == 5) {
				// mode 4 has 10 bytes per line?
				p += 10;
				y = 0;
			}
		}
		(*scoreA4)++;
		if (*scoreA4 == 10) { *scoreA4 = 0; (*scoreA3)++; }
		if (*scoreA3 == 10) { *scoreA3 = 0; (*scoreA2)++; }
		if (*scoreA2 == 10) { *scoreA2 = 0; (*scoreA1)++; }
	}
}
