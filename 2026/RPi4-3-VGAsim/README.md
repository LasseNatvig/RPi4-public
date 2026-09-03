
# README.md for RPi4-3-VGAsim

Code example demonstrating how the small CPUlator VGA display can be simulated on Raspberry Pi4 (linux) and windows PCs. Uses #define and conditional compilation to control the single multi-version code.

Since the SDL3 setup and Makefile are different on my different computers, the Makefile and .vscode are not uploaded to the repo. It is assumed that students can handle the setup by using setups from earlier examples.

Installation of SDL3 can be a challenge, but I have experienced that cline often can help with doing it.

The code can be used for developing software using such a VGA screen on a PC or RPi4 before moving it to CPUlator.

Since the RPi4 is relatively slow, you can also use it to develop sotware on a PC before moving it to RPi4 if that is of interest.

The "portability" of code for reading keystrokes etc. has not yest been investigated.
