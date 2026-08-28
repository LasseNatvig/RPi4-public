# TDT4258 - 2026 - RPi4-1-setup Makefile 
###################################################################
CC=gcc
CFLAGS_DEBUG = -std=c11 -g -Wa,--noexecstack
SOURCES = sensehat_i2c_ids.c

.PHONY: all clean sensehat 

# Builds debug version
sensehat: $(SOURCES)
	$(CC) $(CFLAGS_DEBUG) -o sensehat $(SOURCES)

# Clean  
clean:
	rm -f sensehat
