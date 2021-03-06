#pragma data_seg(DList)

#include "gr.h"
#include "atari-system.h"

export char dl8[] = {
	0xf0, 0x70, 0x70,
	0x42, counterLms, 0x00,
	0xf0,
	0x4f, <lms, >lms,
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 
	0x0f, 
	0x4f, 0, (>lms) + 0x10,
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 
	0x41, <dl8, >dl8
	};

export char dl4[] = {
	0x70, 0x70, 0x70,
	0x42, counterLms, 0x00,
	0x70,
	0x49, <lms, >lms,
	0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 
	0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 
	0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 
	0x09, 0x09, 0x09, 0x09, 
	0x41, <dl4, >dl4
	};

export char dlCounter[] = {
	0x70, 0x70, 0x70,
	0x42, counterLms, 0x00,
	0x41, <dlCounter, >dlCounter
	};

export char dlScore[] = {
	0x70, 0x70, 0x70,
	0x42, <scoreLms, >scoreLms,
	0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 
	0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 
	0x02, 0x02, 0x02, 
	0x41, <dlScore, >dlScore,
	};

export char dlFire[] = {
	0xf0, 0x70, 0x70,
	0x42, counterLms, 0x00,
	0xf0,
	0x42, (<lms) - 0x10, (>lms) + 4,
	0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 
	0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 
	0x41, <dlFire, >dlFire
	};

#pragma code_seg(Code)

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
