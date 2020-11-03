#include <stdlib.h>

// This is quicker than the library routine memset by about 40 frames.
void clearSieveData() {
	uint8_t *p = sieveFlags;
	for (register uint8_t i: 0..0x1f) {
		for (register uint8_t j: 0..0xff) {
			*(p + j) = 1;
		}
		p += 0x100;
	}
}

void clearSieveDataAsm() {
	kickasm(
		clobbers "AXY",
		uses sieveFlags
	) {{
			.label ps = $fe
			// store $fe/$ff zp address so we don't clobber it.
			lda ps
			sta restore1+1
			lda ps+1
			sta restore2+1
			
			lda #<sieveFlags
			sta ps
			lda #>sieveFlags
			sta ps+1

			ldx #0
		!l1:
			ldy #0
			lda #1
		!l2:
			sta (ps),y
			iny
			cpy #0
			bne !l2-
			
			clc
			lda ps+1
			adc #1
			sta ps+1
			
			inx
			cpx #$20
			bne !l1-
			
		restore1:
			lda #0
			sta ps
			
		restore2:
			lda #0
			sta ps+1
	}}

}
