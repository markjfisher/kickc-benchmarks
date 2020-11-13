
#include <atari-xl.h>
#include "atari-system.h"
#include "counter.h"
#include "gr.h"

void runChessboard() {
	memset(lms, 0, 0x1ff0);
	prepareCounter("Chessboard GR8 200 frames \x1c");
	counterOn(1);
	benchmarkChessboard();
	counterOn(0);
	waitFrames(10);
	counterOverwrite();
	counterPrint();
}

void benchmarkChessboard() {
	mode8();
	clearAltScore();

	char *c1 = counterLms + 0x25;
	char *c2 = counterLms + 0x26;
	char *c3 = counterLms + 0x27;
	
	*(RTCLOK + 2) = 0;
	while (*(RTCLOK + 2) < 200) {
		char *p = lms;
		for (char i3: 7..0) {
			for (char i2: 23..0) {
				for (char i1: 3..0) {
					*p     = 0xff;
					*(p+1) = 0xff;
					*(p+2) = 0xff;
					p     += 6;
				}
				p += 16;
			}
			if ((i3 & %1) == 0) {
				p -= 3;
			} else {
				p += 3;
			}
		}
		(*c3)++;
		if (*c3 == 10) { *c3 = 0; (*c2)++; }
		if (*c2 == 10) { *c2 = 0; (*c1)++; }
	}
}
