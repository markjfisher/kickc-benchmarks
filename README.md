# kickc-benchmarks

A series of benchmarks using [kickc](https://gitlab.com/camelot/kickc) c compiler.

The suite is directly translated from [mad pascal
bench suite](https://github.com/zbyti/a8-mad-pascal-bench-suite).

For the latest binary, load [suite.xex](bin/suite.xex).

## Building

There is a Makefile to build the output. If kickc.sh is not on the path, export `KICKC_HOME` first.

	export KICKC_HOME=/path/to/your/kickc-distribution
    make

## TODO

1. Hook up emulator to the build script
2. Generate Altirra style debug files from output

## Latest test results:

![suite results](results/suite.png)
