#pragma target(atarixl)
#pragma encoding(atascii)

#include <atari-xl.h>
#include <stdint.h>
#include "atari-system.h"
#include "counter.h"
#include "gr.h"

void runLandscape() {
	memset(lms, 0, 0x1ff0);
	prepareCounter("Quatari Landscape GR9 10x");
	counterOn(1);
	benchmarkLandscape();
	counterOn(0);
	waitFrames(10);
	counterPrint();
}

void benchmarkLandscape() {
	__ma uint8_t const base[14] = { 0xaa, 0x96, 0x90, 0x90, 0x7a, 0x7a, 0x6e, 0x6e, 0x5e, 0x5e, 0x56, 0x56, 0x52, 0x50 };
	
	uint8_t colHeight[14];

	enableDLI(&priorOff);
	mode8();
	*PRIOR = 0x40;
	GTIA->COLBK = 0xb0;

	for(uint8_t loop: 0..9) {
		memcpy(colHeight, base, 14);
		for (uint8_t x: 39..0) {
			for (uint8_t i: 1..0) {
				uint8_t *screenAddress = lms + x;
				uint8_t start = 0;
				for (uint8_t c: 13..0) {
					uint8_t stop = colHeight[c];
					if (start > stop) {
						uint16_t offset = ((word) (start - stop) * 40);
						screenAddress -= offset;
						stop = start;
						start = colHeight[c];
					}
					if (i == 1) {
						while (start < stop) {
							*screenAddress = c;
							screenAddress += 40;
							start++;
						}
					} else {
						while (start < stop) {
							*screenAddress = ((*screenAddress) & 0xf) | (c << 4);
							screenAddress += 40;
							start++;
						}
					}

					start = stop;
					if ((*RANDOM) < 0x80) { colHeight[c]--; }
					if ((*RANDOM) < 0x80) { colHeight[c]++; }
				}
			}
		}
	}
	disableDLI();
	*PRIOR = 0;
	GTIA->COLBK = 0;
}
