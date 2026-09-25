# Function Performance Measurement Guide

## Overview
This document describes the methodology for measuring function performance using command-line arguments for **array size range (start, max, step)** and **number of experiments (E)**, along with hardware performance counter readings on ARM Cortex-A72 processors.

---

## Methodology

### Command-Line Arguments
The program accepts five command-line arguments:
1. **expName**: Name of the experiment to run (prime, cache, or sorting)
2. **E**: Number of experiments to run for averaging
3. **start**: Starting value for the parameter being varied
4. **step**: Increment between parameter values
5. **end**: Ending value for the parameter being varied

Note: For the **cache** experiment, the parameter varied is the stride (1, 2, 4, etc.). For the **sorting** experiment, the parameter varied is the array size. For the **prime** experiment, start/step/end are not used.

### Example Usage
```bash
# Run prime experiment with 3 experiments
./program prime 3

# Run cache experiment with 5 experiments, stride from 1 to 40 in steps of 1
./program cache 5 1 1 40

# Run sorting experiment with 10 experiments, array size from 2000 to 20000 in steps of 2000
./program sorting 10 2000 2000 20000

# Via Makefile targets
make run        # Runs: ./program prime 3
make cache      # Runs: ./program cache 5 1 1 40
make sort       # Runs: ./program sorting 10 2000 2000 20000
```

---

## Performance Counters

Uses Linux `perf_event_open()` to access ARM Cortex-A72 PMU:

| Counter | Event | Description |
|---------|-------|-------------|
| Cycles | `PERF_COUNT_HW_CPU_CYCLES` | CPU cycles consumed |
| Instructions | `PERF_COUNT_HW_INSTRUCTIONS` | Instructions retired |
| Cache Refs | `PERF_COUNT_HW_CACHE_REFERENCES` | Cache references |
| Cache Misses | `PERF_COUNT_HW_CACHE_MISSES` | Cache misses |
| Branch Misses | `PERF_COUNT_HW_BRANCH_MISSES` | Branch instruction misses |

### Derived Metrics
- **IPC**: Instructions / Cycles
- **CacheMiss%**: (CacheMisses / CacheRefs) * 100%
- **BranchMissPerInstr**: BranchMisses / Instructions (per-instruction rate)
- **Execution Time**: Wall-clock time in seconds
- **RSD%**: Relative standard deviation across E experiments

### Output Value Units
- Cycles, Instructions: displayed in millions (M)
- Cache Refs: displayed in thousands (k)
- Cache Misses, Branch Misses: raw values
- IPC: ratio
- CacheMiss%: percentage
- BranchMissPerInstr: scientific notation
- Time: seconds

---


### Execution Loop
For the **sorting** experiment, array size is varied from start to end in steps of step:
```c
for (int size = start; size <= end; size += step) {
    int* arr = malloc(size * sizeof(int));
    
    for (int exp = 0; exp < E; exp++) {
        srand(0);
        generateRandomArray(arr, size);
        
        // Start measurement
        startExperiment();
        
        // Execute sorting algorithm
        insertionSort(arr, size);
        // or bubbleSort(arr, size);
        
        // Stop measurement
        endExperiment(exp);
    }
    printTiming(outfile, times, E);
    printCounterStats(outfile, counterVals, E);
    free(arr);
}
```

For the **cache** experiment, stride is varied from start to end in steps of step:
```c
for (int stride = start; stride <= end; stride += step) {
    for (int exp = 0; exp < E; exp++) {
        startExperiment();
        strided(arr, size, stride);
        endExperiment(exp);
    }
    printTiming(outfile, times, E);
    printCounterStats(outfile, counterVals, E);
}
```

For the **prime** experiment, the function is run E times with fixed parameters:
```c
for (int exp = 0; exp < E; exp++) {
    startExperiment();
    long result = nth_prime(100000);
    endExperiment(exp);
}
printTiming(outfile, times, E);
printCounterStats(outfile, counterVals, E);
```

### Output
- Statistics printed to console with formatted columns
- All data saved to `res/res-<timestamp>.txt`
- File contains: Experiment name, Experiment count (E), start/step/end parameters, and performance counter statistics for each experiment

---
