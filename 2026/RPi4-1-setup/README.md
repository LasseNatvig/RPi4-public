# TDT4258 - 2026 - RPi4setup README.md

A simple code example demonstrating possible vsCode setup for Raspberry Pi4.

The folder contains a simple ARM v7 assembler program (assembler.s) and a C program (main.c). The C program calls asmAdd and checks the result. It then tests exam question no 17 from 2024.

## How to

Be sure to open vsCode with this folder as the current folder. Typing "make" in the terminal will compile the program. It can be run by typing "./test" in the terminal, og debugged by pressing F5 in the source code. Note that it takes a few seconds to start the debugger. You can singlestep both in the C code and the assembler code, and watch registers and variables.

The example has its own [canvas discussion](https://canvas.ntnu.no/courses/24553/discussion_topics/59671).

## Assumptions

The setup assumes you have installed the necessary software on the RPi4 following the RPi4 -getting started guide at canvas.

It was last tested and found OK at 2026-08-10 with these SW-versions:

* Raspberry Pi OS (Legacy, 32 bit), with updates
* vsCode version 1.132.0
* vsCode extensions: C/C++, Makefile Tools (and some others, not sure if they are needed)
