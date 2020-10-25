#pragma target(atarixl)
#pragma encoding(atascii)
#pragma zp_reserve(0x00..0x7f)

#include <atari-xl.h>
#include "atari-system.h"
#include "counter.h"
#include "gr.h"

void runCountdownFor() {
	memset(lms, 0, 0x1ff0);
	prepareCounter("Countdown 2ML: FOR");
	counterOn(1);
	benchmarkCountdownFor();
	counterOn(0);
	waitFrames(10);
	counterPrint();
}

void benchmarkCountdownFor() {
	__address(0x41) signed char a;
	__address(0x42) signed char b;
	__address(0x43) signed char c;
	__address(0x44) signed char d;
	__address(0x45) signed char e;
	__address(0x46) signed char f;
	__address(0x47) signed char g;
	for(a = 1; a >= 0; a--) {
		for(b = 9; b >= 0; b--) {
			for(c = 9; c >= 0; c--) {
				for(d = 9; d >= 0; d--) {
					for(e = 9; e >= 0; e--) {
						for(f = 9; f >= 0; f--) {
							for(g = 0; g >= 0; g--) {
								// do nothing but loop...
							}
						}
					}
				}
			}
		}
	}
}
