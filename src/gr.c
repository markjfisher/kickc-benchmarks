#pragma target(atarixl)
#pragma encoding(atascii)

#include "gr.h"
#include "atari-system.h"

export char dl8[] = kickasm(
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
		uses fireScreen,
		uses counterLms
	) {{
	.byte $f0, $70, $70
	.byte $42, counterLms, $00
	.byte $f0
	.byte $4F, <fireScreen, >fireScreen
	.fill 20, $02
	.byte $41, <dlFire, >dlFire
	}};

void mode8() {
	asm {
		lda #<dl8
		sta DLIST
		lda #>dl8
		sta DLIST+1
	}
}

void mode4() {
	asm {
		lda #<dl4
		sta DLIST
		lda #>dl4
		sta DLIST+1
	}
}

void modeFire() {
	asm {
		lda #<dlFire
		sta DLIST
		lda #>dlFire
		sta DLIST+1
	}
}

void counterRow() {
	asm {
		lda #<dlCounter
		sta DLIST
		lda #>dlCounter
		sta DLIST+1
	}
}

void showScore() {
	waitFrame();

	// change back to main charset, should be 0x8000, so high byte = 0x80
	*CHBASE = >charset;
	
	asm {
		lda #<dlScore
		sta DLIST
		lda #>dlScore
		sta DLIST+1
	}

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

interrupt(hardware_clobber) void priorOff() {
	*PRIOR = 0;

	asm(clobbers "A") {
		lda #<priorOn
		sta systemOffB.dlivec
		lda #>priorOn
		sta systemOffB.dlivec+1
	}
}

interrupt(hardware_clobber) void priorOn() {
	*PRIOR = 0x40;
	asm(clobbers "A") {
		lda #<priorOff
		sta systemOffB.dlivec
		lda #>priorOff
		sta systemOffB.dlivec+1
	}
}

interrupt(hardware_clobber) void priorOffFire() {
	*PRIOR = 0;

	// for the scoreboard
	*CHBASE = >(charset + 0x400);

	asm(clobbers "A") {
		lda #<priorOnFire
		sta systemOffB.dlivec
		lda #>priorOnFire
		sta systemOffB.dlivec+1
	}
}

interrupt(hardware_clobber) void priorOnFire() {
	*PRIOR = 0x40;

	// for the fire
	*CHBASE = >fireCharset;


	asm(clobbers "A") {
		lda #<priorOffFire
		sta systemOffB.dlivec
		lda #>priorOffFire
		sta systemOffB.dlivec+1
	}
}
