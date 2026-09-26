# TDT4258-2026-RPi4-4 Performance Counters

This code has been developed on an RPi4 with vsCode & Cline running Mistral Medium on Idun@NTNU. The code from Mistral Medium has been reviewed, discussed, tested, changed and commented. It can still contain errors or weak parts. Please send comments to your teacher in e-mail if you have ideas for improvements. Last updated 2026-09-25, Lasse Natvig.

The main purpose of the example is to help students read performance counters on the Raspberry Pi, to demonstrate how we typically organize C-code into several modules, and one of many possible ways to run experiments, store its results and vizualize them. The python plotting scripts are made by "vibe coding" and has only been given simplified review by visual inspection of the plots.

## What

* **main.c** demonstrates the use of performance counters for measuring detailed execution statistics for various smaller functions. It was **introduced in Lecture 5** and used to demonstrate the effect of cold start, disturbance from other programs, and strided access to arrays.
* You would normally have one project and main program for each experiment, but in this case we wanted to illustrate several different smaller example experiments in one TDT4258-RPi4-example.
* The main program allocates storage for the performance counters, and reports the results both in the terminal (console) and to a timestamped file that is stores in subfolder res.
* The main program takes these arguments from the command line:
  * experiment name (prime, cache, sorting)
  * E, number of repeated runs
  * start, step, and end value for a parameter that for sorting can be number of elements in the array to sort (n): the experiment is run for n = start, start + step, start + 2*step until end. In the cache experiment the same "mechanism" used to specify a range of stride values

## How to use

* The makefile contains various examples of use, such as "make sort", "make prime" etc. The plotting scripts will look in the res-folder after the latest .txt file report results with experiment name prime, cache or sort.

## Which parts of the example is part of the TDT4258-2026 reading list

* main.c
* File structure with several "modules" such as cache.c/.h, and classical header guards
* the function parseArgs() in helpers.c

## Comments

* If you have comments to the example, please send them in an e-mail to the TDT4258 course teacher.
  