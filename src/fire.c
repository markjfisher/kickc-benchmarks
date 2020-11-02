#pragma target(atarixl)
#pragma encoding(atascii)

#include <atari-xl.h>
#include <stdint.h>
#include <6502.h>
#include "atari-system.h"
#include "counter.h"
#include "gr.h"

void runFire() {
	memset(lms, 0, 0x1ff0);
	prepareCounter("Fire demo");
	counterOn(1);
	benchmarkFire();
	counterOn(0);
	waitFrames(10);
	counterPrint();
}

void benchmarkFire() {
	clearAltScore();
	*SDMCTL = 0x21;

	enableDLI(&priorOffFire);
	modeFire();
	*PRIOR = 0x40;
	GTIA->COLBK = 0x20;
	
	 for (uint8_t i: 0x00..0xff) {
		*(fireScreen + 0x000 + i) = 0;
		*(fireScreen + 0x100 + i) = 0;
		*(fireScreen + 0x200 + i) = 0;
		*(fireScreen + 0x300 + i) = 0;
	}

	uint8_t t = 0;
	uint8_t * cp = fireCharset;
	for (uint8_t i: 0..0x10) {
		for (uint8_t j: 0..7) {
			*(cp++) = t;
		}
		t += 0x11;
	}

	cp = fireScreen;
	uint8_t k = 0x10;
	*(RTCLOK + 2) = 0;
	while (*(RTCLOK + 2) < 200) {
		t = *(cp - 1 + k);
		t += *(cp + k);
		t += *(cp + 1 + k);
		t += *(cp + 32 + k);
		t >>= 2;
		*(cp - 31 + k) = t;
		
		t = *(cp - 1 + 0x100 + k);
		t += *(cp + 0x100 + k);
		t += *(cp + 1 + 0x100 + k);
		t += *(cp + 32 + 0x100 + k);
		t >>= 2;
		*(cp - 31 + 0x100 + k) = t;

		t = *(cp - 1 + 0x200 + k);
		t += *(cp + 0x200 + k);
		t += *(cp + 1 + 0x200 + k);
		t += *(cp + 32 + 0x200 + k);
		t >>= 2;
		*(cp - 31 + 0x200 + k) = t;
		
		// seems pointless...
		waitFrames(3);

		GTIA->COLBK = 10;
		for (int8_t k2 = 0x1f; k2 >= 0; k2--) {
			*(cp + 0x2e0 + k2) = (*RANDOM) & 0xf;
		}
		k = 0;
		// GTIA->COLBK = *RANDOM;
	}
	disableDLI();
	*PRIOR = 0;
	GTIA->COLBK = 0;
}
