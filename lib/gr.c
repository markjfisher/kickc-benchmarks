#include "gr.h"
#include "atari-system.h"

export char align(0x100) dl8[] = kickasm(
		uses lms,
		uses dl8,
		uses counterLms
	) {{
	.byte $f0, $70, $70
	.byte $42, counterLms, $00
	.byte $f0
	.byte $4f, <lms, >lms
	.fill 101, $0f
	.byte $4f, 0, (>lms) + $10
	.fill 89, $0f
	.byte $41, <dl8, >dl8
	}};

export char dl4[] = kickasm(
		uses lms,
		uses dl4,
		uses counterLms
	) {{
	.byte $70, $70, $70
	.byte $42, counterLms, $00
	.byte $70
	.byte $49, <lms, >lms
	.fill 34, $09
	.byte $41, <dl4, >dl4
	}};

export char dlCounter[] = kickasm(
		uses counterLms,
		uses dlCounter
	) {{
	.byte $70, $70, $70
	.byte $42, counterLms, $00
	.byte $41, <dlCounter, >dlCounter
	}};

export char dlScore[] = kickasm(
		uses scoreLms,
		uses dlScore
	) {{
	.byte $70, $70, $70
	.byte $42, <scoreLms, >scoreLms
	.fill 23, $02
	.byte $41, <dlScore, >dlScore
	}};

export char dlFire[] = kickasm(
		uses dlFire,
		uses lms,
		uses counterLms
	) {{
	.byte $f0, $70, $70
	.byte $42, counterLms, $00
	.byte $f0
	.byte $42, (<lms) - $10, (>lms) + 4
	.fill 20, $02
	.byte $41, <dlFire, >dlFire
	}};

void mode8() {
	*DLIST = dl8;	
}

void mode4() {
	*DLIST = dl4;
}

void modeFire() {
	*DLIST = dlFire;
}

void counterRow() {
	*DLIST = dlCounter;
}

void showScore() {
	waitFrame();
	// change back to main charset, should be 0x8000, so high byte = 0x80
	*CHBASE = >charset;
	*DLIST = dlScore;	
}

char * strToCode(char *s) {
	char *p = s;
	while(*p != 0) {
		char c = convertAtasciiToCode(*p);
		// in our charset, the space is now at position 0xfe, so fix it.
		if (c == 0) c = 0xfe;
		*p = c;
		p++;
	}
	return s;
}

char convertCodeToAtascii(char c) {
	char inverseBit = c & 0x80;
	char noInverse  = c & 0x7f;
	if      (noInverse < 0x40) noInverse += 0x20;
	else if (noInverse < 0x60) noInverse -= 0x40;
	return noInverse | inverseBit;
}

char convertAtasciiToCode(char c) {
	char inverseBit = c & 0x80;
	char noInverse  = c & 0x7f;
	if      (noInverse < 0x20) noInverse += 0x40;
	else if (noInverse < 0x60) noInverse -= 0x20;
	return noInverse | inverseBit;
}

void priorOff() {
	*PRIOR = 0;
	setDLI(&priorOn);
}

void priorOn() {
	*PRIOR = 0x40;
	setDLI(&priorOff);
}

void priorOffFire() {
	*PRIOR = 0;
	ANTIC->DMACTL = 0x22;

	// for the scoreboard
	*CHBASE = >(charset + 0x400);
	setDLI(&priorOnFire);
}

void priorOnFire() {
	*PRIOR = 0x40;
	ANTIC->DMACTL = 0x21;

	// for the fire
	*CHBASE = >fireCharset;
	setDLI(&priorOffFire);
}