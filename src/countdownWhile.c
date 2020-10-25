#pragma target(atarixl)
#pragma encoding(atascii)
#pragma zp_reserve(0x00..0x7f)

#include <atari-xl.h>
#include "atari-system.h"
#include "counter.h"
#include "gr.h"

void runCountdownWhile() {
	memset(lms, 0, 0x1ff0);
	prepareCounter("Countdown 2ML: WHILE");
	counterOn(1);
	benchmarkCountdownWhile();
	counterOn(0);
	waitFrames(10);
	counterPrint();
}

void benchmarkCountdownWhile() {
	char *c1 = counterLms + 0x21;
	char *c2 = counterLms + 0x22;
	char *c3 = counterLms + 0x23;
	char *c4 = counterLms + 0x24;
	char *c5 = counterLms + 0x25;
	char *c6 = counterLms + 0x26;
	char *c7 = counterLms + 0x27;

	*c1 = 1;
	*c2 = 9;
	*c3 = 9;
	*c4 = 9;
	*c5 = 9;
	*c6 = 9;
	*c7 = 9;
	
	while (*c1 != 0xff) {
		*c2 = 9;
		while (*c2 != 0xff) {
			*c3 = 9;
			while (*c3 != 0xff) {
				*c4 = 9;
				while (*c4 != 0xff) {
					*c5 = 9;
					while (*c5 != 0xff) {
						*c6 = 9;
						while (*c6 != 0xff) {
							*c7 = 9;
							while (*c7 != 0xff) {
								(*c7)--;
							}
							(*c6)--;
						}
						(*c5)--;
					}
					(*c4)--;
				}
				(*c3)--;
			}
			(*c2)--;
		}
		(*c1)--;
	}
}
