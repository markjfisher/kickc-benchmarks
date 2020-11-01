#pragma target(atarixl)
#pragma encoding(atascii)

#include <atari-xl.h>
#include "atari-system.h"
#include "counter.h"
#include "gr.h"

void benchmarkCountdownFor() {
	__address(0x41) char a;
	__address(0x42) char b;
	__address(0x43) char c;
	__address(0x44) char d;
	__address(0x45) char e;
	__address(0x46) char f;
	__address(0x47) char g;
	for(a: 1..0) {
		for(b: 9..0) {
			for(c: 9..0) {
				for(d: 9..0) {
					for(e: 9..0) {
						for(f: 9..0) {
							for(g: 9..0) {
								// do nothing but loop...
							}
						}
					}
				}
			}
		}
	}
}
