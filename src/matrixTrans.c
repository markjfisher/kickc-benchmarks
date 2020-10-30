#pragma target(atarixl)
#pragma encoding(atascii)
#pragma zp_reserve(0x00..0x7f)

#include <stdint.h>
#include <stdlib.h>
#include "atari-system.h"
#include "counter.h"
#include "gr.h"
#include "matrixTrans.h"

// 2D matrix transpose, done with 1D arrays as kickc doesn't have 2D, but we can emulate them.

void runMatrix() {
	memset(lms, 0, 0x1ff0);
	prepareCounter("Matrix 64x64 Trans");

	counterOn(1);
	benchmarkMatrix();
	counterOn(0);

	waitFrames(10);
	counterPrint();
}

void benchmarkMatrix() {
	const uint8_t size = 64;
	uint8_t * const A = 0xA000;
	uint8_t * const B = 0xB000;
	
	for (uint8_t y = 0; y < size; y++) {
		for (uint8_t x = 0; x < size; x++) {
			uint16_t i = y * size + x; // emulate 2D array
			*(A + i) = *RANDOM;
		}
	}
	for (uint8_t y = 0; y < size; y++) {
		for (uint8_t x = 0; x < size; x++) {
			uint16_t i = y * size + x;
			uint16_t j = x * size + y;
			*(B + j) = *(A + i);
		}
	}
}
