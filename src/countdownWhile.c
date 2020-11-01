#pragma target(atarixl)
#pragma encoding(atascii)

#include <atari-xl.h>
#include "atari-system.h"
#include "counter.h"
#include "gr.h"

void benchmarkCountdownWhile() {
	char *c1 = counterLms + 0x21;
	char *c2 = counterLms + 0x22;
	char *c3 = counterLms + 0x23;
	char *c4 = counterLms + 0x24;
	char *c5 = counterLms + 0x25;
	char *c6 = counterLms + 0x26;
	char *c7 = counterLms + 0x27;

	*c1 = 1;
	do {
		*c2 = 9;
		do {
			*c3 = 9;
			do {
				*c4 = 9;
				do {
					*c5 = 9;
					do {
						*c6 = 9;
						do {
							*c7 = 9;
							do {
								(*c7)--;
							} while (*c7 != 0xff);
							(*c6)--;
						} while (*c6 != 0xff);
						(*c5)--;
					} while (*c5 != 0xff);
					(*c4)--;
				} while (*c4 != 0xff);
				(*c3)--;
			} while (*c3 != 0xff);
			(*c2)--;
		} while (*c2 != 0xff);
		(*c1)--;
	} while (*c1 != 0xff);
	
}
