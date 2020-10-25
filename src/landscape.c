#pragma target(atarixl)
#pragma encoding(atascii)
#pragma zp_reserve(0x00..0x7f)

#include <atari-xl.h>
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

char landscapeBase[] = kickasm {{
		.byte $AA, $96, $90, $90, $7A, $7A, $6E, $6E, $5E, $5E, $56, $56, $52, $50 
	}};

void benchmarkLandscape() {
	char colHeight[14];
	char z;
	signed char x;
	signed char i;
	signed char c;
	char uc;
	char start;
	char stop;
	
	enableDLI(&g9off);
	mode8();
	*PRIOR = 0x40;
	GTIA->COLBK = 0xb0;

	for(z: 0..9) {
		memcpy(colHeight, landscapeBase, 14);
		for (x = 39; x >= 0; x--) {
			for (i = 1; i >= 0; i--) {
				char *screenAddress = lms + x;
				start = 0;
				for (c = 13; c >= 0; c--) {
					uc = (char) c;
					stop = colHeight[uc];
					if (start > stop) {
						// Need a word here else it overflows
						screenAddress -= ((word) (start - stop) * 40);
						stop = start;
						start = colHeight[uc];
					}
					if (i == 1) {
						while (start < stop) {
							*screenAddress = uc;
							screenAddress += 40;
							start++;
						}
					} else {
						while (start < stop) {
							*screenAddress = ((*screenAddress) & 0xf) | (uc << 4);
							screenAddress += 40;
							start++;
						}
					}
					
					start = stop;
					if (((*RANDOM) & 0x1) == 0) { colHeight[uc]--; }
					if (((*RANDOM) & 0x1) == 0) { colHeight[uc]++; }
				}
			}
		}
	}
	disableDLI();
	*PRIOR = 0;
	GTIA->COLBK = 0;
}

volatile char *pOn;
volatile char *pOff;

interrupt(hardware_clobber) void g9off() {
	*PRIOR = 0;
	asm(clobbers "A") {
		lda #<g9on
		sta systemOffB.dlivec
		lda #>g9on
		sta systemOffB.dlivec+1
	}
}

interrupt(hardware_clobber) void g9on() {
	*PRIOR = 0x40;
	asm(clobbers "A") {
		lda #<g9off
		sta systemOffB.dlivec
		lda #>g9off
		sta systemOffB.dlivec+1
	}
}
