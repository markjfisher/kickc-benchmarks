#pragma target(atarixl)
#pragma encoding(atascii)

#include <atari-xl.h>
#include <string.h>
#include <stdlib.h>
#include "atari-system.h"
#include "counter.h"
#include "gr.h"
#include "sieve.h"

void benchmarkSieve1028() {
	word count = 0;
	for (char loop: 9..0) {
		char count = 0;
		memset(sieveFlags, 1, 0x2000);
		for (char n: 2..91) {
			if (sieveFlags[n] == 1) {
				word k = n << 1;
				while (k < 0x2000) {
					sieveFlags[k] = 0;
					k += n;
				}
			}
		}
	}
	
	for (word k = 2; k < 0x2000; k++) {
		if (sieveFlags[k] == 1) count++;
	}
	showAltValue(count);
}
