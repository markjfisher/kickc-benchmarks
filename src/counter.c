#pragma target(atarixl)
#pragma encoding(atascii)
#pragma zp_reserve(0x00..0x7f)

#include <atari-xl.h>
#include "atari-system.h"
#include "gr.h"

// The current benchmark's name
char *benchName;
char *currentPrintPosition = scoreLms;

// counter on off flag, used in the VBL, so make it volatile
volatile char counterOnValue = 0;

// 0 = off, 1 = on
void counterOn(char onoff) {
	counterOnValue = onoff;
}

interrupt(hardware_clobber) void vblCounter() {
	char * const a = 0x20;
	char * const b = 0x21;
	char * const c = 0x22;
	char * const d = 0x23;
	char * const e = 0x24;

	if (counterOnValue != 0) {
		(*e)++;
		if (*e == 10) { *e = 0; (*d)++; }
		if (*d == 10) { *d = 0; (*c)++; }
		if (*c == 10) { *c = 0; (*b)++; }
		if (*b == 10) { *b = 0; (*a)++; }
	}
}

void initCounter() {
	memcpy(charset + 0x400, charset, 0x400);
	memcpy(charset + 0x400, charset + 0x80, 80); // feels like this should be 0x80
	for(char i: 0..7) {
		char * a = charset + 0x800 - 8 + i;
		*a = *(charset + 0x80 + i) ^ 0xff;
	}
	memset(charset + 0x800 - 0x10, 0xff, 8);
}

void prepareCounter(char *name) {
	waitFrame();
	counterRow();
	*CHBASE = >(charset + 0x400);
	benchName = name;
	memset(counterLms, 0xfe, 0x28);
	memset(counterLms, 0, 5);
	
	char *p = name;
	while (*p != 0) {
		if (*p == 0) *p = 0xfe;
		p++;
	}
	memcpy(counterLms + 6, name, strlen(name));
}

void counterPrint() {
	char *position = currentPrintPosition;
	memcpy(position, benchName, strlen(benchName));
	position += 26;
	for(char i: 0..4) {
		char * a = position + i;
		*a = *(counterLms + i) + 16;
	}
	currentPrintPosition += 40;
}
