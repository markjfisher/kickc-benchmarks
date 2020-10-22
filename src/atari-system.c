// Equivalent functions from https://gitlab.com/bocianu/blibs/-/blob/master/b_system.pas
#pragma zp_reserve(0x00..0x7f)

#include <atari-xl.h>
#include "atari-system.h"

volatile char __nmien = 0;

// Turns on OS back and resets machine
void systemResetB(char port_b) {
	asm { sei }
	*NMIEN = 0;
	*PORTB = port_b;
	asm(clobbers "AXY") {
		jmp (RESVEC)
	}
}

void systemReset() {
	systemResetB(PORTB_SYSTEM_ON);
}

// Turns off OS with custom PORTB($D301) value.
void systemOffB(char port_b) {
	asm {
		sei
		lda #0
		sta NMIEN

		lda #port_b
		sta PORTB

		lda #<nmi
		sta NMIVEC
		lda #>nmi
		sta NMIVEC+1

		lda #<systemOffB.iret
		sta IRQVEC
		sta systemOffB.vblvec
		sta systemOffB.dlivec

		lda #>systemOffB.iret
		sta IRQVEC+1
		sta systemOffB.vblvec+1
		sta systemOffB.dlivec+1

		lda #$40
		sta NMIEN
		sta __nmien
		bne systemOffB.stop
	nmi:
		bit NMIST
		bpl systemOffB.vbl
	}
	kickasm {{
		.label dlivec = * + 1
		jmp dlivec
	}}

	asm {
		rti
	vbl:
		inc RTCLOK+2
		bne systemOffB.vblvec-1
		inc RTCLOK+1
		bne systemOffB.vblvec-1
		inc RTCLOK
	}
	kickasm {{
		.label vblvec = * + 1
	}}
	asm(clobbers "AXY") {
		jmp systemOffB.vblvec
	iret:
		rti
	stop:
	}
}

void systemOff() {
	systemOffB(PORTB_BASIC_OFF + PORTB_SELFTEST_OFF + %01111100);
}

// Enable and set custom handler for Vertical Blank Interrupt.
// To set vertical blank interrupt vector from your inline assembly code
void enableVBLI(void *vblptr) {
	waitFrame();
	asm {
		lda #0
		sta NMIEN

		lda #<vblptr
		sta systemOffB.vblvec
		lda #>vblptr
		sta systemOffB.vblvec+1

		lda __nmien
		ora #$40
		sta NMIEN
		sta __nmien
	}
	
}

// Disables custom routine of Vertical Blank Interrupt
void disableVBLI() {
	waitFrame();
	asm {
		lda #0
		sta NMIEN

		lda #<systemOffB.iret
		sta systemOffB.vblvec
		lda #>systemOffB.iret
		sta systemOffB.vblvec+1

		lda __nmien
		ora #$40
		sta NMIEN
		sta __nmien
	}
}

// Enables IRQ Interrupts
inline void enableIRQ() {
	asm { cli }
}

// Disables IRQ Interrupts
inline void disableIRQ() {
	asm { sei }
}

inline void setIRQ(void * irqptr) {
	asm {
		lda #<irqptr
		sta IRQVEC
		lda #>irqptr
		sta IRQVEC+1
	}
}

// Enable and set custom handler for Display List Interrupt
void enableDLI(void *dliptr) {
	waitFrame();
	asm {
		lda #0
		sta NMIEN

		lda #<dliptr
		sta systemOffB.dlivec
		lda #>dliptr
		sta systemOffB.dlivec+1

		lda __nmien
		ora #$c0
		sta NMIEN
		sta __nmien
	}
}

// Disables Display List Interrupts
void disableDLI() {
	waitFrame();
	*NMIEN = 0x40;
	__nmien = 0x40;
}

void waitFrames(char frames) {
	while(frames > 0) {
		waitFrame();
		frames--;
	}
}

inline void waitFrame() {
	asm {
			lda RTCLOK+2
		!:	cmp RTCLOK+2
			beq !-
	}
}

void setCharset(char msb) {
	*CHBASE = msb;
}
