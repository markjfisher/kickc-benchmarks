// Add these to global definitions

// Internal realtime clock at 0x12, 0x13, 0x14.
// Location 0x14 increments every stage 1 VBLANK interrupt
// (1/60 second = 1 jiffy) until it reaches #0xFF, then increments 0x13, etc.
char * const RTCLOK = (char *) 0x12;

// (W) Priority selection register. PRIOR establishes which objects
// on the screen (players, missiles, and playfields) will be in front of
// other objects.
char * const PRIOR = (char *) 0xD01B;

// When this location is read, it acts as a random number
// generator. It reads the high order eight bits of the 17 bit
// polynomial counter (nine bit if BIT 7 of AUDCTL is set) for the
// value of the number.
char * const RANDOM = (char *) 0xD20A;

// POKE with zero, and VBLANK and system clock are disabled, and shadowing is suspended
char * const NMIEN = (char *) 0xD40E;

// disable the ROM between $C000-$CFFF and $D800-$FFFF by setting bit 0 to 0.
// Bit 1 controls BASIC; if 0, BASIC is enabled, if 1, it is disabled
// Bits 2 and 3 control the 1200XL LEDs; 0 means on, 1 means off
// Bits 4-6 are reserved (unused) in the XL and 65XE.
// Bits 4 and 5 in the 130XE are used to enable bank switching
// Bit 7 controls the RAM region $5000-$57FF, normally enabled (1).
char * const PORTB = (char *) 0xD301;

// Display list pointer. Tells the OS the address of the display list
// instructions about what screen mode(s) to display and where to
// find the screen data. See also shadow register SDLIST ($230, $231).
char** const DLIST = (char **) 0xD402;

// (W) Character base address; the location of the start of the
// character set, either the standard Atari set or a user-designed set.
// The default is 224 ($EO), which points to the start of the Atari
// ROM character set
char * const CHBASE = (char *) 0xD409;

// Non Maskable Interrupt Status - holds cause for the NMI interrupt in BITs 5, 6 and 7
char * const NMIST  = (char *) 0xD40F;

// Non Maskable Interrupt Reset - Reset for NMIST. Clears the interrupt request register and resets all of the NMI status together
void * const NMIRES = (void *) 0xD40F;

void * const ATARI_CHARSET = (void *) 0xE000;

// Copied from cx16.h, "pointer to interrupt function"
typedef void (*VEC_TYPE)(void);

VEC_TYPE* const NMIVEC = (VEC_TYPE*)0xFFFA;
VEC_TYPE* const RESVEC = (VEC_TYPE*)0xFFFC;
VEC_TYPE* const IRQVEC = (VEC_TYPE*)0xFFFE;

const char PORTB_SELFTEST_OFF = %10000000;
const char PORTB_BASIC_OFF    = %00000010;
const char PORTB_SYSTEM_ON    = %00000001;

// Turns off OS with custom PORTB($D301) value.
void systemOffB(char port_b);

// Turns off OS and BASIC. Gives maximum available RAM
void systemOff();

// Turns on OS back and resets machine
void systemResetB(char port_b);

// Calls with default arg
void systemReset();

// Enable and set custom handler for Vertical Blank Interrupt.
// To set vertical blank interrupt vector from your inline assembly code
void enableVBLI(VEC_TYPE vblptr) ;

// Disables custom routine of Vertical Blank Interrupt
void disableVBLI();

// Enables IRQ Interrupts
void enableIRQ();

// Disables IRQ Interrupts
void disableIRQ();

// Set IRQ Interrupt Vector
void setIRQ(VEC_TYPE irqptr);

// Enable and set custom handler for Display List Interrupt
void enableDLI(VEC_TYPE dliptr);

// Set custom handler for Display List Interrupt
void setDLI(VEC_TYPE dliptr);

// Disables Display List Interrupts
void disableDLI();

// Waits till drawing of current frame ends
void waitFrame();

// Waits for a specified number of frames.
// Each frame is 1/50 sec for PAL systems, or 1/60 sec for NTSC
void waitFrames(signed char frames);

// Sets font charset located at specified page of memory
void setCharset(char msb);
