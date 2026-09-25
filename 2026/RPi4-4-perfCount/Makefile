CC=gcc
CFLAGS_DEBUG = -std=c11 -g -Wa,--noexecstack
CFLAGS_RELEASE = -std=c11 -O2 -Wa,--noexecstack 

SOURCES_PERF = measure.c measure.h main.c cache.c cache.h stats.c stats.h helpers.c helpers.h report.c report.h primes.c primes.h sort.c sort.h
LIBS = -lm # -lrt

# Phony targets
.PHONY: all clean run plot

# Main target builds the performance measurement tool
all: program

# Build program, optimized for performance
program: $(SOURCES_PERF)
	$(CC) $(CFLAGS_RELEASE) -o program $(SOURCES_PERF) ${LIBS}

# Clean builds
clean:
	rm -f *.tmp *.o program
 
prime:
	@./program prime 20

cache:
	@./program cache 5 1 1 40

sort:
	@./program sorting 10 2000 2000 20000 

run:
	@./program prime 3

# Plot results (requires matplotlib to be installed)
plot-prime:
	@python3 plot_times.py prime

plot-cache:
	@python3 plot_cache_stride.py cache

plot-sort:
	@python3 plot_sorting.py sort

# Debug build for development
debug:
	$(CC) $(CFLAGS_DEBUG) -o program $(SOURCES_PERF) ${LIBS}



