#pragma encoding(atascii)
#pragma zp_reserve($00..$7f)

#include "gr.h"
#include "atari-system.h"

char dl8[] = kickasm(
		uses lms,
		uses dl8,
		uses counterLms
	) {{
	.byte $f0, $70, $70
	.byte $42, counterLms, $00
	.byte $f0, <lms, >lms
	.fill 101, $0f
	.byte $4f,0, (<lms) + $10
	.fill 89, $0f
	.byte $41, <dl8, >dl8
	}};

char dlCounter[] = kickasm(
		uses counterLms,
		uses dlCounter
	) {{
	.byte $70, $70, $70
	.byte $42, counterLms, $00
	.byte $41, <dlCounter, >dlCounter
	}};

char dlScore[] = kickasm(
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

void mode4() {}

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
	*CHBASE = >charset;
	asm {
		lda #<dlScore
		sta DLIST
		lda #>dlScore
		sta DLIST+1
	}
}
