// Main Benchmark Starting Unit

#pragma target(atarixl)
#pragma encoding(atascii)
#pragma zp_reserve(0x00..0x7f)

#include <atari-xl.h>
#include <string.h>
#include "atari-system.h"
#include "counter.h"
#include "gr.h"

// Now the suite programs
#include "bsort.h"
#include "chessboard.h"
#include "landscape.h"
#include "lipsum.h"
#include "qr1d.h"
#include "countdownFor.h"
#include "countdownWhile.h"
#include "sieve1028.h"
#include "sieve1899.h"
#include "montecarlo.h"

void main() {
	initSuite();

	// run each benchmark
	runMontecarlo();
	runLandscape();
	runChessboard();
	// runLipsum();
	runQR1d();
	runCountdownFor();
	runCountdownWhile();
	runSieve1028();
	runSieve1899();
	runBsort();

	// Finally...
	showScore();
	for(;;) ;
}

void initSuite() {
	memcpy(charset, ATARI_CHARSET, 0x400);
	systemOff();
	memset(counterLms, 0, 40);
	memset(scoreLms, 0, 0xfff);
	initCounter();
	enableVBLI(&vblCounter);
}
