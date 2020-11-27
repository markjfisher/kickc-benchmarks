#pragma code_seg(Code)

#include <atari-xl.h>
#include <stdint.h>
#include <6502.h>
#include "atari-system.h"
#include "counter.h"
#include "gr.h"

void runFire() {
	memset(lms, 0, 0x1ff0);
	prepareCounter("Flames GTIA 250 frames    \x1c");
	counterOn(1);
	benchmarkFire();
	counterOn(0);
	counterOverwrite();
	waitFrames(40);
	counterPrint();
}

void benchmarkFire() {
	clearAltScore();
	*SDMCTL = 0x21;

	enableDLI(&priorOffFire);
	modeFire();
	*PRIOR = 0x40;
	GTIA->COLBK = 0x20;

	uint8_t t = 0;
	uint8_t *p = fireCharset;
	
	for(uint8_t i: 0..0xf) {
		for (uint8_t j: 0..7) {
			*(p++) = t;
		}
		t += 0x11;
	}
	
	clearAltScore();
	
	*(RTCLOK + 2) = 0;
	while (*(RTCLOK + 2) < 250) {
		uint8_t *p0 = fireScreen - 31;
		uint8_t *p1 = fireScreen - 31 + 0x100;
		uint8_t *p2 = fireScreen - 31 + 0x200;

		for (uint8_t i: 0..255) {
			*(p0 + i) = (*(p0+30 + i) + *(p0+31 + i) + *(p0+32 + i) + *(p0+63 + i)) >> 2;
			*(p1 + i) = (*(p1+30 + i) + *(p1+31 + i) + *(p1+32 + i) + *(p1+63 + i)) >> 2;
			*(p2 + i) = (*(p2+30 + i) + *(p2+31 + i) + *(p2+32 + i) + *(p2+63 + i)) >> 2;
		}
		
		p0 = fireScreen + 0x2e0;
		for (uint8_t i: 0x1f..0) {
			*(p0 + i) = (*RANDOM) & 0xf;
		}
		
		(*scoreA4)++;
		if (*scoreA4 == 10) { *scoreA4 = 0; (*scoreA3)++; }
		if (*scoreA3 == 10) { *scoreA3 = 0; (*scoreA2)++; }
		if (*scoreA2 == 10) { *scoreA2 = 0; (*scoreA1)++; }

	}

	disableDLI();
	*PRIOR = 0;
	ANTIC->DMACTL = 0x22;
	GTIA->COLBK = 0;
}
