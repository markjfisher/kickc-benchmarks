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
