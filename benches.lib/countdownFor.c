#pragma code_seg(Code)

#include <atari-xl.h>
#include "atari-system.h"
#include "counter.h"
#include "gr.h"

void runCountdownFor() {
	memset(lms, 0, 0x1ff0);
	prepareCounter("Countdown 2ML: FOR        \x1d");
	counterOn(1);
	benchmarkCountdownFor();
	counterOn(0);
	waitFrames(10);
	counterPrint();
}

void benchmarkCountdownFor() {
	for(__address(0x41) char a: 1..0) {
		for(__address(0x42) char b: 9..0) {
			for(__address(0x43) char c: 9..0) {
				for(__address(0x44) char d: 9..0) {
					for(__address(0x45) char e: 9..0) {
						for(__address(0x46) char f: 9..0) {
							for(__address(0x47) char g: 9..0) {
								// do nothing but loop...
							}
						}
					}
				}
			}
		}
	}
}
