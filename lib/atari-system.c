// Equivalent functions from https://gitlab.com/bocianu/blibs/-/blob/master/b_system.pas

#pragma code_seg(Code)

#include <atari-xl.h>
#include <6502.h>
#include "atari-system.h"

// Turns on OS back and resets machine
void systemResetB(char port_b) {
	SEI();
	*NMIEN = 0;
	*PORTB = port_b;
	(*RESVEC)();
}

void systemReset() {
	systemResetB(PORTB_SYSTEM_ON);
}

// Empty routine - for doing nothing
void do_nothing() {
	kickasm {{ 
		// do nothing
	}}
}

// Cached value of NMIEN (Non Maskable Interrupt Enable)
volatile char __nmien = 0;

// Vertical Blank Code Vector - called by nmi() on Vertical Blank (VBL) Interrupt
VEC_TYPE volatile __vblvec;

// Display List Code Vector - called by nmi() on Display List (DLI) Interrupt
VEC_TYPE volatile __dlivec;

// The NMI interrupt routine. Calls the respective Display List Code or Vertical Blank Code
__interrupt(hardware_clobber) void nmi() {
	if(*NMIST&0x80) {
		// Display List (DLI) Interrupt
		(*__dlivec)();
	} else {
		// Vertical Blank (VBL) Interrupt
		if(++RTCLOK[2]==0)
			if(++RTCLOK[1]==0)
				++RTCLOK[0];
		(*__vblvec)();
	}
}

// Empty interrupt routine - for doing nothing
__interrupt(hardware_clobber) void irq_empty() {
	kickasm {{ 
		// do nothing
	}}
}

// Turns off OS with custom PORTB($D301) value.
void systemOffB(char port_b) {
	SEI();
	*NMIEN = 0;
	*PORTB = port_b;
	*NMIVEC = &nmi;
	*IRQVEC = &irq_empty;
	__vblvec = &do_nothing;
	__dlivec = &do_nothing;
	*NMIEN = 0x40;
	__nmien = 0x40;
}

void systemOff() {
	systemOffB(PORTB_BASIC_OFF + PORTB_SELFTEST_OFF + %01111100);
}

// Enable and set custom handler for Vertical Blank Interrupt.
// To set vertical blank interrupt vector from your inline assembly code
void enableVBLI(VEC_TYPE vblptr) {
	waitFrame();
	*NMIEN = 0;
	__vblvec = vblptr;
	__nmien |= 0x40;
	*NMIEN = __nmien;
}

// Disables custom routine of Vertical Blank Interrupt
void disableVBLI() {
	waitFrame();
	*NMIEN = 0;
	__vblvec = &do_nothing;
	__nmien |= 0x40;
	*NMIEN = __nmien;
}

// Enables IRQ Interrupts
inline void enableIRQ() {
	CLI();
}

// Disables IRQ Interrupts
inline void disableIRQ() {
	SEI();
}

inline void setIRQ(VEC_TYPE irqptr) {
	*IRQVEC = irqptr;
}

// Enable and set custom handler for Display List Interrupt
void enableDLI(VEC_TYPE dliptr) {
	waitFrame();
	*NMIEN = 0;
	__dlivec = dliptr;
	__nmien |= 0xc0;
	*NMIEN = __nmien;
}

// Set custom handler for Display List Interrupt
void setDLI(VEC_TYPE dliptr) {
	__dlivec = dliptr;
}

// Disables Display List Interrupts
void disableDLI() {
	waitFrame();	
	__nmien = 0x40;
	*NMIEN = __nmien;
}

void waitFrames(signed char frames) {
	while(frames > 0) {
		waitFrame();
		frames--;
	}
}

inline void waitFrame() {
	char frame = RTCLOK[2];
	while(RTCLOK[2]==frame) ;
}

void setCharset(char msb) {
	*CHBASE = msb;
}
