#pragma target(atarixl)
#pragma encoding(atascii)
#pragma zp_reserve(0x00..0x7f)

#include "gr.h"
#include "atari-system.h"

char align(0x400) dl8[] = kickasm(
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

char align(0x400) dl4[] = kickasm(
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

char align(0x400) dlCounter[] = kickasm(
		uses counterLms,
		uses dlCounter
	) {{
	.byte $70, $70, $70
	.byte $42, counterLms, $00
	.byte $41, <dlCounter, >dlCounter
	}};

char align(0x400) dlScore[] = kickasm(
		uses scoreLms,
		uses dlScore
	) {{
	.byte $70, $70, $70
	.byte $42, <scoreLms, >scoreLms
	.fill 23, $02
	.byte $41, <dlScore, >dlScore
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
