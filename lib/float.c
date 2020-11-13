#include <stdint.h>

// Originally translated from http://www.6502.org/source/floats/wozfp1.txt (NOW FIXED)
// but after discovering constants errors in r22 and a1, and a patch
// later suggested in Dr Dobbs journal, used the version at:
// http://www.easy68k.com/paulrsm/6502/WOZFPIA.TXT
// BUT! this version has a const wrong too, so using a mixture of both sources.

__ma uint8_t __fp_sign = 0;
__ma uint8_t __fp_x2[1];
__ma uint8_t __fp_m2[3];
__ma uint8_t __fp_x1[1];
__ma uint8_t __fp_m1[3];
__ma uint8_t __fp_e[4];
__ma uint8_t __fp_z[4];
__ma uint8_t __fp_t[4];
__ma uint8_t __fp_sexp[4];
__ma uint8_t __fp_int[1];

__ma uint8_t __fp_r22[4]  = { 0x80, 0x5A, 0x82, 0x7A };
__ma uint8_t __fp_le2[4]  = { 0x7F, 0x58, 0xb9, 0x0C };
__ma uint8_t __fp_a1[4]   = { 0x80, 0x52, 0xB0, 0x40 };
__ma uint8_t __fp_mb[4]   = { 0x81, 0xAB, 0x86, 0x49 };
__ma uint8_t __fp_c[4]    = { 0x80, 0x6A, 0x08, 0x66 };
__ma uint8_t __fp_mhlf[4] = { 0x7F, 0x40, 0x00, 0x00 };


// Natural Log: ln(x), result in r
void __fp_ln(uint32_t *v, uint32_t *r) {
	kickasm(
		clobbers "AXY",
		uses v,
		uses r,
		uses __fp_sign,
		uses __fp_x2,
		uses __fp_m2,
		uses __fp_x1,
		uses __fp_m1,
		uses __fp_e,
		uses __fp_z,
		uses __fp_t,
		uses __fp_sexp,
		uses __fp_int,
		uses __fp_r22,
		uses __fp_le2,
		uses __fp_a1,
		uses __fp_mb,
		uses __fp_c,
		uses __fp_mhlf
	) {{

				// argsFP v
				lda __fp_m1
				beq error
				bpl cont
		error:	rts 		// ERROR: arg <= 0
		cont:	jsr swap
				ldx #0
				lda __fp_x2
				ldy #$80
				sty __fp_x2
				eor #$80
				sta __fp_m1+1
				bpl !+
				dex
				stx __fp_m1
		!:
				jsr float
		sexp1:	mloadFP __fp_x2 : __fp_z : __fp_x1 : __fp_sexp : __fp_r22 : __fp_x1
				jsr fsub
		savet:	mloadFP __fp_x1 : __fp_t : __fp_z : __fp_x1 : __fp_r22 : __fp_x2
				jsr fadd
		tm2:	loadFP __fp_t : __fp_x2
				jsr fdiv
		mit:	loadFP __fp_x1 : __fp_t : __fp_x2
				jsr fmul
				jsr swap
		mic:	loadFP __fp_c : __fp_x1
				jsr fsub
		m2mb:	loadFP __fp_mb : __fp_x2
				jsr fdiv
		m2a1:	loadFP __fp_a1 : __fp_x2
				jsr fadd
		m2t:	loadFP __fp_t : __fp_x2
				jsr fmul
		m2mhl:	loadFP __fp_mhlf : __fp_x2
				jsr fadd
		ldexp:	loadFP __fp_sexp : __fp_x2
				jsr fadd
		mle2:	loadFP __fp_le2 : __fp_x2
				jsr fmul
				returnFP r

		float:
		fsub:
		fadd:
		fdiv:
		fmul:
		swap:
				nop

	}}
}
