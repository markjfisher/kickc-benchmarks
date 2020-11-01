// Main Benchmark Starting Unit

#pragma target(atarixl)
#pragma encoding(atascii)
#pragma zp_reserve(0x00..0x6f)

#include <string.h>
#include <atari-xl.h>
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
#include "yoshplus.h"
#include "guessing.h"
#include "md5.h"
#include "matrixTrans.h"
// #include "fire.h"

void main() {
	initSuite();

	runBenchmark("Quatari Landscape GR9 10x", 0, 10, &benchmarkLandscape);
	runBenchmark("Chessboard GR8 200 frames", 1, 10, &benchmarkChessboard);
	runBenchmark("QR 1D Array 200 frames", 1, 10, &benchmarkQR1d);
	runBenchmark("Countdown 2ML: FOR", 0, 10, &benchmarkCountdownFor);
	runBenchmark("Countdown 2ML: WHILE", 0, 10, &benchmarkCountdownWhile);
	runBenchmark("Sieve 1028 10x", 0, 10, &benchmarkSieve1028);
	runBenchmark("Sieve 1899 10x", 0, 100, &benchmarkSieve1899);
	runBenchmark("Bubble Sort: 255 elements", 0, 10, &benchmarkBsort);
	runBenchmark("Monte Carlo Pi 10K", 0, 100, &benchmarkMontecarlo);
	runBenchmark("YoshPlus 100 frames", 1, 40, &benchmarkYoshplus);
	runBenchmark("Guessing game", 0, 40, &benchmarkGuessing);
	runBenchmark("Matrix 64x64 Trans", 0, 10, &benchmarkMatrix);

	// this doesn't like to be run indirectly... TODO
	runMd5();

	//runFire();

	// TODO: implement this correctly.
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

void runBenchmark(__ma uint8_t *name, __ma uint8_t overwrite, __ma uint8_t wait, __ma void ()*benchmark) {
	memset(lms, 0, 0x1ff0);
	prepareCounter(name);
	counterOn(1);
	(*benchmark)();
	counterOn(0);
	waitFrames(wait);
	if (overwrite == 1) {
		counterOverwrite();
	}
	counterPrint();
}
