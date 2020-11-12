
#include <atari-xl.h>
#include "atari-system.h"
#include "counter.h"
#include "gr.h"

void runQR1d() {
	memset(lms, 0, 0x1ff0);
	prepareCounter("QR 1D Array 200 frames");
	counterOn(1);
	benchmarkQR1d();
	counterOn(0);
	waitFrames(10);
	counterOverwrite();
	counterPrint();
}

char align(0x100) qr[] = {
   0b11111111,0b11111111,0b11111111,0b11111111,0b11100000,
   0b10000000,0b10001010,0b01111111,0b00100000,0b00100000,
   0b10111110,0b10001101,0b00000111,0b11101111,0b10100000,
   0b10100010,0b10100100,0b01001110,0b01101000,0b10100000,
   0b10100010,0b10010100,0b01101101,0b01101000,0b10100000,
   0b10100010,0b11111100,0b11101101,0b00101000,0b10100000,
   0b10111110,0b10110010,0b01000010,0b01101111,0b10100000,
   0b10000000,0b10101010,0b10101010,0b10100000,0b00100000,
   0b11111111,0b11011001,0b10001111,0b00111111,0b11100000,
   0b10011000,0b11110100,0b11010101,0b00110100,0b00100000,
   0b10011001,0b01001011,0b00100111,0b11011001,0b10100000,
   0b10100010,0b10110011,0b10011100,0b11111010,0b01100000,
   0b10101111,0b01100101,0b01001111,0b01011111,0b00100000,
   0b11101000,0b00101000,0b00010010,0b10111100,0b11100000,
   0b10100011,0b10011111,0b10001011,0b00000001,0b10100000,
   0b10110000,0b10101100,0b00001110,0b01110010,0b01100000,
   0b10011001,0b00101011,0b10011110,0b10100001,0b00100000,
   0b10110110,0b00110010,0b01000001,0b11011011,0b11100000,
   0b10110111,0b01011000,0b11101010,0b10111101,0b10100000,
   0b10111100,0b00100111,0b01011100,0b11111010,0b01100000,
   0b10010001,0b01111011,0b01011000,0b11110001,0b11100000,
   0b11100010,0b10010101,0b11010001,0b00001011,0b11100000,
   0b10010101,0b01001011,0b00101101,0b11011011,0b10100000,
   0b11101100,0b10011110,0b00111000,0b11111011,0b01100000,
   0b11110111,0b10100101,0b10011101,0b01111101,0b11100000,
   0b10001010,0b11000111,0b11010001,0b10000010,0b01100000,
   0b11111111,0b10011000,0b10011111,0b10111011,0b01100000,
   0b10000000,0b11011101,0b01011000,0b10101000,0b11100000,
   0b10111110,0b10111101,0b01001000,0b00111001,0b00100000,
   0b10100010,0b10001000,0b11001001,0b10000011,0b10100000,
   0b10100010,0b11010101,0b00100111,0b10111111,0b00100000,
   0b10100010,0b11101010,0b00011100,0b10111101,0b01100000,
   0b10111110,0b10010000,0b01011100,0b11000001,0b11100000,
   0b10000000,0b10100010,0b10110001,0b11101111,0b10100000,
   0b11111111,0b11111111,0b11111111,0b11111111,0b11100000
};

void benchmarkQR1d() {
	mode4();
	clearAltScore();

	char y = 0;
	*(RTCLOK + 2) = 0;
	while (*(RTCLOK + 2) < 200) {
		char *p = lms;
		for (char x: 0..174) {
			*(p + y++) = *(qr + x);
			if (y == 5) {
				// mode 4 has 10 bytes per line?
				p += 10;
				y = 0;
			}
		}
		(*scoreA4)++;
		if (*scoreA4 == 10) { *scoreA4 = 0; (*scoreA3)++; }
		if (*scoreA3 == 10) { *scoreA3 = 0; (*scoreA2)++; }
		if (*scoreA2 == 10) { *scoreA2 = 0; (*scoreA1)++; }
	}
}
