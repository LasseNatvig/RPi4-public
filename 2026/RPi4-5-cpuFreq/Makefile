CC=gcc
CFLAGS_DEBUG = -std=c17 -g -Wa,--noexecstack
SOURCES = cpuFreqTest.c 

cpuFreq: $(SOURCES)
	$(CC) $(CFLAGS_DEBUG) -o cpuFreq $(SOURCES) ${LIBS}

parallelPi: parallelPi.c
	$(CC) -std=c17 -O2 -pthread -o parallelPi parallelPi.c

setFreq_kHz: utils/setFreq_kHz.c
	$(CC) -std=c17 -O2 -o $@ $<

setGovernor: utils/setGovernor.c
	$(CC) -std=c17 -O2 -o $@ $<

all: cpuFreq parallelPi setFreq_kHz setGovernor

clean:
	rm -f *.tmp cpuFreq parallelPi setFreq_kHz setGovernor

# Phony targets
.PHONY: all clean plot parPlot

test: cpuFreq parallelPi
	@python3 test.py cpuFreq 

plot: results.txt
	@python3 plot.py

parPlot: results.txt
	@python3 parPlot.py



