// Main Benchmark Starting Unit

#pragma target(benchmark)
#pragma code_seg(Code)

#include <string.h>
#include <atari-xl.h>
#include "atari-system.h"
#include "counter.h"
#include "gr.h"

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
#include "yoshplus.h"
#include "guessing.h"
#include "md5.h"
#include "matrixTrans.h"
#include "fire.h"

void main() {
	initSuite();

	runFire();
	runLandscape();
	runChessboard();
	runQR1d();
	runCountdownFor();
	runCountdownWhile();
	runSieve1028();
	runSieve1899();
	runBsort();
	runMontecarlo();
	runYoshplus();
	runGuessing();
	runMd5();
	runMatrix();

	// runLipsum();

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
