#include <stdint.h>
#include "atari-system.h"
#include "matrixTrans.h"

// 2D matrix transpose, done with 1D arrays as kickc doesn't have 2D, but we can emulate them.

void benchmarkMatrix() {
	const uint8_t size = 64;
	uint8_t * const A = 0xA000;
	uint8_t * const B = 0xB000;
	
	for (uint8_t y: 0..(size -1)) {
		for (uint8_t x: 0..(size-1)) {
			uint16_t i = y * size + x; // emulate 2D array
			*(A + i) = *RANDOM;
		}
	}
	for (uint8_t y: 0..(size -1)) {
		for (uint8_t x: 0..(size -1)) {
			uint16_t i = y * size + x;
			uint16_t j = x * size + y;
			*(B + j) = *(A + i);
		}
	}
}
